/*
 * Copyright (C) 2012
 * Alessio Sclocco <a.sclocco@vu.nl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <fstream>
#include <vector>
#include <string>
#include <H5Cpp.h>

using std::ios;
using std::string;
using std::ifstream;
using std::vector;
using H5::H5File;
using H5::Group;
using H5::DataSet;
using H5::FloatType;
using H5::IntType;

#include <Observation.hpp>
#include <GPUData.hpp>
#include <utils.hpp>

using isa::OpenCL::GPUData;
using isa::utils::toStringValue;
using isa::utils::changeEndianness;


#ifndef READ_DATA_HPP
#define READ_DATA_HPP

namespace AstroData {

template< typename T > void readSIGPROC(Observation &observation, unsigned int bytestoSkip, unsigned int *paddedSecond, ifstream *inputFile, vector< GPUData< T > * > &data);
template< typename T > void readLOFAR(string headerFilename, string rawFilename, Observation &observation, unsigned int *paddedSecond, T *minValue, T *maxValue, vector< GPUData< T > * > &data);


// Implementation

template< typename T > void readSIGPROC(Observation &observation, unsigned int bytesToSkip, unsigned int *paddedSecond, string inputFilename, vector< GPUData< T > * > &data) {
	ifstream inputFile;
	const unsigned int BUFFER_DIM = 4;
	*paddedSecond = observation.getNrSamplesPerSecond() + (observation.getNrSamplesPerSecond() % 4);
	char *buffer = new char [BUFFER_DIM];

	inputFile.open(inputFilename.c_str());
	inputFile.sync_with_stdio(false);
	inputFile.ignore(bytesToSkip);
	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		data.at(second) = new GPUData< T >("second" + toStringValue< unsigned int >(second), true, true);
		(data.at(second))->allocateHostData((*paddedSecond) * observation.getNrChannels());
		
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int channel = observation.getNrChannels(); channel > 0; channel-- ) {
				inputFile.read(buffer, BUFFER_DIM);
				((data.at(second))->getHostData())[((channel - 1) * (*paddedSecond)) + sample] = *(reinterpret_cast< T * >(buffer));
			}
		}
	}
	inputFile.close();

	delete [] buffer;
}


template< typename T > void readLOFAR(string headerFilename, string rawFilename, Observation &observation, unsigned int *paddedSecond, T *minValue, T *maxValue, vector< GPUData< T > * > &data) {
	unsigned int totalSamples = 0;
	unsigned int nrSubbands = 0;
	char *word = new char[4];
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
	observation.setNrChannels(valueUInt);
	currentNode.openAttribute("CHANNEL_WIDTH").read(typeDouble, reinterpret_cast< void * >(&valueDouble));
	observation.setChannelBandwidth(valueDouble / 1000000);
	DataSet currentData = currentNode.openDataSet("STOKES_0");
	currentData.openAttribute("NOF_SUBBANDS").read(typeUInt, reinterpret_cast< void * >(&valueUInt));
	nrSubbands = valueUInt;
	headerFile.close();
	
	observation.setNrSamplesPerSecond(static_cast< unsigned int >(totalSamples / totalIntegrationTime));
	observation.setSamplingRate(1.0f / observation.getNrSamplesPerSecond());
	observation.setNrSeconds(static_cast< unsigned int >(totalIntegrationTime));
	*paddedSecond = observation.getNrSamplesPerSecond() + (observation.getNrSamplesPerSecond() % 4);
		
	// Read the raw file with the actual data
	ifstream rawFile;
	rawFile.open(rawFilename.c_str(), ios::binary);
	rawFile.sync_with_stdio(false);
		
	data.resize(observation.getNrSeconds());
	
	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		data.at(second) = new GPUData< T >("second" + toStringValue< unsigned int >(second), true, true);
		(data.at(second))->allocateHostData(nrSubbands * observation.getNrChannels() * (*paddedSecond));
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int subband = 0; subband < nrSubbands; subband++ ) {
				for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
					T value = 0;

					rawFile.read(word, 4);
					changeEndianness(word);
					value = *(reinterpret_cast< T * >(word));

					((data.at(second))->getHostData())[(((subband * observation.getNrChannels()) + channel) * (*paddedSecond)) + sample] = value;
					if ( value <  *minValue ) {
						*minValue = value;
					}
					if ( value > *maxValue ) {
						*maxValue = value;
					}
				}
			}
		}
	}
	rawFile.close();

	observation.setNrChannels(observation.getNrChannels() * nrSubbands);

	delete [] word;
}

} // AstroData

#endif // READ_DATA_HPP

