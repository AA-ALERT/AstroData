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

Observation::Observation() : nrBatches(0), nrStations(0), nrBeams(0), nrSynthesizedBeams(0), samplingRate(0.0f), nrSamplesPerBatchSubbanding(0), nrSamplesPerBatch(0), nrSamplesPerSubbandingDispersedChannel(0), nrSamplesPerDispersedChannel(0), nrSubbands(0), nrChannels(0), nrChannelsPerSubband(0), nrZappedChannels(0), subbandMinFreq(0.0), subbandMaxFreq(0.0), subbandBandwidth(0.0), minFreq(0.0f), maxFreq(0.0f), channelBandwidth(0.0f), nrDelayBatchesSubbanding(0), nrDelayBatches(0), nrDMsSubbanding(0), nrDMs(0), firstDMSubbanding(0.0f), firstDM(0.0f), lastDMSubbanding(0.0f), lastDM(0.0f), DMSubbandingStep(0.0f), DMStep(0.0f), nrPeriods(0), firstPeriod(0), lastPeriod(0), periodStep(0), nrBins(0) {}

Observation::~Observation() {}

void Observation::setNrBatches(const unsigned int seconds) {
  nrBatches = seconds;
}

void Observation::setNrStations(const unsigned int stations) {
  nrStations = stations;
}

void Observation::setNrBeams(const unsigned int beams) {
  nrBeams = beams;
}

void Observation::setNrSynthesizedBeams(const unsigned int beams) {
  nrSynthesizedBeams = beams;
}

void Observation::setNrDelayBatchesSubbanding(const unsigned int seconds) {
  nrDelayBatchesSubbanding = seconds;
}

void Observation::setNrDelayBatches(const unsigned int seconds) {
  nrDelayBatches = seconds;
}

void Observation::setSamplingRate(const float sampling) {
  samplingRate = sampling;
}

void Observation::setNrSamplesPerBatch(const unsigned int samples) {
  nrSamplesPerBatch = samples;
}

void Observation::setNrSamplesPerBatchSubbanding(const unsigned int samples) {
  nrSamplesPerBatchSubbanding = samples;
}

void Observation::setNrSamplesPerSubbandingDispersedChannel(const unsigned int samples) {
  nrSamplesPerSubbandingDispersedChannel = samples;
}

void Observation::setNrSamplesPerDispersedChannel(const unsigned int samples) {
  nrSamplesPerDispersedChannel = samples;
}

void Observation::setFrequencyRange(const unsigned int subbands, const unsigned int channels, const float baseFrequency, const float bandwidth) {
  nrSubbands = subbands;
  nrChannels = channels;
  nrChannelsPerSubband = channels / subbands;
  subbandMinFreq = baseFrequency + ((nrChannelsPerSubband / 2) * bandwidth);
  subbandMaxFreq = baseFrequency + ((nrChannels - (nrChannelsPerSubband / 2)) * bandwidth);
  subbandBandwidth = nrChannelsPerSubband * bandwidth;
  minFreq = baseFrequency;
  maxFreq = baseFrequency + ((channels - 1) * bandwidth);
  channelBandwidth = bandwidth;
}

void Observation::setNrZappedChannels(const unsigned int zappedChannels) {
  nrZappedChannels = zappedChannels;
}

void Observation::setDMSubbandingRange(const unsigned int dms, const float baseDM, const float step) {
  nrDMsSubbanding = dms;
  DMSubbandingStep = step;
  firstDMSubbanding = baseDM;
  lastDMSubbanding = baseDM + ((dms - 1) * step);
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

