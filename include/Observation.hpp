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

	// Set values
	inline void setNrSeconds(unsigned int seconds);
	inline void setNrStations(unsigned int nrStations);
	inline void setNrBeams(unsigned int beams);
	inline void setNrSamplesPerSecond(unsigned int samples);
	inline void setNrSamplesPerPaddedSecond(unsigned int samples);
	void setNrChannels(unsigned int channels);

	inline void setSamplingRate(float rate);
	inline void setMinFreq(float freq);
	inline void setMaxFreq(float freq);
	inline void setChannelBandwidth(float bandwidth);

	inline void setMinValue(T value);
	inline void setMaxValue(T value);
	inline void setAverage(unsigned int channel, double avg);
	inline void setVariance(unsigned int channel, double var);
	inline void setStdDev(unsigned int channel, double dev);

	inline void setNrDMs(unsigned int dms);
	inline void setFirstDM(float dm);
	inline void setDMStep(float step);

	// Get values
	inline string getName();
	inline string getDataType();

	inline unsigned int getNrSeconds();
	inline unsigned int getNrStations();
	inline unsigned int getNrBeams();
	inline unsigned int getNrSamplesPerSecond();
	inline unsigned int getNrSamplesPerPaddedSecond();
	inline unsigned int getNrChannels();

	inline float getSamplingRate();
	inline float getMinFreq();
	inline float getMaxFreq();
	inline float getChannelBandwidth();

	inline T getMinValue();
	inline T getMaxValue();
	inline double getAverage(unsigned int channel);
	inline double getVariance(unsigned int channel);
	inline double getStdDev(unsigned int channel);

	inline unsigned int getNrDMs();
	inline float getFirstDM();
	inline float getDMStep();

private:
	string name;
	string dataType;

	unsigned int nrSeconds;
	unsigned int nrStations;
	unsigned int nrBeams;
	unsigned int nrSamplesPerSecond;
	unsigned int nrSamplesPerPaddedSecond;
	unsigned int nrChannels;

	float samplingRate;
	float minFreq;
	float maxFreq;
	float channelBandwidth;

	T minValue;
	T maxValue;
	double *average;
	double *variance;
	double *stdDev;

	unsigned int nrDMs;
	float firstDM;
	float DMStep;
};


// Implementation

template< typename T > Observation< T >::Observation(string name, string dataType) : name(name), dataType(dataType), nrSeconds(0), nrStations(0), nrBeams(0), nrSamplesPerSecond(0), nrSamplesPerPaddedSecond(0), nrChannels(0), samplingRate(0.0f), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), minValue(numeric_limits< T >::max()), maxValue(numeric_limits< T >::min()), average(0.0), variance(0.0), stdDev(0.0), nrDMs(0), firstDM(0.0f), DMStep(0.0f) {}

template< typename T > Observation< T >::~Observation() {
	delete [] average;
	delete [] variance;
	delete [] stdDev;
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

template< typename T > inline void Observation< T >::setNrSamplesPerSecond(unsigned int samples) {
	nrSamplesPerSecond = samples;
}

template< typename T > inline void Observation< T >::setNrSamplesPerPaddedSecond(unsigned int samples) {
	nrSamplesPerPaddedSecond = samples;
}

template< typename T > void Observation< T >::setNrChannels(unsigned int channels) {
	nrChannels = channels;

	average = new double [channels];
	variance = new double [channels];
	stdDev = new double [channels];
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

template< typename T > inline void Observation< T >::setAverage(unsigned int channel, double avg) {
	average[channel] = avg;
}

template< typename T > inline void Observation< T >::setVariance(unsigned int channel, double var) {
	variance[channel] = var;
}

template< typename T > inline void Observation< T >::setStdDev(unsigned int channel, double dev) {
	stdDev[channel] = dev;
}

template< typename T > inline void Observation< T >::setNrDMs(unsigned int dms) {
	nrDMs = dms;
}

template< typename T > inline void Observation< T >::setFirstDM(float dm) {
	firstDM = dm;
}

template< typename T > inline void Observation< T >::setDMStep(float step) {
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

template< typename T > inline unsigned int Observation< T >::getNrSamplesPerPaddedSecond() {
	return nrSamplesPerPaddedSecond;
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

template< typename T > inline double Observation< T >::getAverage(unsigned int channel) {
	return average[channel];
}

template< typename T > inline double Observation< T >::getVariance(unsigned int channel) {
	return variance[channel];
}

template< typename T > inline double Observation< T >::getStdDev(unsigned int channel) {
	return stdDev[channel];
}

template< typename T > inline unsigned int Observation< T >::getNrDMs() {
	return nrDMs;
}

template< typename T > inline float Observation< T >::getFirstDM() {
	return firstDM;
}

template< typename T > inline float Observation< T >::getDMStep() {
	return DMStep;
}

} // AstroData

#endif // OBSERVATION_HPP
