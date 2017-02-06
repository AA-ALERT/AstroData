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
#include <set>
#include <string>
#include <cmath>
#include <exception>
#include <H5Cpp.h>
#include <dada_hdu.h>
#include <ascii_header.h>

#include <Observation.hpp>
#include <utils.hpp>


#ifndef READ_DATA_HPP
#define READ_DATA_HPP

namespace AstroData {

// Exception: the PSRDada ring buffer does not work
class RingBufferError : public std::exception {
public:
  RingBufferError();
  ~RingBufferError() throw ();

  const char * what() const throw ();
};

// Zapped channels (excluded from computation)
void readZappedChannels(Observation & observation, const std::string & inputFileName, std::vector< uint8_t > & zappedChannels);
// Integration steps
void readIntegrationSteps(const Observation & observation, const std::string  & inputFileName, std::set< unsigned int > & integrationSteps);
// SIGPROC data
template< typename T > void readSIGPROC(const Observation & observation, const unsigned int padding, const uint8_t inputBits, const unsigned int bytesToSkip, const std::string & inputFilename, std::vector< std::vector< T > * > & data, const unsigned int firstBatch = 0);
// LOFAR data
template< typename T > void readLOFAR(std::string headerFilename, std::string rawFilename, Observation & observation, const unsigned int padding, std::vector< std::vector< T > * > & data, unsigned int nrBatches = 0, unsigned int firstBatch = 0);
// PSRDADA buffer
template< typename T > void readPSRDadaHeader(Observation & observation, dada_hdu_t & ringBuffer) throw(RingBufferError);
template< typename T > inline void readPSRDada(Observation & observation, const unsigned int padding, dada_hdu_t & ringBuffer, std::vector< T > * data) throw(RingBufferError);


// Implementations

template< typename T > void readSIGPROC(const Observation & observation, const unsigned int padding, const uint8_t inputBits, const unsigned int bytesToSkip, const std::string & inputFilename, std::vector< std::vector< T > * > & data, const unsigned int firstBatch = 0) {
	std::ifstream inputFile;
	const unsigned int BUFFER_DIM = sizeof(T);
	char * buffer = new char [BUFFER_DIM];

	inputFile.open(inputFilename.c_str(), std::ios::binary);
	inputFile.sync_with_stdio(false);
	inputFile.seekg(bytesToSkip, std::ios::beg);
	for ( unsigned int batch = 0; batch < observation.getNrBatches(); batch++ ) {
    if ( inputBits >= 8 ) {
      data.at(batch) = new std::vector< T >(observation.getNrChannels() * observation.getNrSamplesPerPaddedBatch(padding / sizeof(T)));
      for ( unsigned int sample = 0; sample < observation.getNrSamplesPerBatch(); sample++ ) {
        for ( unsigned int channel = observation.getNrChannels(); channel > 0; channel-- ) {
          inputFile.read(buffer, BUFFER_DIM);
          data.at(batch)->at((static_cast< uint64_t >(channel - 1) * observation.getNrSamplesPerPaddedBatch(padding / sizeof(T))) + sample) = *(reinterpret_cast< T * >(buffer));
        }
      }
    } else {
      uint64_t bytesToRead = static_cast< uint64_t >(observation.getNrSamplesPerBatch() * (observation.getNrChannels() / (8.0 / inputBits)));

      data.at(batch) = new std::vector< T >(observation.getNrChannels() * isa::utils::pad(observation.getNrSamplesPerBatch() / (8 / inputBits), padding / sizeof(T)));
      for ( uint64_t byte = 0; byte < bytesToRead; byte++ ) {
        unsigned int channel = (observation.getNrChannels() - 1) - ((byte * (8 / inputBits)) % observation.getNrChannels());
        unsigned int sample = (byte * (8 / inputBits)) / observation.getNrChannels();
        unsigned int sampleByte = sample / (8 / inputBits);
        uint8_t sampleFirstBit = (sample % (8 / inputBits)) * inputBits;

        inputFile.read(buffer, BUFFER_DIM);
        for ( unsigned int item = 0; item < 8 / inputBits; item++ ) {
          uint8_t channelFirstBit = item * inputBits;
          uint8_t sampleBuffer = 0;

          if ( item > channel ) {
            // All channels read, the remaining elements are from the next sample
            unsigned int channelOffset = 0;
            channel = (observation.getNrChannels() - 1);
            sample += 1;
            sampleByte = sample / (8 / inputBits);
            sampleFirstBit = (sample % (8 / inputBits)) * inputBits;

            while ( item < (8 / inputBits) ) {
              channelFirstBit = item * inputBits;
              sampleBuffer = data.at(batch)->at((static_cast< uint64_t >(channel - channelOffset) * isa::utils::pad(observation.getNrSamplesPerBatch() / (8 / inputBits), padding / sizeof(T))) + sampleByte);
              for ( uint8_t bit = 0; bit < inputBits; bit++ ) {
                isa::utils::setBit(sampleBuffer, isa::utils::getBit(*buffer, channelFirstBit + bit), sampleFirstBit + bit);
              }
              data.at(batch)->at((static_cast< uint64_t >(channel - channelOffset) * isa::utils::pad(observation.getNrSamplesPerBatch() / (8 / inputBits), padding / sizeof(T))) + sampleByte) = sampleBuffer;
              item++;
              channelOffset++;
            }

            break;
          }
          sampleBuffer = data.at(batch)->at((static_cast< uint64_t >(channel - item) * isa::utils::pad(observation.getNrSamplesPerBatch() / (8 / inputBits), padding / sizeof(T))) + sampleByte);
          for ( uint8_t bit = 0; bit < inputBits; bit++ ) {
            isa::utils::setBit(sampleBuffer, isa::utils::getBit(*buffer, channelFirstBit + bit), sampleFirstBit + bit);
          }
          data.at(batch)->at((static_cast< uint64_t >(channel - item) * isa::utils::pad(observation.getNrSamplesPerBatch() / (8 / inputBits), padding / sizeof(T))) + sampleByte) = sampleBuffer;
        }
      }
    }
	}
	inputFile.close();

	delete [] buffer;
}

template< typename T > void readLOFAR(std::string headerFilename, std::string rawFilename, Observation & observation, const unsigned int padding, std::vector< std::vector< T > * > & data, unsigned int nrBatches, unsigned int firstBatch) {
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

	observation.setNrSamplesPerBatch(static_cast< unsigned int >(totalSamples / totalIntegrationTime));
	if ( nrBatches == 0 ) {
		observation.setNrBatches(static_cast< unsigned int >(totalIntegrationTime));
	} else {
		if ( static_cast< unsigned int >(totalIntegrationTime) >= (firstBatch + nrBatches) ) {
			observation.setNrBatches(nrBatches);
		} else {
			observation.setNrBatches(static_cast< unsigned int >(totalIntegrationTime) - firstBatch);
		}
	}
  observation.setFrequencyRange(1, nrSubbands * nrChannels, minFreq, channelBandwidth);

	// Read the raw file with the actual data
	std::ifstream rawFile;
	rawFile.open(rawFilename.c_str(), std::ios::binary);
	rawFile.sync_with_stdio(false);
	if ( firstBatch > 0 ) {
		rawFile.seekg(firstBatch * observation.getNrSamplesPerBatch() * nrSubbands * nrChannels, std::ios::beg);
	}
	data.resize(observation.getNrBatches());

	char * word = new char [4];
	for ( unsigned int batch = 0; batch < observation.getNrBatches(); batch++ ) {
		data.at(batch) = new std::vector< T >(observation.getNrChannels() * observation.getNrSamplesPerPaddedBatch(padding / sizeof(T)));
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerBatch(); sample++ ) {
			for ( unsigned int subband = 0; subband < nrSubbands; subband++ ) {
				for ( unsigned int channel = 0; channel < nrChannels; channel++ ) {
          unsigned int globalChannel = (subband * nrChannels) + channel;

					rawFile.read(word, 4);
          isa::utils::bigEndianToLittleEndian(word);
          data.at(batch)->at((globalChannel * observation.getNrSamplesPerPaddedBatch(padding / sizeof(T))) + sample) = *(reinterpret_cast< T * >(word));
				}
			}
		}
	}
	rawFile.close();
	delete [] word;
}

template< typename T > void readPSRDadaHeader(Observation & observation, dada_hdu_t & ringBuffer) throw(RingBufferError) {
  // Staging variables for the header elements
  unsigned int uintValue = 0;
  float floatValue[2] = {0.0f, 0.0f};
  // Header string
  uint64_t headerBytes = 0;
  char * header = 0;

  header = ipcbuf_get_next_read(ringBuffer.header_block, &headerBytes);

  if ( (header == 0) || (headerBytes == 0 ) ) {
    throw RingBufferError();
  }

  ascii_header_get(header, "SAMPLES_PER_BATCH", "%d", &uintValue);
  observation.setNrSamplesPerBatch(uintValue);
  ascii_header_get(header, "CHANNELS", "%d", &uintValue);
  ascii_header_get(header, "MIN_FREQUENCY", "%f", &floatValue[0]);
  ascii_header_get(header, "CHANNEL_BANDWIDTH", "%f", &floatValue[1]);
  observation.setFrequencyRange(1, uintValue, floatValue[0], floatValue[1]);
  ipcbuf_mark_cleared(reinterpret_cast< ipcbuf_t * >(ringBuffer.header_block));

  delete header;
}

template< typename T > inline void readPSRDada(const unsigned int padding, dada_hdu_t & ringBuffer, std::vector< T > * data, const unsigned int dataSize) throw(RingBufferError) {
  uint8_t * buffer = 0;
  uint64_t bufferSize = 0;

  buffer = ipcbuf_get_next_read(reinterpret_cast< ipcbuf_t * >(&ringBuffer), &bufferSize);
  if ( bufferSize < dataSize * sizeof(T) ) {
    throw RingBufferError();
  }
  memcpy(reinterpret_cast< void * >(data->data()), reinterpret_cast< const void * >(buffer), dataSize * sizeof(T));
  if ( (ipcbuf_mark_cleeared(reinterpret_cast< ipcbuf_t * >(&ringBuffer))) < 0 ) {
    throw RingBufferError();
  }
}

} // AstroData

#endif // READ_DATA_HPP

