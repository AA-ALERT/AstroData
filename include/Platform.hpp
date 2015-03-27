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

#include <string>
#include <map>
#include <fstream>

#include <utils.hpp>

#ifndef PLATFORM_HPP
#define PLATFORM_HPP

namespace AstroData {

// Memory padding
typedef std::map< std::string, unsigned int > paddingConf;
// Vector unit width
typedef std::map< std::string, unsigned int > vectorWidthConf;

// Read configuration files
void readPaddingConf(paddingConf & padding, const std::string & paddingFilename);
void readVectorWidthConf(vectorWidthConf & vectorWidth, const std::string & vectorFilename);

} // AstroData

#endif // PLATFORM_HPP

