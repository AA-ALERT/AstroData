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

#include <Observation.hpp>
#include <utils.hpp>


#ifndef READ_DATA_HPP
#define READ_DATA_HPP

namespace AstroData {

template< typename T > void readSIGPROC(Observation & observation, const unsigned int bytestoSkip, std::string inputFilename, std::vector< std::vector< T > * > & data, unsigned int firstSecond = 0);
template< typename T > void readLOFAR(std::string headerFilename, std::string rawFilename, Observation & observation, std::vector< std::vector< T > * > & data, unsigned int nrSeconds = 0, unsigned int firstSecond = 0);


// Implementation

template< typename T > void readSIGPROC(Observation & observation, unsigned int bytesToSkip, std::string inputFilename, std::vector< std::vector< T > * > & data, unsigned int firstSecond) {
	std::ifstream inputFile;
	const unsigned int BUFFER_DIM = 4;
	char * buffer = new char [BUFFER_DIM];

	inputFile.open(inputFilename.c_str(), std::ios::binary);
	inputFile.sync_with_stdio(false);
	inputFile.seekg(bytesToSkip, std::ios::beg);
	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		data.at(second) = new std::vector< T >(observation.getNrSamplesPerPaddedSecond() * observation.getNrChannels());

		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int channel = observation.getNrChannels(); channel > 0; channel-- ) {
				inputFile.read(buffer, BUFFER_DIM);
        data.at(second)->at((static_cast< long long unsigned int >(channel - 1) * observation.getNrSamplesPerPaddedSecond()) + sample) = *(reinterpret_cast< T * >(buffer));
			}
		}
	}
	inputFile.close();

	delete [] buffer;
}


template< typename T > void readLOFAR(std::string headerFilename, std::string rawFilename, Observation & observation, std::vector< std::vector< T > * > & data, unsigned int nrSeconds, unsigned int firstSecond) {
  unsigned int nrSubbands, nrChannels;
  float minFreq, channelBandwidth;
	// Read the HDF5 file with the metadata
	H5::H5File headerFile = H5::H5File(headerFilename, H5F_ACC_RDONLY);
	H5::FloatType typeDouble = H5::FloatType(H5::PredType::NATIVE_DOUBLE);
	double valueDouble = 0.0;
	H5::IntType typeUInt = H5::IntType(H5::PredType::NATIVE_UINT);
	unsigned int valueUInt = 0;

	H5::Group currentNode = headerFile.openGroup("/");
	currentNode.openAttribute("OBSERVATION_FREQUENCY_MIN").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
  minFreq = valueDouble;
	currentNode = currentNode.openGroup(currentNode.getObjnameByIdx(0));
	currentNode.openAttribute("TOTAL_INTEGRATION_TIME").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
	double totalIntegrationTime = valueDouble;
	currentNode.openAttribute("NOF_BEAMS").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	observation.setNrBeams(valueUInt);
	currentNode = currentNode.openGroup(currentNode.getObjnameByIdx(0));
	currentNode.openAttribute("NOF_SAMPLES").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	unsigned int totalSamples = valueUInt;
	currentNode.openAttribute("NOF_STATIONS").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	observation.setNrStations(valueUInt);
	currentNode.openAttribute("CHANNELS_PER_SUBBAND").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	nrChannels = valueUInt;
	currentNode.openAttribute("CHANNEL_WIDTH").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
  channelBandwidth = valueDouble / 1000000;
	H5::DataSet currentData = currentNode.openDataSet("STOKES_0");
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
  observation.setFrequencyRange(nrSubbands * nrChannels, minFreq, channelBandwidth);

	// Read the raw file with the actual data
	std::ifstream rawFile;
	rawFile.open(rawFilename.c_str(), std::ios::binary);
	rawFile.sync_with_stdio(false);
	if ( firstSecond > 0 ) {
		rawFile.seekg(firstSecond * observation.getNrSamplesPerSecond() * nrSubbands * nrChannels, std::ios::beg);
	}
	data.resize(observation.getNrSeconds());

	char * word = new char [4];
	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		data.at(second) = new std::vector< T >(observation.getNrChannels() * observation.getNrSamplesPerPaddedSecond());
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int subband = 0; subband < nrSubbands; subband++ ) {
				for ( unsigned int channel = 0; channel < nrChannels; channel++ ) {
					rawFile.read(word, 4);
          isa::utils::bigEndianToLittleEndian(word);
          data.at(second)->at((static_cast< long long unsigned int >(second) * observation.getNrSamplesPerSecond()) + sample) = *(reinterpret_cast< T * >(word));
				}
			}
		}
	}
	rawFile.close();
	delete [] word;
}

} // AstroData

#endif // READ_DATA_HPP

