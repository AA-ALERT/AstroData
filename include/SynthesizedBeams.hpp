// Copyright 2017 Netherlands Institute for Radio Astronomy (ASTRON)
// Copyright 2017 Netherlands eScience Center
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <vector>
#include <string>
#include <fstream>

#include <Observation.hpp>
#include "Platform.hpp"


#pragma once

namespace AstroData {
/**
 ** @brief Generate a mapping between input and synthesized beams.
 ** The current mapping simply associates one input beam to one synthesized beam.
 **
 ** @param observation Object containing the observation parameters.
 ** @param beamMapping The vector containing the index of input beams corresponding to synthesized beams.
 ** @param padding The padding used to store data structures.
 ** @param subbanding A flag to indicate if using or not subbanding mode.
 */
void generateBeamMapping(const AstroData::Observation & observation, std::vector<unsigned int> & beamMapping, const unsigned int padding, const bool subbanding = false);
/**
 ** @brief Read the mapping between input and synthesized beams from an input file.
 **
 ** @param observation Object containing the observation parameters.
 ** @param inputFilename The filename of the file containing the mappings.
 ** @param beamMapping The vector containing the index of input beams corresponding to synthesized beams.
 ** @param padding The padding used to store data structures.
 ** @param subbanding A flag to indicate if using or not subbanding mode.
 */
void readBeamMapping(const AstroData::Observation & observation, const std::string & inputFilename, std::vector<unsigned int> & beamMapping, const unsigned int padding, const bool subbanding = false);
} // AstroData

