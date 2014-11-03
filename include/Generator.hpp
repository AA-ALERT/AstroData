// Copyright 2014 Alessio Sclocco <a.sclocco@vu.nl>
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

#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include <Observation.hpp>

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

namespace AstroData {

template< typename T > void generatePulsar(const unsigned int period, const unsigned int width, const float DM, const AstroData::Observation & observation, std::vector< std::vector< T > * > & data, const bool random = false);

// Implementations
template< typename T > void generatePulsar(const unsigned int period, const unsigned int width, const float DM, const AstroData::Observation & observation, std::vector< std::vector< T > * > & data, const bool random) {
  std::srand(std::time(0));
  // Generate the  "noise"
  for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
    data[second] = new std::vector< T >(observation.getNrChannels() * observation.getNrSamplesPerPaddedSecond());
    if ( random ) {
      for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
        for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + sample) = std::rand() % 100;
        }
      }
    } else {
      std::fill(data[second]->begin(), data[second]->end(), 1);
    }
  }
  // Generate the pulsar
	float inverseHighFreq = 1.0f / (observation.getMaxFreq() * observation.getMaxFreq());
  float kDM = 4148.808f * DM;
  for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
    float inverseFreq = 1.0f / ((observation.getMinFreq() + (channel * observation.getChannelBandwidth())) * (observation.getMinFreq() + (channel * observation.getChannelBandwidth())));
    float delta = kDM * (inverseFreq - inverseHighFreq);
    unsigned int shift = static_cast< unsigned int >(delta * observation.getNrSamplesPerSecond());

    for ( unsigned int sample = shift; sample < observation.getNrSeconds() * observation.getNrSamplesPerSecond(); sample += period ) {
      for ( unsigned int i = 0; i < width; i++ ) {
        if ( sample + i >= observation.getNrSeconds() * observation.getNrSamplesPerSecond() ) {
        break;
        }
        unsigned int second = (sample + i) / observation.getNrSamplesPerSecond();
        unsigned int internalSample = (sample + i) % observation.getNrSamplesPerSecond();

        if ( random ) {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + internalSample) = std::rand() % 10000;
        } else {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + internalSample) = 42;
        }
      }
    }
  }
}

} // AstroData

#endif // GENERATOR_HPP

