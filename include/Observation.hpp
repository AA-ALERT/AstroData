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
  void setFrequencyRange(const unsigned int channels, const float bandwidth, const float baseFrequency);

  // Dispersion measures
  void setDMRange(const unsigned int dms, const float baseDM, const float step);

  // Periods
  voit setPeriodRange(const unsigned int periods, const unsigned int basePeriod, const unsigned int step);
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

template< typename T > Observation< T >::Observation(std::string name, std::string dataType) : name(name), dataType(dataType), padding(1), nrSeconds(0), nrStations(0), nrBeams(0), nrSamplesPerSecond(0), samplingRate(0.0f), nrSamplesPerPaddedSecond(0), nrChannels(0), nrPaddedChannels(0), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), minValue(std::numeric_limits< T >::max()), maxValue(std::numeric_limits< T >::min()), average(0), variance(0), stdDev(0), nrSamplesPerDispersedChannel(0), nrDMs(0), nrPaddedDMs(0), firstDM(0.0f), DMStep(0.0f), nrPeriods(0), nrPaddedPeriods(0), nrBins(0), nrPaddedBins(0), firstPeriod(0), periodStep(0) {}

template< typename T > Observation< T >::~Observation() {
	delete [] average;
	delete [] variance;
	delete [] stdDev;
}

template< typename T > inline void Observation< T >::setPadding(unsigned int pad) {
	padding = pad;
}

template< typename T > inline void Observation< T >::setNrSeconds(unsigned int seconds) {
	nrSeconds = seconds;
}

template< typename T > inline void Observation< T >::setNrStations(unsigned int stations) {
	nrStations = stations;
}

template< typename T > inline void Observation< T >::setNrBeams(unsigned int beams) {
	nrBeams = beams;
}

template< typename T > void Observation< T >::setNrSamplesPerSecond(unsigned int samples) {
	nrSamplesPerSecond = samples;
	samplingRate = 1.0f / samples;
	if ( (nrSamplesPerSecond % padding) == 0 ) {
		nrSamplesPerPaddedSecond = nrSamplesPerSecond;
	}
	else {
		nrSamplesPerPaddedSecond = nrSamplesPerSecond + (padding - (nrSamplesPerSecond % padding));
	}
}

template< typename T > void Observation< T >::setNrChannels(unsigned int channels) {
	nrChannels = channels;

	average = new double [channels];
	variance = new double [channels];
	stdDev = new double [channels];

	if ( (nrChannels % padding) == 0 ) {
		nrPaddedChannels = nrChannels;
	}
	else {
		nrPaddedChannels = nrChannels + (padding - (nrChannels % padding));
	}
}

template< typename T > inline void Observation< T >::setMinFreq(float freq) {
	minFreq = freq;
}

template< typename T > inline void Observation< T >::setMaxFreq(float freq) {
	maxFreq = freq;
}

template< typename T > inline void Observation< T >::setChannelBandwidth(float bandwidth) {
	channelBandwidth = bandwidth;
}

template< typename T > inline void Observation< T >::setMinValue(T value) {
	minValue = value;
}

template< typename T > inline void Observation< T >::setMaxValue(T value) {
	maxValue = value;
}

template< typename T > inline void Observation< T >::setAverage(unsigned int channel, double avg) {
	average[channel] = avg;
}

template< typename T > inline void Observation< T >::setVariance(unsigned int channel, double var) {
	variance[channel] = var;
}

template< typename T > inline void Observation< T >::setStdDev(unsigned int channel, double dev) {
	stdDev[channel] = dev;
}

template< typename T > void Observation< T >::setNrSamplesPerDispersedChannel(unsigned int samples) {
  nrSamplesPerDispersedChannel = isa::utils::pad(samples, padding);
}

template< typename T > void Observation< T >::setNrDMs(unsigned int dms) {
	nrDMs = dms;

	if ( (nrDMs % padding) == 0 ) {
		nrPaddedDMs = nrDMs;
	}
	else {
		nrPaddedDMs = nrDMs + (padding - (nrDMs % padding));
	}
}

template< typename T > inline void Observation< T >::setFirstDM(float dm) {
	firstDM = dm;
}

template< typename T > inline void Observation< T >::setDMStep(float step) {
	DMStep = step;
}

template< typename T > void Observation< T >::setNrPeriods(unsigned int periods) {
	nrPeriods = periods;

	if ( (nrPeriods % padding) == 0 ) {
		nrPaddedPeriods = nrPeriods;
	}
	else {
		nrPaddedPeriods = nrPeriods + (padding - (nrPeriods % padding));
	}
}

template< typename T > void Observation< T >::setNrBins(unsigned int bins) {
	nrBins = bins;

	if ( (nrBins % padding) == 0 ) {
		nrPaddedBins = nrBins;
	}
	else {
		nrPaddedBins = nrBins + (padding - (nrBins % padding));
	}
}

template< typename T > inline void Observation< T >::setFirstPeriod(unsigned int period) {
	firstPeriod = period;
}

template< typename T > inline void Observation< T >::setPeriodStep(unsigned int step) {
	periodStep = step;
}

template< typename T > inline std::string Observation< T >::getName() const {
	return name;
}

template< typename T > inline std::string Observation< T >::getDataType() const {
	return dataType;
}

template< typename T > inline unsigned int Observation< T >::getPadding() const {
	return padding;
}

template< typename T > inline unsigned int Observation< T >::getNrSeconds() const {
	return nrSeconds;
}

template< typename T > inline unsigned int Observation< T >::getNrStations() const {
	return nrStations;
}

template< typename T > inline unsigned int Observation< T >::getNrBeams() const {
	return nrBeams;
}

template< typename T > inline unsigned int Observation< T >::getNrSamplesPerSecond() const {
	return nrSamplesPerSecond;
}

template< typename T > inline float Observation< T >::getSamplingRate() const {
	return samplingRate;
}

template< typename T > inline unsigned int Observation< T >::getNrSamplesPerPaddedSecond() const {
	return nrSamplesPerPaddedSecond;
}

template< typename T > inline unsigned int Observation< T >::getNrChannels() const {
	return nrChannels;
}

template< typename T > inline unsigned int Observation< T >::getNrPaddedChannels() const {
	return nrPaddedChannels;
}

template< typename T > inline float Observation< T >::getMinFreq() const {
	return minFreq;
}

template< typename T > inline float Observation< T >::getMaxFreq() const {
	return maxFreq;
}

template< typename T > inline float Observation< T >::getChannelBandwidth() const {
	return channelBandwidth;
}

template< typename T > inline T Observation< T >::getMinValue() const {
	return minValue;
}

template< typename T > inline T Observation< T >::getMaxValue() const {
	return maxValue;
}

template< typename T > inline double Observation< T >::getAverage(unsigned int channel) const {
	return average[channel];
}

template< typename T > inline double Observation< T >::getVariance(unsigned int channel) const {
	return variance[channel];
}

template< typename T > inline double Observation< T >::getStdDev(unsigned int channel) const {
	return stdDev[channel];
}

template< typename T > inline unsigned int Observation< T >::getNrSamplesPerDispersedChannel() const {
  return nrSamplesPerDispersedChannel;
}

template< typename T > inline unsigned int Observation< T >::getNrDMs() const {
	return nrDMs;
}

template< typename T > inline unsigned int Observation< T >::getNrPaddedDMs() const {
	return nrPaddedDMs;
}

template< typename T > inline float Observation< T >::getFirstDM() const {
	return firstDM;
}

template< typename T > inline float Observation< T >::getDMStep() const {
	return DMStep;
}

template< typename T > inline unsigned int Observation< T >::getNrPeriods() const {
	return nrPeriods;
}

template< typename T > inline unsigned int Observation< T >::getNrPaddedPeriods() const {
	return nrPaddedPeriods;
}

template< typename T > inline unsigned int Observation< T >::getNrBins() const {
	return nrBins;
}

template< typename T > inline unsigned int Observation< T >::getNrPaddedBins() const {
	return nrPaddedBins;
}

template< typename T > inline unsigned int Observation< T >::getFirstPeriod() const {
	return firstPeriod;
}

template< typename T > inline unsigned int Observation< T >::getPeriodStep() const {
	return periodStep;
}

} // AstroData

#endif // OBSERVATION_HPP

