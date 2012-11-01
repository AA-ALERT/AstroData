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

#include <GPUData.hpp>
#include <utils.hpp>
using isa::OpenCL::GPUData;
using isa::utils::toStringValue;


#ifndef READ_DATA_HPP
#define READ_DATA_HPP

namespace AstroData {

template< typename T > void readSIGPROC(unsigned int nrSeconds, unsigned int nrSamplesPerSecond, unsigned int nrChannels, unsigned int bytestoSkip, ifstream *inputFile, vector< GPUData< T > * > &data);
template< typename T > void readLOFAR(string headerFilename, string rawFilename, unsigned int &nrSeconds, unsigned int &nrSamplesPerSecond, unsigned int &paddedSecond, unsigned int &nrChannels, vector< GPUData< T > * > &data);


// Implementation

template< typename T > void readSIGPROC(unsigned int nrSeconds, unsigned int nrSamplesPerSecond, unsigned int nrChannels, unsigned int bytesToSkip, unsigned int *paddedSecond, string inputFilename, vector< GPUData< T > * > &data) {
	const unsigned int BUFFER_DIM = 4;
	*paddedSecond = nrSamplesPerSecond + (nrSamplesPerSecond % 4);
	char *buffer = new char [BUFFER_DIM];

	inputFile->sync_with_stdio(false);
	inputFile->ignore(bytesToSkip);
	for ( unsigned int second = 0; second < nrSeconds; second++ ) {
		data.at(second) = new GPUData< T >("second" + toStringValue< unsigned int >(second), true, true);
		(data.at(second))->allocateHostData(*paddedSecond * nrChannels);
		
		for ( unsigned int sample = 0; sample < nrSamplesPerSecond; sample++ ) {
			for ( unsigned int channel = nrChannels; channel > 0; channel-- ) {
				inputFile->read(buffer, BUFFER_DIM);
				((data.at(second))->getHostData())[((channel - 1) * (*paddedSecond)) + sample] = *(reinterpret_cast< T * >(buffer));
			}
		}
	}

	delete [] buffer;
}


template< typename T > void readLOFAR(string headerFilename, string rawFilename, unsigned int &nrSeconds, unsigned int &nrSamplesPerSecond, unsigned int &paddedSecond, unsigned int &nrChannels, vector< GPUData< T > * > &data) {
	unsigned int totalSamples = 0;
	unsigned int nrSubbands = 0;
	char *word = new char[4];
	double totalIntegrationTime = 0.0;

	// Read the HDF5 file with the metadata
	H5File headerFile = H5File(headerFilename, H5F_ACC_RDONLY);
	FloatType typeDouble = FloatType(H5::PredType::NATIVE_DOUBLE);
	IntType typeUInt = IntType(H5::PredType::NATIVE_UINT);

	Group currentNode = headerFile.openGroup("/");
	currentNode = currentNode.openGroup(currentNode.getObjnameByIdx(0));
	currentNode.openAttribute("TOTAL_INTEGRATION_TIME").read(typeDouble, reinterpret_cast< void * >(&totalIntegrationTime));
	currentNode = currentNode.openGroup(currentNode.getObjnameByIdx(0));
	currentNode.openAttribute("NOF_SAMPLES").read(typeUInt, reinterpret_cast< void * >(&totalSamples));
	currentNode.openAttribute("CHANNELS_PER_SUBBAND").read(typeUInt, reinterpret_cast< void * >(&nrChannels));
	DataSet currentData = currentNode.openDataSet("STOKES_0");
	currentData.openAttribute("NOF_SUBBANDS").read(typeUInt, reinterpret_cast< void * >(&nrSubbands));
	headerFile.close();
	
	nrSamplesPerSecond = static_cast< unsigned int >(totalSamples / totalIntegrationTime);
	nrSeconds = static_cast< unsigned int >(totalIntegrationTime + 1);
	paddedSecond = nrSamplesPerSecond + (nrSamplesPerSecond % 4);
		
	// Read the raw file with the actual data
	data.resize(nrSeconds);
	for ( unsigned int second = 0; second < nrSeconds; second++ ) {
		data.at(second) = new GPUData< T >("second" + toStringValue< unsigned int >(second), true, true);
		(data.at(second))->allocateHostData(nrSubbands * nrChannels * paddedSecond);
	}
	
	ifstream rawFile;
	rawFile.open(rawFilename.c_str(), ios::binary);
	rawFile.sync_with_stdio(false);
		
	for ( unsigned int subband = 0; subband < nrSubbands; subband++ ) {
		for ( unsigned int channel = 0; channel < nrChannels; channel++ ) {
			for ( unsigned int sample = 0; sample < totalSamples; sample++ ) {
				rawFile.read(word, 4);
				((data.at(sample / nrSamplesPerSecond))->getHostData())[(((subband * nrChannels) + channel) * paddedSecond) + (sample % nrSamplesPerSecond)] = *(reinterpret_cast< T * >(word));
			}
		}
	}
	rawFile.close();

	nrChannels *= nrSubbands;

	delete [] word;
}

} // AstroData

#endif // READ_DATA_HPP

