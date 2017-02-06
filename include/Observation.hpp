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
  unsigned int getNrBatches() const;
  unsigned int getNrStations() const;
  unsigned int getNrPaddedStations(unsigned int padding) const;
  unsigned int getNrBeams() const;
  unsigned int getNrPaddedBeams(unsigned int padding) const;
  unsigned int getNrSynthesizedBeams() const;
  unsigned int getNrPaddedSynthesizedBeams(const unsigned int padding) const;
  float getSamplingRate() const;
  unsigned int getNrSamplesPerBatch() const;
  unsigned int getNrSamplesPerPaddedBatch(unsigned int padding) const;
  // Frequency parameters
  unsigned int getNrSubbands() const;
  unsigned int getNrChannels() const;
  unsigned int getNrChannelsPerSubband() const;
  unsigned int getNrPaddedSubbands(unsigned int padding) const;
  unsigned int getNrPaddedChannels(unsigned int padding) const;
  unsigned int getNrZappedChannels() const;
  float getSubbandMinFreq() const;
  float getSubbandMaxFreq() const;
  float getSubbandBandwidth() const;
  float getMinFreq() const;
  float getMaxFreq() const;
  float getChannelBandwidth() const;
  // Dispersion measures
  unsigned int getNrSamplesPerBatchSubbanding() const;
  unsigned int getNrSamplesPerPaddedBatchSubbanding(unsigned int padding) const;
  unsigned int getNrDelayBatchesSubbanding() const;
  unsigned int getNrDelayBatches() const;
  unsigned int getNrDMsSubbanding() const;
  unsigned int getNrDMs() const;
  unsigned int getNrPaddedDMsSubbanding(unsigned int padding) const;
  unsigned int getNrPaddedDMs(unsigned int padding) const;
  unsigned int getNrSamplesPerSubbandingDispersedChannel() const;
  unsigned int getNrSamplesPerDispersedChannel() const;
  unsigned int getNrSamplesPerPaddedSubbandingDispersedChannel(unsigned int padding) const;
  unsigned int getNrSamplesPerPaddedDispersedChannel(unsigned int padding) const;
  float getFirstDMSubbanding() const;
  float getFirstDM() const;
  float getLastDMSubbanding() const;
  float getLastDM() const;
  float getDMSubbandingStep() const;
  float getDMStep() const;
  // Periods
  unsigned int getNrPeriods() const;
  unsigned int getNrPaddedPeriods(unsigned int padding) const;
  unsigned int getFirstPeriod() const;
  unsigned int getLastPeriod() const;
  unsigned int getPeriodStep() const;
  unsigned int getNrBins() const;
  unsigned int getNrPaddedBins(unsigned int padding) const;

  // Setters
  // General observation parameters
  void setNrBatches(const unsigned int batches);
  void setNrStations(const unsigned int stations);
  void setNrBeams(const unsigned int beams);
  void setNrSynthesizedBeams(const unsigned int beams);
  void setSamplingRate(const float sampling);
  void setNrSamplesPerBatch(const unsigned int samples);
  // Frequency parameters
  void setFrequencyRange(const unsigned int subbands, const unsigned int channels, const float baseFrequency, const float bandwidth);
  void setNrZappedChannels(const unsigned int zappedChannels);
  // Dispersion measures
  void setNrSamplesPerBatchSubbanding(const unsigned int samples);
  void setNrDelayBatchesSubbanding(const unsigned int batches);
  void setNrDelayBatches(const unsigned int batches);
  void setNrSamplesPerSubbandingDispersedChannel(const unsigned int samples);
  void setNrSamplesPerDispersedChannel(const unsigned int samples);
  void setDMSubbandingRange(const unsigned int dms, const float baseDM, const float step);
  void setDMRange(const unsigned int dms, const float baseDM, const float step);
  // Periods
  void setPeriodRange(const unsigned int periods, const unsigned int basePeriod, const unsigned int step);
  void setNrBins(const unsigned int bins);

private:
  unsigned int nrBatches;
  unsigned int nrStations;
  unsigned int nrBeams;
  unsigned int nrSynthesizedBeams;
  float samplingRate;
  unsigned int nrSamplesPerBatchSubbanding;
  unsigned int nrSamplesPerBatch;
  unsigned int nrSamplesPerSubbandingDispersedChannel;
  unsigned int nrSamplesPerDispersedChannel;

  unsigned int nrSubbands;
  unsigned int nrChannels;
  unsigned int nrChannelsPerSubband;
  unsigned int nrZappedChannels;
  float subbandMinFreq;
  float subbandMaxFreq;
  float subbandBandwidth;
  float minFreq;
  float maxFreq;
  float channelBandwidth;

  unsigned int nrDelayBatchesSubbanding;
  unsigned int nrDelayBatches;
  unsigned int nrDMsSubbanding;
  unsigned int nrDMs;
  float firstDMSubbanding;
  float firstDM;
  float lastDMSubbanding;
  float lastDM;
  float DMSubbandingStep;
  float DMStep;

  unsigned int nrPeriods;
  unsigned int firstPeriod;
  unsigned int lastPeriod;
  unsigned int periodStep;
  unsigned int nrBins;
};

// Implementations
inline unsigned int Observation::getNrBatches() const {
  return nrBatches;
}

inline unsigned int Observation::getNrStations() const {
  return nrStations;
}

inline unsigned int Observation::getNrPaddedStations(unsigned int padding) const {
  return isa::utils::pad(nrStations, padding);
}

inline unsigned int Observation::getNrBeams() const {
  return nrBeams;
}

inline unsigned int Observation::getNrPaddedBeams(unsigned int padding) const {
  return isa::utils::pad(nrBeams, padding);
}

inline unsigned int Observation::getNrSynthesizedBeams() const {
  return nrSynthesizedBeams;
}

inline unsigned int Observation::getNrPaddedSynthesizedBeams(unsigned int padding) const {
  return isa::utils::pad(nrSynthesizedBeams, padding);
}

inline unsigned int Observation::getNrSamplesPerBatch() const {
  return nrSamplesPerBatch;
}

inline float Observation::getSamplingRate() const {
  return samplingRate;
}

inline unsigned int Observation::getNrSamplesPerPaddedBatch(unsigned int padding) const {
  return isa::utils::pad(nrSamplesPerBatch, padding);
}

inline unsigned int Observation::getNrSubbands() const {
  return nrSubbands;
}

inline unsigned int Observation::getNrChannelsPerSubband() const {
  return nrChannelsPerSubband;
}

inline unsigned int Observation::getNrChannels() const {
  return nrChannels;
}

inline unsigned int Observation::getNrPaddedSubbands(unsigned int padding) const {
  return isa::utils::pad(nrSubbands, padding);
}

inline unsigned int Observation::getNrPaddedChannels(unsigned int padding) const {
  return isa::utils::pad(nrChannels, padding);
}

inline unsigned int Observation::getNrZappedChannels() const {
  return nrZappedChannels;
}

inline unsigned int Observation::getNrSamplesPerPaddedSubbandingDispersedChannel(unsigned int padding) const {
  return isa::utils::pad(nrSamplesPerSubbandingDispersedChannel, padding);
}

inline unsigned int Observation::getNrSamplesPerPaddedDispersedChannel(unsigned int padding) const {
  return isa::utils::pad(nrSamplesPerDispersedChannel, padding);
}

inline float Observation::getSubbandMinFreq() const {
  return subbandMinFreq;
}

inline float Observation::getSubbandMaxFreq() const {
  return subbandMaxFreq;
}

inline float Observation::getSubbandBandwidth() const {
  return subbandBandwidth;
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

inline unsigned int Observation::getNrSamplesPerSubbandingDispersedChannel() const {
  return nrSamplesPerSubbandingDispersedChannel;
}

inline unsigned int Observation::getNrSamplesPerDispersedChannel() const {
  return nrSamplesPerDispersedChannel;
}

inline unsigned int Observation::getNrSamplesPerBatchSubbanding() const {
  return nrSamplesPerBatchSubbanding;
}

inline unsigned int Observation::getNrSamplesPerPaddedBatchSubbanding(unsigned int padding) const {
  return isa::utils::pad(nrSamplesPerBatchSubbanding, padding);
}

inline unsigned int Observation::getNrDelayBatchesSubbanding() const {
  return nrDelayBatchesSubbanding;
}

inline unsigned int Observation::getNrDelayBatches() const {
  return nrDelayBatches;
}

inline unsigned int Observation::getNrDMsSubbanding() const {
  return nrDMsSubbanding;
}

inline unsigned int Observation::getNrDMs() const {
  return nrDMs;
}

inline unsigned int Observation::getNrPaddedDMsSubbanding(unsigned int padding) const {
  return isa::utils::pad(nrDMsSubbanding, padding);
}

inline unsigned int Observation::getNrPaddedDMs(unsigned int padding) const {
  return isa::utils::pad(nrDMs, padding);
}

inline float Observation::getFirstDMSubbanding() const {
  return firstDMSubbanding;
}

inline float Observation::getFirstDM() const {
  return firstDM;
}

inline float Observation::getLastDMSubbanding() const {
  return lastDMSubbanding;
}

inline float Observation::getLastDM() const {
  return lastDM;
}

inline float Observation::getDMSubbandingStep() const {
  return DMSubbandingStep;
}

inline float Observation::getDMStep() const {
  return DMStep;
}

inline unsigned int Observation::getNrPeriods() const {
  return nrPeriods;
}

inline unsigned int Observation::getNrPaddedPeriods(unsigned int padding) const {
  return isa::utils::pad(nrPeriods, padding);
}

inline unsigned int Observation::getNrBins() const {
  return nrBins;
}

inline unsigned int Observation::getNrPaddedBins(unsigned int padding) const {
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

