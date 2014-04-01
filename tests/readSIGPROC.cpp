//
// Copyright (C) 2014
// Alessio Sclocco <a.sclocco@vu.nl>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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
