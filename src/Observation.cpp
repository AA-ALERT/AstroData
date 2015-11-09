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

Observation::Observation() : nrSeconds(0), nrStations(0), nrBeams(0), samplingRate(0.0f), nrSamplesPerSecond(0), nrSamplesPerDispersedChannel(0), nrChannels(0), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), nrDelaySeconds(0), nrDMs(0), firstDM(0.0f), lastDM(0.0f), DMStep(0.0f), nrPeriods(0), firstPeriod(0), lastPeriod(0), periodStep(0), nrBins(0) {}

Observation::~Observation() {}

void Observation::setNrSamplesPerSecond(const unsigned int samples) {
	samplingRate = 1.0f / samples;
	nrSamplesPerSecond = samples;
}

void Observation::setNrSamplesPerDispersedChannel(const unsigned int samples) {
  nrSamplesPerDispersedChannel = samples;
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

} // AstroData

