// Copyright 2012 Alessio Sclocco <a.sclocco@vu.nl>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <H5Cpp.h>
using std::ios;
using std::string;
using std::ifstream;
using std::vector;
using std::sqrt;
using H5::H5File;
using H5::Group;
using H5::DataSet;
using H5::FloatType;
using H5::IntType;

#include <Observation.hpp>
#include <CLData.hpp>
#include <utils.hpp>
using isa::OpenCL::CLData;
using isa::utils::toStringValue;
using isa::utils::changeEndianness;


#ifndef READ_DATA_HPP
#define READ_DATA_HPP

namespace AstroData {

template< typename T > void readSIGPROC(Observation< T > & observation, unsigned int bytestoSkip, string inputFilename, vector< CLData< T > * > & data, unsigned int firstSecond = 0);
template< typename T > void readLOFAR(string headerFilename, string rawFilename, Observation< T > &observation, vector< CLData< T > * > & data, unsigned int nrSeconds = 0, unsigned int firstSecond = 0);


// Implementation

template< typename T > void readSIGPROC(Observation< T > & observation, unsigned int bytesToSkip, string inputFilename, vector< CLData< T > * > & data, unsigned int firstSecond) {
	ifstream inputFile;
	const unsigned int BUFFER_DIM = 4;
	char * buffer = new char [BUFFER_DIM];

	inputFile.open(inputFilename.c_str(), ios::binary);
	inputFile.sync_with_stdio(false);
	inputFile.ignore(bytesToSkip);
	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		data.at(second) = new CLData< T >("second" + toStringValue< unsigned int >(second), true);
		(data.at(second))->setDeviceReadOnly();
		(data.at(second))->allocateHostData(observation.getNrSamplesPerPaddedSecond() * observation.getNrChannels());

		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int channel = observation.getNrChannels(); channel > 0; channel-- ) {
				inputFile.read(buffer, BUFFER_DIM);
				((data.at(second))->getHostData())[((channel - 1) * observation.getNrSamplesPerPaddedSecond()) + sample] = *(reinterpret_cast< T * >(buffer));
			}
		}
	}
	inputFile.close();

	delete [] buffer;
}


template< typename T > void readLOFAR(string headerFilename, string rawFilename, Observation< T > &observation, vector< CLData< T > * > & data, unsigned int nrSeconds, unsigned int firstSecond) {
	unsigned int totalSamples = 0;
	unsigned int nrSubbands = 0;
	unsigned int nrChannels = 0;
	char * word = new char[4];
	double totalIntegrationTime = 0.0;

	// Read the HDF5 file with the metadata
	H5File headerFile = H5File(headerFilename, H5F_ACC_RDONLY);
	FloatType typeDouble = FloatType(H5::PredType::NATIVE_DOUBLE);
	double valueDouble = 0.0;
	IntType typeUInt = IntType(H5::PredType::NATIVE_UINT);
	unsigned int valueUInt = 0;

	Group currentNode = headerFile.openGroup("/");
	currentNode.openAttribute("OBSERVATION_FREQUENCY_MAX").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
	observation.setMaxFreq(valueDouble);
	currentNode.openAttribute("OBSERVATION_FREQUENCY_MIN").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
	observation.setMinFreq(valueDouble);
	currentNode = currentNode.openGroup(currentNode.getObjnameByIdx(0));
	currentNode.openAttribute("TOTAL_INTEGRATION_TIME").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
	totalIntegrationTime = valueDouble;
	currentNode.openAttribute("NOF_BEAMS").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	observation.setNrBeams(valueUInt);
	currentNode = currentNode.openGroup(currentNode.getObjnameByIdx(0));
	currentNode.openAttribute("NOF_SAMPLES").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	totalSamples = valueUInt;
	currentNode.openAttribute("NOF_STATIONS").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	observation.setNrStations(valueUInt);
	currentNode.openAttribute("CHANNELS_PER_SUBBAND").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	nrChannels = valueUInt;
	currentNode.openAttribute("CHANNEL_WIDTH").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
	observation.setChannelBandwidth(valueDouble / 1000000);
	DataSet currentData = currentNode.openDataSet("STOKES_0");
	currentData.openAttribute("NOF_SUBBANDS").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	nrSubbands = valueUInt;
	headerFile.close();

	observation.setNrSamplesPerSecond(static_cast< unsigned int >(totalSamples / totalIntegrationTime));
	if ( nrSeconds == 0 ) {
		observation.setNrSeconds(static_cast< unsigned int >(totalIntegrationTime));
	} else {
		if ( static_cast< unsigned int >(totalIntegrationTime) >= (firstSecond + nrSeconds) ) {
			observation.setNrSeconds(nrSeconds);
		} else {
			observation.setNrSeconds(static_cast< unsigned int >(totalIntegrationTime) - firstSecond);
		}
	}
	observation.setNrChannels(nrChannels * nrSubbands);

	// Read the raw file with the actual data
	ifstream rawFile;
	rawFile.open(rawFilename.c_str(), ios::binary);
	rawFile.sync_with_stdio(false);
	rawFile.seekg(firstSecond * observation.getNrSamplesPerSecond() * nrSubbands * nrChannels, ios::beg);

	data.resize(observation.getNrSeconds());

	double *aOld = new double [nrSubbands * nrChannels];
	double *vOld = new double [nrSubbands * nrChannels];

	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		data.at(second) = new CLData< T >("second" + toStringValue< unsigned int >(second), true);
		(data.at(second))->setDeviceReadOnly();
		(data.at(second))->allocateHostData(observation.getNrChannels() * observation.getNrSamplesPerPaddedSecond());
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int subband = 0; subband < nrSubbands; subband++ ) {
				for ( unsigned int channel = 0; channel < nrChannels; channel++ ) {
					long long unsigned int element = (second * observation.getNrSamplesPerSecond()) + sample;
					T value = 0;

					rawFile.read(word, 4);
					changeEndianness(word);
					value = *(reinterpret_cast< T * >(word));

					((data.at(second))->getHostData())[(((subband * nrChannels) + channel) * observation.getNrSamplesPerPaddedSecond()) + sample] = value;
					if ( value <  observation.getMinValue() ) {
						observation.setMinValue(value);
					}
					if ( value > observation.getMaxValue() ) {
						observation.setMaxValue(value);
					}

					if ( element == 0 ) {
						observation.setAverage((subband * nrChannels) + channel, value);
						observation.setVariance((subband * nrChannels) + channel, 0.0);
					}
					else {
						aOld[(subband * nrChannels) + channel] = observation.getAverage((subband * nrChannels) + channel);
						vOld[(subband * nrChannels) + channel] = observation.getVariance((subband * nrChannels) + channel);

						observation.setAverage((subband * nrChannels) + channel, aOld[(subband * nrChannels) + channel] + ((value - aOld[(subband * nrChannels) + channel]) / (element + 1)));
						observation.setVariance((subband * nrChannels) + channel, vOld[(subband * nrChannels) + channel] + ((value - aOld[(subband * nrChannels) + channel]) * (value - observation.getAverage((subband * nrChannels) + channel))));
					}
				}
			}
		}
	}
	rawFile.close();

	for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
		observation.setVariance(channel, observation.getVariance(channel) / (observation.getNrSeconds() * observation.getNrSamplesPerSecond()));
		observation.setStdDev(channel, sqrt(observation.getVariance(channel)));
	}

	delete [] word;
	delete [] aOld;
	delete [] vOld;
}

} // AstroData

#endif // READ_DATA_HPP

