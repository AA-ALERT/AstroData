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

#include <Observation.hpp>

namespace AstroData {

Observation::Observation() : padding(0), nrSeconds(0), nrStations(0), nrBeams(0), samplingRate(0.0f), nrSamplesPerSecond(0), nrSamplesPerDispersedChannel(0), nrChannels(0), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), nrDMs(0), firstDM(0.0f), lastDM(0.0f), DMStep(0.0f), nrPeriods(0), firstPeriod(0), lastPeriod(0), periodStep(0), nrBins(0) {}

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
}

void Observation::setNrSamplesPerDispersedChannel(const unsigned int samples) {
  nrSamplesPerDispersedChannel = isa::utils::pad(samples, padding);
}

void Observation::setFrequencyRange(const unsigned int channels, const float baseFrequency, const float bandwidth) {
  nrChannels = channels;
  channelBandwidth = bandwidth;
  minFreq = baseFrequency;
  maxFreq = baseFrequency + ((channels - 1) * bandwidth);
}

void Observation::setDMRange(const unsigned int dms, const float baseDM, const float step) {
	nrDMs = dms;
  DMStep = step;
  firstDM = baseDM;
  lastDM = baseDM + ((dms - 1) * step);
}

void Observation::setPeriodRange(const unsigned int periods, const unsigned int basePeriod, const unsigned int step) {
  nrPeriods = periods;
  periodStep = step;
  firstPeriod = basePeriod;
  lastPeriod = basePeriod + ((periods - 1) * step);
}

void Observation::setNrBins(const unsigned int bins) {
  nrBins = bins;
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

