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
using isa::utils::ArgumentList;
using isa::OpenCL::GPUData;
using AstroData::readLOFAR;


int main(int argc, char *argv[]) {
	string headerFilename;
	string rawFilename;
	unsigned int nrSeconds = 0;
	unsigned int nrSamplesPerSecond = 0;
	unsigned int samplesInMem = 0;
	unsigned int nrChannels = 0;
	unsigned int nrOutputSamples = 0;

	// Parse command line
	if ( argc != 7 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -os <#output_samples>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		nrOutputSamples = args.getSwitchArgument< unsigned int >("-os");
	}
	catch ( exception &err ) {
		cerr << err.what() << endl;
		return 1;
	}

	// Load input
	vector< GPUData< float > * > *input = new vector< GPUData< float > * >(nrSeconds);
	readLOFAR(headerFilename, rawFilename, nrSeconds, nrSamplesPerSecond, nrChannels, *input);
	samplesInMem = nrSamplesPerSecond + (nrSamplesPerSecond % 4);

	// Plot the output
	ofstream oFile;
	
	oFile.open("./rawInput.dat");
	oFile << fixed << setprecision(3);
	for ( unsigned int sample = 0; sample < nrOutputSamples; sample++ ) {
		float oSample = 0.0f;

		for ( unsigned int channel = 0; channel < nrChannels; channel++ ) {
			oSample += ((input->at(sample / nrSamplesPerSecond))->getHostData())[(channel * samplesInMem) + (sample % nrSamplesPerSecond)];
		}

		oFile << sample << " " << oSample << endl;
	}
	oFile.close();

	return 0;
}

