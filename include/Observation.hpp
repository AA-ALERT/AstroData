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

using std::string;


#ifndef OBSERVATION_HPP
#define OBSERVATION_HPP

namespace AstroData {

template< typename T > class Observation {
public:
	Observation(string name, string dataType);

	// Set values
	inline void setNrSeconds(unsigned int seconds);
	inline void setNrStations(unsigned int nrStations);
	inline void setNrBeams(unsigned int beams);
	inline void setNrSamplesPerSecond(unsigned int samples);
	inline void setNrChannels(unsigned int channels);

	inline void setSamplingRate(float rate);
	inline void setMinFreq(float freq);
	inline void setMaxFreq(float freq);
	inline void setChannelBandwidth(float bandwidth);

	inline void setMinValue(T value);
	inline void setMaxValue(T value);
	inline void setAverage(float avg);
	inline void setVariance(float var);
	inline void setStdDev(float dev);

	// Get values
	inline string getName();
	inline string getDataType();

	inline unsigned int getNrSeconds();
	inline unsigned int getNrStations();
	inline unsigned int getNrBeams();
	inline unsigned int getNrSamplesPerSecond();
	inline unsigned int getNrChannels();

	inline float getSamplingRate();
	inline float getMinFreq();
	inline float getMaxFreq();
	inline float getChannelBandwidth();

	inline T getMinValue();
	inline T getMaxValue();
	inline float getAverage();
	inline float getVariance();
	inline float getStdDev();

private:
	string name;
	string dataType;

	unsigned int nrSeconds;
	unsigned int nrStations;
	unsigned int nrBeams;
	unsigned int nrSamplesPerSecond;
	unsigned int nrChannels;

	float samplingRate;
	float minFreq;
	float maxFreq;
	float channelBandwidth;

	T minValue;
	T maxValue;
	float average;
	float variance;
	float stdDev;
};


// Implementation

template< typename T > Observation< T >::Observation(string name, string dataType) : name(name), dataType(dataType), nrSeconds(0), nrStations(0), nrBeams(0), nrSamplesPerSecond(0), nrChannels(0), samplingRate(0.0f), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), minValue(0), maxValue(0), average(0.0f), variance(0.0f), stdDev(0.0f) {}

template< typename T > inline void Observation< T >::setNrSeconds(unsigned int seconds) {
	nrSeconds = seconds;
}

template< typename T > inline void Observation< T >::setNrStations(unsigned int stations) {
	nrStations = stations;
}

template< typename T > inline void Observation< T >::setNrBeams(unsigned int beams) {
	nrBeams = beams;
}

template< typename T > inline void Observation< T >::setNrSamplesPerSecond(unsigned int samples) {
	nrSamplesPerSecond = samples;
}

template< typename T > inline void Observation< T >::setNrChannels(unsigned int channels) {
	nrChannels = channels;
}

template< typename T > inline void Observation< T >::setSamplingRate(float rate) {
	samplingRate = rate;
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

template< typename T > inline void Observation< T >::setAverage(float avg) {
	average = avg;
}

template< typename T > inline void Observation< T >::setVariance(float var) {
	variance = var;
}

template< typename T > inline void Observation< T >::setStdDev(float dev) {
	stdDev = dev;
}

template< typename T > inline string Observation< T >::getName() {
	return name;
}

template< typename T > inline string Observation< T >::getDataType() {
	return dataType;
}

template< typename T > inline unsigned int Observation< T >::getNrSeconds() {
	return nrSeconds;
}

template< typename T > inline unsigned int Observation< T >::getNrStations() {
	return nrStations;
}

template< typename T > inline unsigned int Observation< T >::getNrBeams() {
	return nrBeams;
}

template< typename T > inline unsigned int Observation< T >::getNrSamplesPerSecond() {
	return nrSamplesPerSecond;
}

template< typename T > inline unsigned int Observation< T >::getNrChannels() {
	return nrChannels;
}

template< typename T > inline float Observation< T >::getSamplingRate() {
	return samplingRate;
}

template< typename T > inline float Observation< T >::getMinFreq() {
	return minFreq;
}

template< typename T > inline float Observation< T >::getMaxFreq() {
	return maxFreq;
}

template< typename T > inline float Observation< T >::getChannelBandwidth() {
	return channelBandwidth;
}

template< typename T > inline T Observation< T >::getMinValue() {
	return minValue;
}

template< typename T > inline T Observation< T >::getMaxValue() {
	return maxValue;
}

template< typename T > inline float Observation< T >::getAverage() {
	return average;
}

template< typename T > inline float Observation< T >::getVariance() {
	return variance;
}

template< typename T > inline float Observation< T >::getStdDev() {
	return stdDev;
}

} // AstroData

#endif // OBSERVATION_HPP
