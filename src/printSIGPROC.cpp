/*
 * Copyright (C) 2012
 * Alessio Sclocco <a.sclocco@vu.nl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::fixed;
using std::setprecision;
using std::string;

#include <ArgumentList.hpp>
#include <GPUData.hpp>
#include <Exceptions.hpp>
#include <ReadData.hpp>
#include <Observation.hpp>
using isa::utils::ArgumentList;
using isa::OpenCL::GPUData;
using AstroData::readSIGPROC;
using AstroData::Observation;


int main(int argc, char *argv[]) {
	Observation< float > observation("SIGPROC", "float");
	string iFilename;
	unsigned int headerBytes = 0;
	unsigned int nrOutputSeconds = 0;

	// Parse command line
	if ( argc != 13 ) {
		cerr << "Usage: " << argv[0] << " -if <input_file> -h <header_bytes> -is <#seconds> -ss <#samples_per_second> -c <#channels> -os <output_seconds>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		iFilename = args.getSwitchArgument< string >("-if");
		headerBytes = args.getSwitchArgument< unsigned int >("-h");
		observation.setNrSeconds(args.getSwitchArgument< unsigned int >("-is"));
		observation.setNrSamplesPerSecond(args.getSwitchArgument< unsigned int >("-ss"));
		observation.setNrChannels(args.getSwitchArgument< unsigned int >("-c"));
		nrOutputSeconds = args.getSwitchArgument< unsigned int >("-os");
	}
	catch ( exception &err ) {
		cerr << err.what() << endl;
		return 1;
	}
	if ( (observation.getNrSamplesPerSecond() % 4) != 0 ) {
		observation.setNrSamplesPerPaddedSecond(observation.getNrSamplesPerSecond() + (4 - (observation.getNrSamplesPerSecond() % 4)));
	}
	else {
		observation.setNrSamplesPerPaddedSecond(observation.getNrSamplesPerSecond());
	}

	if ( nrOutputSeconds > observation.getNrSeconds() ) {
		cerr << "It is not possible to output more seconds than " << observation.getNrSeconds() << "." << endl;
		return 1;
	}

	// Load input
	vector< GPUData< float > * > *input = new vector< GPUData< float > * >(observation.getNrSeconds());
	
	readSIGPROC(observation, headerBytes, iFilename, *input);

	// Plot the output
	ofstream oFile;
	
	oFile.open("./rawSIGPROC.dat");
	oFile << fixed;
	for ( unsigned int second = 0; second < nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			float oSample = 0.0f;

			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				oSample += (input->at(second)->getHostData())[(channel * observation.getNrSamplesPerPaddedSecond()) + sample];
			}

			oFile << setprecision(6) << ((second * observation.getNrSamplesPerSecond()) + sample) * observation.getSamplingRate() << " " << setprecision(3) << oSample << endl;
		}
	}
	oFile.close();

	return 0;
}

