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

#include <ReadData.hpp>
#include <ArgumentList.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

std::string const wrongFileName = "does_not_exist";
std::string channelsFileName;
std::string integrationStepsFileName;

int main(int argc, char * argv[])
{
    testing::InitGoogleTest(&argc, argv);
    isa::utils::ArgumentList arguments(argc, argv);
    try
    {
        channelsFileName = arguments.getSwitchArgument<std::string>("-zapped_channels_file");
        integrationStepsFileName = arguments.getSwitchArgument<std::string>("-integration_steps_file");
    }
    catch ( isa::utils::EmptyCommandLine & err )
    {
        std::cerr << std::endl;
        std::cerr << "Required command line parameters:" << std::endl;
        std::cerr << "\t-zapped_channels_file <string>" << std::endl;
        std::cerr << "\t-integration_steps_file <string>" << std::endl;
        std::cerr << std::endl;
    }
    return RUN_ALL_TESTS();
}

TEST(ZappedChannels, FileError)
{
    AstroData::Observation observation;
    std::vector<unsigned int> channels;
    ASSERT_THROW(AstroData::readZappedChannels(observation, wrongFileName, channels), AstroData::FileError);
}

TEST(ZappedChannels, MatchingChannels)
{
    AstroData::Observation observation;
    std::vector<unsigned int> channels;
    observation.setFrequencyRange(1, 1024, 0.0f, 0.0f);
    channels.resize(observation.getNrChannels());
    AstroData::readZappedChannels(observation, channelsFileName, channels);
    EXPECT_EQ(channels.size(), 7);
    EXPECT_EQ(channels.at(4), 1);
    EXPECT_EQ(channels.at(39), 1);
    EXPECT_EQ(channels.at(7), 1);
    EXPECT_EQ(channels.at(19), 1);
    EXPECT_EQ(channels.at(1023), 1);
    EXPECT_EQ(channels.at(0), 1);
    EXPECT_NE(channels.at(45), 1);
    EXPECT_NE(channels.at(128), 1);
}

TEST(IntegrationSteps, FileError)
{
    AstroData::Observation observation;
    std::set<unsigned int> steps;
    ASSERT_THROW(AstroData::readIntegrationSteps(observation, wrongFileName, steps), AstroData::FileError);
}

TEST(IntegrationSteps, MatchingSteps)
{
    AstroData::Observation observation;
    std::set<unsigned int> steps;
    observation.setNrSamplesPerBatch(12500);
    AstroData::readIntegrationSteps(observation, channelsFileName, steps);
    EXPECT_EQ(steps.size(), 3);
    EXPECT_EQ(*steps.find(2), 2);
    EXPECT_EQ(*steps.find(5), 5);
    EXPECT_EQ(*steps.find(100), 100);
    EXPECT_EQ(steps.find(12500), steps.end());
}
