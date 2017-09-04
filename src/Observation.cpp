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

Observation::Observation() : nrBatches(0), nrStations(0), nrBeams(0), nrSynthesizedBeams(0), samplingTime(0.0f), nrSamplesPerBatch(0), nrSamplesPerBatch_subbanding(0), nrSamplesPerDispersedChannel(0), nrSamplesPerDispersedChannel_subbanding(0), nrSubbands(0), nrChannels(0), nrChannelsPerSubband(0), nrZappedChannels(0), minSubbandFreq(0.0), maxSubbandFreq(0.0), subbandBandwidth(0.0), minChannelFreq(0.0f), maxChannelFreq(0.0f), channelBandwidth(0.0f), nrDelayBatches(0), nrDelayBatches_subbanding(0), nrDMs(0), nrDMs_subbanding(0), firstDM(0.0f), firstDM_subbanding(0.0f), lastDM(0.0f), lastDM_subbanding(0.0f), DMStep(0.0f), DMStep_subbanding(0.0f), nrPeriods(0), firstPeriod(0), lastPeriod(0), periodStep(0), nrBins(0) {}

Observation::~Observation() {}

void Observation::setFrequencyRange(const unsigned int subbands, const unsigned int channels, const float baseFrequency, const float bandwidth) {
  nrSubbands = subbands;
  nrChannels = channels;
  nrChannelsPerSubband = channels / subbands;
  minSubbandFreq = baseFrequency + ((nrChannelsPerSubband / 2) * bandwidth);
  maxSubbandFreq = baseFrequency + ((nrChannels - (nrChannelsPerSubband / 2)) * bandwidth);
  subbandBandwidth = nrChannelsPerSubband * bandwidth;
  minChannelFreq = baseFrequency;
  maxChannelFreq = baseFrequency + ((channels - 1) * bandwidth);
  channelBandwidth = bandwidth;
}

void Observation::setDMSubbandingRange(const unsigned int dms, const float baseDM, const float step) {
  nrDMsSubbanding = dms;
  DMStep_subbanding = step;
  firstDM_subbanding = baseDM;
  lastDM_subbanding = baseDM + ((dms - 1) * step);
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

} // AstroData

