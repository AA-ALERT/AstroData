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
#include <cmath>
#include <algorithm>

#include <Observation.hpp>

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

namespace AstroData {

template< typename T > void generatePulsar(const unsigned int period, const unsigned int width, const float DM, const AstroData::Observation & observation, std::vector< std::vector< T > * > & data, const bool random = false);
template< typename T > void generateSinglePulse(const unsigned int width, const float DM, const AstroData::Observation & observation, std::vector< std::vector< T > * > & data, const uint8_t inputBits, const bool random = false);

// Implementations
template< typename T > void generatePulsar(const unsigned int period, const unsigned int width, const float DM, const AstroData::Observation & observation, std::vector< std::vector< T > * > & data, const bool random) {
  std::srand(std::time(0));
  // Generate the  "noise"
  for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
    data[second] = new std::vector< T >(observation.getNrChannels() * observation.getNrSamplesPerPaddedSecond());
    if ( random ) {
      for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
        for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + sample) = static_cast< T >(std::rand() % 25);
        }
      }
    } else {
      std::fill(data[second]->begin(), data[second]->end(), static_cast< T >(8));
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
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + internalSample) = static_cast< T >(std::rand() % 128);
        } else {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + internalSample) = static_cast< T >(42);
        }
      }
    }
  }
}

template< typename T > void generateSinglePulse(const unsigned int width, const float DM, const AstroData::Observation & observation, std::vector< std::vector< T > * > & data, const uint8_t inputBits, const bool random) {
  std::srand(std::time(0));
  // Generate the  "noise"
  for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
    if ( inputBits >= 8 ) {
      data[second] = new std::vector< T >(observation.getNrChannels() * observation.getNrSamplesPerPaddedSecond());
    } else {
      data[second] = new std::vector< T >(observation.getNrChannels() * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding()));
    }
    if ( random ) {
      for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
        for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
          if ( inputBits >= 8 ) {
            data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + sample) = static_cast< T >(std::rand() % 25);
          } else {
            uint8_t value = static_cast< unsigned int >(std::rand() % (inputBits - 1));
            char buffer = data[second]->at((channel * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding())) + (sample / (8 / inputBits)));

            for ( uint8_t bit = 0; bit < inputBits; bit++ ) {
              isa::utils::setBit(buffer, isa::utils::getBit(value, bit), (sample % (8 / inputBits)) + bit);
            }
            data[second]->at((channel * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding())) + sample) = buffer;
          }
        }
      }
    } else {
      if ( inputBits >= 8 ) {
        std::fill(data[second]->begin(), data[second]->end(), static_cast< T >(8));
      } else {
        std::fill(data[second]->begin(), data[second]->end(), static_cast< T >(0));
      }
    }
  }
  // Generate the pulse
  unsigned int second = 0;
  unsigned int sample = 0;
	float inverseHighFreq = 1.0f / std::pow(observation.getMaxFreq(), 2.0f);
  float kDM = 4148.808f * DM;

  if ( random ) {
    second = std::rand() % observation.getNrSeconds();
    sample = std::rand() % (observation.getNrSamplesPerSecond() - width);
  } else {
    second = observation.getNrSeconds() / 2;
    sample = observation.getNrSamplesPerSecond() / 2;
  }

  for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
    float inverseFreq = 1.0f / std::pow(observation.getMinFreq() + (channel * observation.getChannelBandwidth()), 2.0f);
    unsigned int shift = static_cast< unsigned int >(kDM * (inverseFreq - inverseHighFreq) * observation.getNrSamplesPerSecond());

    for ( unsigned int i = 0; i < width; i++ ) {
      if ( sample + i >= observation.getNrSamplesPerSecond() ) {
      break;
      }

      if ( random ) {
        if ( inputBits >= 8 ) {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + (sample + i + shift)) = static_cast< T >(std::rand() % 256);
        } else {
          uint8_t value = static_cast< unsigned int >(std::rand() % inputBits);
          char buffer = data[second]->at((channel * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding())) + ((sample + i + shift) / (8 / inputBits)));

          for ( uint8_t bit = 0; bit < inputBits; bit++ ) {
            isa::utils::setBit(buffer, isa::utils::getBit(value, bit), ((sample + i + shift) % (8 / inputBits)) + bit);
          }
          data[second]->at((channel * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding())) + ((sample + i + shift) / (8 / inputBits))) = buffer;
        }
      } else {
        if ( inputBits >= 8 ) {
          data[second]->at((channel * observation.getNrSamplesPerPaddedSecond()) + (sample + i + shift)) = static_cast< T >(42);
        } else {
          unsigned int byte = (sample + i + shift) / (8 / inputBits);
          uint8_t firstBit = 0;
          unsigned char buffer = 0;

          if ( (sample + i + shift) % (8 / inputBits) != 0 ) {
            firstBit = ((sample + i + shift) % (8 / inputBits)) * inputBits;
          }
          buffer = data[second]->at((channel * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding())) + byte);

          for ( uint8_t bit = 0; bit < inputBits; bit++ ) {
            isa::utils::setBit(buffer, isa::utils::getBit(inputBits, bit), firstBit + bit);
          }
          data[second]->at((channel * isa::utils::pad(observation.getNrSamplesPerSecond() / (8 / inputBits), observation.getPadding())) + byte) = buffer;
        }
      }
    }
  }
}

} // AstroData

#endif // GENERATOR_HPP

