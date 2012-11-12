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

class Observation {
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
};


// Implementation

Observation::Observation(string name, string dataType) : name(name), dataType(dataType), nrSeconds(0), nrStations(0), nrBeams(0), nrSamplesPerSecond(0), nrChannels(0), samplingRate(0.0f), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f) {}

inline void Observation::setNrSeconds(unsigned int seconds) {
	nrSeconds = seconds;
}

inline void Observation::setNrStations(unsigned int stations) {
	nrStations = stations;
}

inline void Observation::setNrBeams(unsigned int beams) {
	nrBeams = beams;
}

inline void Observation::setNrSamplesPerSecond(unsigned int samples) {
	nrSamplesPerSecond = samples;
}

inline void Observation::setNrChannels(unsigned int channels) {
	nrChannels = channels;
}

inline void Observation::setSamplingRate(float rate) {
	samplingRate = rate;
}

inline void Observation::setMinFreq(float freq) {
	minFreq = freq;
}

inline void Observation::setMaxFreq(float freq) {
	maxFreq = freq;
}

inline void Observation::setChannelBandwidth(float bandwidth) {
	channelBandwidth = bandwidth;
}

inline string Observation::getName() {
	return name;
}

inline string Observation::getDataType() {
	return dataType;
}

inline unsigned int Observation::getNrSeconds() {
	return nrSeconds;
}

inline unsigned int Observation::getNrStations() {
	return nrStations;
}

inline unsigned int Observation::getNrBeams() {
	return nrBeams;
}

inline unsigned int Observation::getNrSamplesPerSecond() {
	return nrSamplesPerSecond;
}

inline unsigned int Observation::getNrChannels() {
	return nrChannels;
}

inline float Observation::getSamplingRate() {
	return samplingRate;
}

inline float Observation::getMinFreq() {
	return minFreq;
}

inline float Observation::getMaxFreq() {
	return maxFreq;
}

inline float Observation::getChannelBandwidth() {
	return channelBandwidth;
}

} // AstroData

#endif // OBSERVATION_HPP
