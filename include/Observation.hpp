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
  inline unsigned int getPadding() const;
  inline unsigned int getNrSeconds() const;
  inline unsigned int getNrStations() const;
  inline unsigned int getNrBeams() const;
  inline unsigned int getNrPaddedBeams() const;
  inline float getSamplingRate() const;
  inline unsigned int getNrSamplesPerSecond() const;
  inline unsigned int getNrSamplesPerPaddedSecond() const;
  inline unsigned int getNrSamplesPerDispersedChannel() const;

	// Frequency parameters
  inline unsigned int getNrChannels() const;
  inline unsigned int getNrPaddedChannels() const;
  inline float getMinFreq() const;
  inline float getMaxFreq() const;
  inline float getChannelBandwidth() const;

	// Dispersion measures
  inline unsigned int getNrDMs() const;
  inline unsigned int getNrPaddedDMs() const;
  inline float getFirstDM() const;
  inline float getLastDM() const;
  inline float getDMStep() const;

	// Periods
  inline unsigned int getNrPeriods() const;
  inline unsigned int getNrPaddedPeriods() const;
  inline unsigned int getFirstPeriod() const;
  inline unsigned int getLastPeriod() const;
  inline unsigned int getPeriodStep() const;
  inline unsigned int getNrBins() const;
  inline unsigned int getNrPaddedBins() const;

  // Setters
  // General observation parameters
  inline void setPadding(const unsigned int pad);
  inline void setNrSeconds(const unsigned int seconds);
  inline void setNrStations(const unsigned int stations);
  inline void setNrBeams(const unsigned int beams);
  void setNrSamplesPerSecond(const unsigned int samples);
  void setNrSamplesPerDispersedChannel(const unsigned int samples);

  // Frequency parameters
  void setFrequencyRange(const unsigned int channels, const float baseFrequency, const float bandwidth);

  // Dispersion measures
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
  unsigned int nrSamplesPerPaddedSecond;
  unsigned int nrSamplesPerDispersedChannel;

  unsigned int nrChannels;
  unsigned int nrPaddedChannels;
  float minFreq;
  float maxFreq;
  float channelBandwidth;

  unsigned int nrDMs;
  unsigned int nrPaddedDMs;
  float firstDM;
  float lastDM;
  float DMStep;

  unsigned int nrPeriods;
  unsigned int nrPaddedPeriods;
  unsigned int firstPeriod;
  unsigned int lastPeriod;
  unsigned int periodStep;
  unsigned int nrBins;
  unsigned int nrPaddedBins;
};


// Implementation

Observation::Observation() : padding(0), nrSeconds(0), nrStations(0), nrBeams(0), samplingRate(0.0f), nrSamplesPerSecond(0), nrSamplesPerPaddedSecond(0), nrSamplesPerDispersedChannel(0), nrChannels(0), nrPaddedChannels(0), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), nrDMs(0), nrPaddedDMs(0), firstDM(0.0f), lastDM(0.0f), DMStep(0.0f), nrPeriods(0), nrPaddedPeriods(0), firstPeriod(0), lastPeriod(0), periodStep(0), nrBins(0), nrPaddedBins(0) {}

Observation::~Observation() {}

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

void Observation::setNrSamplesPerSecond(const unsigned int samples) {
	samplingRate = 1.0f / samples;
	nrSamplesPerSecond = samples;
  nrSamplesPerPaddedSecond = isa::utils::pad(samples, padding);
}

void Observation::setNrSamplesPerDispersedChannel(const unsigned int samples) {
  nrSamplesPerDispersedChannel = isa::utils::pad(samples, padding);
}

void Observation::setFrequencyRange(const unsigned int channels, const float baseFrequency, const float bandwidth) {
  nrChannels = channels;
  nrPaddedChannels = isa::utils::pad(channels, padding);
  channelBandwidth = bandwidth;
  minFreq = baseFrequency;
  maxFreq = baseFrequency + ((channels - 1) * bandwidth);
}

void Observation::setDMRange(const unsigned int dms, const float baseDM, const float step) {
	nrDMs = dms;
  nrPaddedDMs = isa::utils::pad(dms, padding);
  DMStep = step;
  firstDM = baseDM;
  lastDM = baseDM + ((dms - 1) * step);
}

void Observation::setPeriodRange(const unsigned int periods, const unsigned int basePeriod, const unsigned int step) {
  nrPeriods = periods;
  nrPaddedPeriods = isa::utils::pad(periods, padding);
  periodStep = step;
  firstPeriod = basePeriod;
  lastPeriod = basePeriod + ((periods - 1) * step);
}

void Observation::setNrBins(const unsigned int bins) {
  nrBins = bins;
  nrPaddedBins = isa::utils::pad(bins, padding);
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
	return nrSamplesPerPaddedSecond;
}

inline unsigned int Observation::getNrChannels() const {
	return nrChannels;
}

inline unsigned int Observation::getNrPaddedChannels() const {
	return nrPaddedChannels;
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

inline unsigned int Observation::getNrDMs() const {
	return nrDMs;
}

inline unsigned int Observation::getNrPaddedDMs() const {
	return nrPaddedDMs;
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
	return nrPaddedPeriods;
}

inline unsigned int Observation::getNrBins() const {
	return nrBins;
}

inline unsigned int Observation::getNrPaddedBins() const {
	return nrPaddedBins;
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

