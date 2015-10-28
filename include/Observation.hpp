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

#include <string>
#include <limits>

#include <utils.hpp>


#ifndef OBSERVATION_HPP
#define OBSERVATION_HPP

namespace AstroData {

class Observation {
public:
  Observation();
  ~Observation();

  // Getters
  // General observation parameters
  unsigned int getPadding() const;
  unsigned int getNrSeconds() const;
  unsigned int getNrStations() const;
  unsigned int getNrPaddedStations() const;
  unsigned int getNrBeams() const;
  unsigned int getNrPaddedBeams() const;
  float getSamplingRate() const;
  unsigned int getNrSamplesPerSecond() const;
  unsigned int getNrSamplesPerPaddedSecond() const;
  unsigned int getNrSamplesPerDispersedChannel() const;
  unsigned int getNrSamplesPerPaddedDispersedChannel() const;
	// Frequency parameters
  unsigned int getNrChannels() const;
  unsigned int getNrPaddedChannels() const;
  float getMinFreq() const;
  float getMaxFreq() const;
  float getChannelBandwidth() const;
	// Dispersion measures
  unsigned int getNrDelaySeconds() const;
  unsigned int getNrDMs() const;
  unsigned int getNrPaddedDMs() const;
  float getFirstDM() const;
  float getLastDM() const;
  float getDMStep() const;
	// Periods
  unsigned int getNrPeriods() const;
  unsigned int getNrPaddedPeriods() const;
  unsigned int getFirstPeriod() const;
  unsigned int getLastPeriod() const;
  unsigned int getPeriodStep() const;
  unsigned int getNrBins() const;
  unsigned int getNrPaddedBins() const;

  // Setters
  // General observation parameters
  void setPadding(const unsigned int pad);
  void setNrSeconds(const unsigned int seconds);
  void setNrStations(const unsigned int stations);
  void setNrBeams(const unsigned int beams);
  void setNrSamplesPerSecond(const unsigned int samples);
  void setNrSamplesPerDispersedChannel(const unsigned int samples);
  // Frequency parameters
  void setFrequencyRange(const unsigned int channels, const float baseFrequency, const float bandwidth);
  // Dispersion measures
  void setNrDelaySeconds(const unsigned int seconds);
  void setDMRange(const unsigned int dms, const float baseDM, const float step);
  // Periods
  void setPeriodRange(const unsigned int periods, const unsigned int basePeriod, const unsigned int step);
  void setNrBins(const unsigned int bins);

private:
  unsigned int padding;
  unsigned int nrSeconds;
  unsigned int nrStations;
  unsigned int nrBeams;
  float samplingRate;
  unsigned int nrSamplesPerSecond;
  unsigned int nrSamplesPerDispersedChannel;

  unsigned int nrChannels;
  float minFreq;
  float maxFreq;
  float channelBandwidth;

  unsigned int nrDelaySeconds;
  unsigned int nrDMs;
  float firstDM;
  float lastDM;
  float DMStep;

  unsigned int nrPeriods;
  unsigned int firstPeriod;
  unsigned int lastPeriod;
  unsigned int periodStep;
  unsigned int nrBins;
};

// Implementations

inline void Observation::setPadding(const unsigned int pad) {
	padding = pad;
}

inline void Observation::setNrSeconds(const unsigned int seconds) {
	nrSeconds = seconds;
}

inline void Observation::setNrStations(const unsigned int stations) {
	nrStations = stations;
}

inline void Observation::setNrBeams(const unsigned int beams) {
	nrBeams = beams;
}

inline void Observation::setNrDelaySeconds(const unsigned int seconds) {
  nrDelaySeconds = seconds;
}

inline unsigned int Observation::getPadding() const {
	return padding;
}

inline unsigned int Observation::getNrSeconds() const {
	return nrSeconds;
}

inline unsigned int Observation::getNrStations() const {
	return nrStations;
}

inline unsigned int Observation::getNrPaddedStations() const {
	return isa::utils::pad(nrStations, padding);
}

inline unsigned int Observation::getNrBeams() const {
	return nrBeams;
}

inline unsigned int Observation::getNrPaddedBeams() const {
	return isa::utils::pad(nrBeams, padding);
}

inline unsigned int Observation::getNrSamplesPerSecond() const {
	return nrSamplesPerSecond;
}

inline float Observation::getSamplingRate() const {
	return samplingRate;
}

inline unsigned int Observation::getNrSamplesPerPaddedSecond() const {
	return isa::utils::pad(nrSamplesPerSecond, padding);
}

inline unsigned int Observation::getNrChannels() const {
	return nrChannels;
}

inline unsigned int Observation::getNrPaddedChannels() const {
	return isa::utils::pad(nrChannels, padding);
}

inline unsigned int Observation::getNrSamplesPerPaddedDispersedChannel() const {
  return isa::utils::pad(nrSamplesPerDispersedChannel, padding);
}

inline float Observation::getMinFreq() const {
	return minFreq;
}

inline float Observation::getMaxFreq() const {
	return maxFreq;
}

inline float Observation::getChannelBandwidth() const {
	return channelBandwidth;
}

inline unsigned int Observation::getNrSamplesPerDispersedChannel() const {
  return nrSamplesPerDispersedChannel;
}

inline unsigned int Observation::getNrDelaySeconds() const {
  return nrDelaySeconds;
}

inline unsigned int Observation::getNrDMs() const {
	return nrDMs;
}

inline unsigned int Observation::getNrPaddedDMs() const {
	return isa::utils::pad(nrDMs, padding);
}

inline float Observation::getFirstDM() const {
	return firstDM;
}

inline float Observation::getLastDM() const {
  return lastDM;
}

inline float Observation::getDMStep() const {
	return DMStep;
}

inline unsigned int Observation::getNrPeriods() const {
	return nrPeriods;
}

inline unsigned int Observation::getNrPaddedPeriods() const {
	return isa::utils::pad(nrPeriods, padding);
}

inline unsigned int Observation::getNrBins() const {
	return nrBins;
}

inline unsigned int Observation::getNrPaddedBins() const {
	return isa::utils::pad(nrBins, padding);
}

inline unsigned int Observation::getFirstPeriod() const {
	return firstPeriod;
}

inline unsigned int Observation::getLastPeriod() const {
  return lastPeriod;
}

inline unsigned int Observation::getPeriodStep() const {
	return periodStep;
}

} // AstroData

#endif // OBSERVATION_HPP

