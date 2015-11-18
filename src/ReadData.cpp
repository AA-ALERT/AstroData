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

RingBufferError::RingBufferError() {}

RingBufferError::~RingBufferError() throw () {}

const char * RingBufferError::what() const throw() {
    return ("Impossible to read from the PSRDada ring buffer.");
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

void readIntegrationSteps(const Observation & observation, const std::string  & inputFileName, std::list< unsigned int > & integrationSteps) {
  std::ifstream input;

  input.open(inputFileName);
  while ( !input.eof() ) {
    unsigned int step = observation.getNrSamplesPerSecond();

    input >> step;
    if ( step < observation.getNrSamplesPerSecond() ) {
      set.insert(step);
    }
  }
  input.close()
}

} // AstroData

