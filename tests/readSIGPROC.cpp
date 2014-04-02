// Copyright 2014 Alessio Sclocco <a.sclocco@vu.nl>
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

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <exception>
using std::exception;
#include <string>
using std::string;

#include <ArgumentList.hpp>
using isa::utils::ArgumentList;
#include <Observation.hpp>
using AstroData::Observation;
#include <CLData.hpp>
using isa::OpenCL::CLData;
#include <ReadData.hpp>
using AstroData::readSIGPROC;


int main(int argc, char *argv[]) {
  unsigned int bytesToSkip = 0;
  Observation< float > obs("SIGPROC", "int");
  string dataFile;

  try {
    ArgumentList args(argc, argv);

    bytesToSkip = args.getSwitchArgument< unsigned int >("-header");
    dataFile = args.getSwitchArgument< string >("-data");
    obs.setNrSeconds(args.getSwitchArgument< unsigned int >("-seconds"));
    obs.setNrChannels(args.getSwitchArgument< unsigned int >("-channels"));
    obs.setNrSamplesPerSecond(args.getSwitchArgument< unsigned int >("-samples"));
  } catch ( exception &err ) {
    cerr << err.what() << endl;
    return 1;
  }

  vector< CLData< float > * > * input = new vector< CLData< float > * >(obs.getNrSeconds());
  readSIGPROC(obs, bytesToSkip, dataFile, *input);

  for ( unsigned int second = 0; second < obs.getNrSeconds(); second++ ) {
    for ( unsigned int sample = 0; sample < obs.getNrSamplesPerSecond(); sample++ ) {
      for ( int channel = obs.getNrChannels() - 1; channel >= 0; channel-- ) {
        cout << (*input->at(second))[(channel * obs.getNrSamplesPerPaddedSecond()) + sample] << " ";
      }
      cout << endl;
    }
    cout << endl << endl;
  }

  return 0;

}
