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
  inline unsigned int getNrPaddedStations() const;
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
  unsigned int nrSamplesPerDispersedChannel;

  unsigned int nrChannels;
  float minFreq;
  float maxFreq;
  float channelBandwidth;

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

} // AstroData

#endif // OBSERVATION_HPP

