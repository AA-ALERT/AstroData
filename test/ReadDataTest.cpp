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

#include <Observation.hpp>
#include <ReadData.hpp>
#include <string>
#include <vector>
#include <gtest/gtest.h>

TEST(ZappedChannels, FileError)
{
    AstroData::Observation observation;
    std::vector<unsigned int> channels;
    std::string wrongFileName = "./zappred_channels.conf";
    ASSERT_THROW(AstroData::readZappedChannels(observation, wrongFileName, channels), AstroData::FileError);    
}

TEST(ZappedChannels, MatchingChannels)
{
    AstroData::Observation observation;
    std::vector<unsigned int> channels;
    std::string fileName = "./zapped_channels.conf";
    observation.setFrequencyRange(1, 1024, 0.0f, 0.0f);
    channels.resize(observation.getNrChannels());
    AstroData::readZappedChannels(observation, fileName, channels);
    EXPECT_EQ(channels.at(4), 1);
    EXPECT_EQ(channels.at(39), 1);
    EXPECT_EQ(channels.at(7), 1);
    EXPECT_EQ(channels.at(19), 1);
    EXPECT_EQ(channels.at(1023), 1);
    EXPECT_EQ(channels.at(0), 1);
    EXPECT_EQ(channels.at(45), 0);
    EXPECT_EQ(channels.at(128), 0);
}
