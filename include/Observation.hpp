/*
 * Copyright (C) 2012
 * Alessio Sclocco <a.sclocco@vu.nl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <limits>
using std::string;
using std::numeric_limits;


#ifndef OBSERVATION_HPP
#define OBSERVATION_HPP

namespace AstroData {

template< typename T > class Observation {
public:
								Observation(string name, string dataType);
								~Observation();

	inline	void				setPadding(unsigned int pad);

	// General observation parameters
	inline	void 				setNrSeconds(unsigned int seconds);
	inline 	void 				setNrStations(unsigned int nrStations);
	inline 	void 				setNrBeams(unsigned int beams);
			void 				setNrSamplesPerSecond(unsigned int samples);
			void 				setNrChannels(unsigned int channels);

	// Frequency parameters
	inline 	void 				setMinFreq(float freq);
	inline 	void 				setMaxFreq(float freq);
	inline 	void 				setChannelBandwidth(float bandwidth);

	// Statistical properties
	inline 	void 				setMinValue(T value);
	inline 	void 				setMaxValue(T value);
	inline 	void 				setAverage(unsigned int channel, double avg);
	inline 	void 				setVariance(unsigned int channel, double var);
	inline 	void 				setStdDev(unsigned int channel, double dev);

	// Dispersion measures
			void 				setNrDMs(unsigned int dms);
	inline 	void 				setFirstDM(float dm);
	inline 	void 				setDMStep(float step);

	// Periods
			void 				setNrPeriods(unsigned int periods);
			void 				setNrBins(unsigned int bins);
	inline 	void				setBasePeriod(unsigned int period);
	inline 	void 				setFirstPeriod(unsigned int period);
	inline 	void 				setPeriodStep(unsigned int step);

	inline 	string 				getName() const;
	inline 	string 				getDataType() const;
	inline 	unsigned int 		getPadding() const;

	// General observation parameters
	inline 	unsigned int 		getNrSeconds() const;
	inline 	unsigned int 		getNrStations() const;
	inline 	unsigned int 		getNrBeams() const;
	inline 	unsigned int 		getNrSamplesPerSecond() const;
	inline 	float 				getSamplingRate() const;
	inline 	unsigned int 		getNrSamplesPerPaddedSecond() const;
	inline 	unsigned int 		getNrChannels() const;
	inline 	unsigned int 		getNrPaddedChannels() const;

	// Frequency parameters
	inline 	float 				getMinFreq() const;
	inline 	float 				getMaxFreq() const;
	inline 	float 				getChannelBandwidth() const;

	// Statistical properties
	inline 	T 					getMinValue() const;
	inline 	T 					getMaxValue() const;
	inline 	double 				getAverage(unsigned int channel) const;
	inline 	double 				getVariance(unsigned int channel) const;
	inline 	double 				getStdDev(unsigned int channel) const;

	// Dispersion measures
	inline 	unsigned int 		getNrDMs() const;
	inline 	unsigned int 		getNrPaddedDMs() const;
	inline 	float 				getFirstDM() const;
	inline 	float 				getDMStep() const;

	// Periods
	inline 	unsigned int 		getNrPeriods() const;
	inline 	unsigned int 		getNrPaddedPeriods() const;
	inline 	unsigned int 		getNrBins() const;
	inline 	unsigned int 		getNrPaddedBins() const;
	inline 	unsigned int 		getBasePeriod() const;
	inline 	unsigned int 		getFirstPeriod() const;
	inline 	unsigned int 		getPeriodStep() const;

private:
			string 				name;
			string 				dataType;
			unsigned int 		padding;

			unsigned int 		nrSeconds;
			unsigned int 		nrStations;
			unsigned int 		nrBeams;
			unsigned int 		nrSamplesPerSecond;
			float 				samplingRate;
			unsigned int 		nrSamplesPerPaddedSecond;
			unsigned int 		nrChannels;
			unsigned int 		nrPaddedChannels;

			float 				minFreq;
			float				maxFreq;
			float 				channelBandwidth;

			T 					minValue;
			T 					maxValue;
			double *			average;
			double *			variance;
			double *			stdDev;

			unsigned int 		nrDMs;
			unsigned int 		nrPaddedDMs;
			float 				firstDM;
			float 				DMStep;

			unsigned int 		nrPeriods;
			unsigned int 		nrPaddedPeriods;
			unsigned int 		nrBins;
			unsigned int 		nrPaddedBins;
			unsigned int 		basePeriod;
			unsigned int 		firstPeriod;
			unsigned int 		periodStep;
};


// Implementation

template< typename T > Observation< T >::Observation(string name, string dataType) : name(name), dataType(dataType), padding(1), nrSeconds(0), nrStations(0), nrBeams(0), nrSamplesPerSecond(0), samplingRate(0.0f), nrSamplesPerPaddedSecond(0), nrChannels(0), nrPaddedChannels(0), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), minValue(numeric_limits< T >::max()), maxValue(numeric_limits< T >::min()), average(0), variance(0), stdDev(0), nrDMs(0), nrPaddedDMs(0), firstDM(0.0f), DMStep(0.0f), nrPeriods(0), nrPaddedPeriods(0), nrBins(0), nrPaddedBins(0), basePeriod(0), firstPeriod(0), periodStep(0) {}

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

template< typename T > inline void Observation< T >::setBasePeriod(unsigned int period) {
	basePeriod = period;
}

template< typename T > inline void Observation< T >::setFirstPeriod(unsigned int period) {
	firstPeriod = period;
}

template< typename T > inline void Observation< T >::setPeriodStep(unsigned int step) {
	periodStep = step;
}

template< typename T > inline string Observation< T >::getName() const {
	return name;
}

template< typename T > inline string Observation< T >::getDataType() const {
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

template< typename T > inline unsigned int Observation< T >::getBasePeriod() const {
	return basePeriod;
}

template< typename T > inline unsigned int Observation< T >::getFirstPeriod() const {
	return firstPeriod;
}

template< typename T > inline unsigned int Observation< T >::getPeriodStep() const {
	return periodStep;
}

} // AstroData

#endif // OBSERVATION_HPP
