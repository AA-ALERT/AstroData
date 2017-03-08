// Copyright 2015 Alessio Sclocco <a.sclocco@vu.nl>
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

#include <ReadData.hpp>

namespace AstroData {

RingBufferError::RingBufferError(std::string message) : message(message) {}

RingBufferError::~RingBufferError() throw () {}

const char * RingBufferError::what() const throw() {
    return message.c_str();
}

void readZappedChannels(Observation & observation, const std::string & inputFileName, std::vector< uint8_t > & zappedChannels) {
  unsigned int nrChannels = 0;
  std::ifstream input;

  input.open(inputFileName);
  while ( !input.eof() ) {
    unsigned int channel = observation.getNrChannels();

    input >> channel;
    if ( channel < observation.getNrChannels() ) {
      zappedChannels[channel] = 1;
      nrChannels++;
    }
  }
  input.close();
  observation.setNrZappedChannels(nrChannels);
}

void readIntegrationSteps(const Observation & observation, const std::string  & inputFileName, std::set< unsigned int > & integrationSteps) {
  std::ifstream input;

  input.open(inputFileName);
  while ( !input.eof() ) {
    unsigned int step = observation.getNrSamplesPerBatch();

    input >> step;
    if ( step < observation.getNrSamplesPerBatch() ) {
      integrationSteps.insert(step);
    }
  }
  input.close();
}

#ifdef HAVE_PSRDADA
void readPSRDADAHeader(Observation & observation, dada_hdu_t & ringBuffer) throw(RingBufferError) {
  // Staging variables for the header elements
  unsigned int uintValue = 0;
  float floatValue[2] = {0.0f, 0.0f};
  // Header string
  uint64_t headerBytes = 0;
  char * header = 0;

  header = ipcbuf_get_next_read(ringBuffer.header_block, &headerBytes);
  if ( (header == 0) || (headerBytes == 0 ) ) {
    throw RingBufferError("Impossible to read the PSRDADA header.");
  }
  ascii_header_get(header, "SAMPLES_PER_BATCH", "%d", &uintValue);
  observation.setNrSamplesPerBatch(uintValue);
  ascii_header_get(header, "CHANNELS", "%d", &uintValue);
  ascii_header_get(header, "MIN_FREQUENCY", "%f", &floatValue[0]);
  ascii_header_get(header, "CHANNEL_BANDWIDTH", "%f", &floatValue[1]);
  observation.setFrequencyRange(1, uintValue, floatValue[0], floatValue[1]);
  if ( ipcbuf_mark_cleared(ringBuffer.header_block) < 0 ) {
    throw RingBufferError("Impossible to mark the PSRDADA header as cleared.");
  }
}
#endif
} // AstroData

