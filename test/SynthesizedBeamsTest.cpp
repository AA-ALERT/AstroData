// Copyright 2019 Netherlands eScience Center and Netherlands Institute for Radio Astronomy (ASTRON)
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

#include <SynthesizedBeams.hpp>
#include <ArgumentList.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

const std::string wrongFileName = "does_not_exist";
const unsigned int padding = 128;
std::string path;

int main(int argc, char * argv[])
{
    testing::InitGoogleTest(&argc, argv);
    isa::utils::ArgumentList arguments(argc, argv);
    try
    {
        path = arguments.getSwitchArgument<std::string>("-path");
    }
    catch ( std::exception & err )
    {
        std::cerr << std::endl;
        std::cerr << "Required command line parameters:" << std::endl;
        std::cerr << "\t-path <string> // The path of the test input files" << std::endl;
        std::cerr << std::endl;
        return -1;
    }
    return RUN_ALL_TESTS();
}

TEST(BeamMapping, GenerateSingle)
{
    AstroData::Observation observation;
    std::vector<unsigned int> mapping;
    observation.setNrBeams(12);
    observation.setNrSynthesizedBeams(71);
    observation.setFrequencyRange(1, 1536, 0.0f, 0.0f);
    mapping.resize(observation.getNrSynthesizedBeams() * observation.getNrChannels(padding));
    AstroData::generateBeamMapping(observation, mapping, padding);
    for ( unsigned int sBeam = 0; sBeam < observation.getNrSynthesizedBeams(); sBeam++ )
    {
        for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ )
        {
            EXPECT_EQ(mapping[(sBeam * observation.getNrChannels(padding / sizeof(unsigned int))) + channel], sBeam % observation.getNrBeams());
        }
    }
}

TEST(BeamMapping, GenerateSubband)
{
    AstroData::Observation observation;
    std::vector<unsigned int> mapping;
    observation.setNrBeams(12);
    observation.setNrSynthesizedBeams(71);
    observation.setFrequencyRange(32, 1536, 0.0f, 0.0f);
    mapping.resize(observation.getNrSynthesizedBeams() * observation.getNrSubbands(padding));
    AstroData::generateBeamMapping(observation, mapping, padding, true);
    for ( unsigned int sBeam = 0; sBeam < observation.getNrSynthesizedBeams(); sBeam++ )
    {
        for ( unsigned int subband = 0; subband < observation.getNrSubbands(); subband++ )
        {
            EXPECT_EQ(mapping[(sBeam * observation.getNrSubbands(padding / sizeof(unsigned int))) + subband], sBeam % observation.getNrBeams());
        }
    }
}

TEST(BeamMapping, ReadSubband)
{
    AstroData::Observation observation;
    std::vector<unsigned int> mapping;
    observation.setNrBeams(12);
    observation.setNrSynthesizedBeams(71);
    observation.setFrequencyRange(32, 1536, 0.0f, 0.0f);
    mapping.resize(observation.getNrSynthesizedBeams() * observation.getNrSubbands(padding));
    AstroData::readBeamMapping(observation, path + "/sb_table.conf", mapping, padding, true);
    EXPECT_EQ(mapping[0], 4);
    EXPECT_EQ(mapping[31], 11);
    EXPECT_EQ(mapping[(18 * observation.getNrSubbands(padding / sizeof(unsigned int))) + 8], 3);
    EXPECT_EQ(mapping[(18 * observation.getNrSubbands(padding / sizeof(unsigned int))) + 18], 4);
    EXPECT_EQ(mapping[(18 * observation.getNrSubbands(padding / sizeof(unsigned int))) + 19], 4);
    EXPECT_EQ(mapping[(18 * observation.getNrSubbands(padding / sizeof(unsigned int))) + 27], 5);
    for ( unsigned int subband = 0; subband < observation.getNrSubbands(); subband++ )
    {
        EXPECT_EQ(mapping[(35 * observation.getNrSubbands(padding / sizeof(unsigned int))) + subband], 0);
    }
    EXPECT_EQ(mapping[(70 * observation.getNrSubbands(padding / sizeof(unsigned int)))], 8);
    EXPECT_EQ(mapping[(70 * observation.getNrSubbands(padding / sizeof(unsigned int))) + 31], 1);
}

TEST(BeamMapping, ReadIdentity)
{
    AstroData::Observation observation;
    std::vector<unsigned int> mapping;
    observation.setNrBeams(5);
    observation.setNrSynthesizedBeams(5);
    observation.setFrequencyRange(7, 70, 0.0f, 0.0f);
    mapping.resize(observation.getNrSynthesizedBeams() * observation.getNrSubbands(padding));
    AstroData::readBeamMapping(observation, path + "/sb_identity.conf", mapping, padding, true);
    for ( unsigned int sBeam = 0; sBeam < observation.getNrSynthesizedBeams(); sBeam++ )
    {
        for ( unsigned int subband = 0; subband < observation.getNrSubbands(); subband++ )
        {
            EXPECT_EQ(mapping[(sBeam * observation.getNrSubbands(padding / sizeof(unsigned int))) + subband], sBeam % observation.getNrBeams());
        }
    }
}
