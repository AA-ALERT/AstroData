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
#include <limits>
#include <CImg.h>
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
using std::numeric_limits;
using cimg_library::CImg;

#include <ArgumentList.hpp>
#include <GPUData.hpp>
#include <Exceptions.hpp>
#include <ReadData.hpp>
#include <Observation.hpp>
using isa::utils::ArgumentList;
using isa::OpenCL::GPUData;
using AstroData::readLOFAR;
using AstroData::Observation;


int main(int argc, char *argv[]) {
	string headerFilename;
	string rawFilename;
	unsigned int paddedSecond = 0;
	unsigned int nrOutputSeconds = 0;

	// Parse command line
	if ( argc != 7 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -os <output_seconds>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		nrOutputSeconds = args.getSwitchArgument< unsigned int >("-os");
	}
	catch ( exception &err ) {
		cerr << err.what() << endl;
		return 1;
	}

	// Load input
	Observation observation("LOFAR", "float");
	vector< GPUData< float > * > *input = new vector< GPUData< float > * >(1);
	readLOFAR(headerFilename, rawFilename, observation, &paddedSecond, *input);

	// Plot the output
	float maxSample = numeric_limits< float >::min();
	float minSample = numeric_limits< float >::max();
	float diffMinMax = 0.0f;
	CImg< unsigned char > oImage(nrOutputSeconds * observation.getNrSamplesPerSecond(), observation.getNrChannels(), 1, 1);
	
	for ( unsigned int second = 0; second < nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				float value = (input->at(second)->getHostData())[(channel * paddedSecond) + sample];

				if ( value < minSample ) {
					minSample = value;
				}
				if ( value > maxSample ) {
					maxSample = value;
				}
			}
		}
	}
	diffMinMax = maxSample - minSample;

	for ( unsigned int second = 0; second < nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				float value = (input->at(second)->getHostData())[(channel * paddedSecond) + sample] - minSample;
				
				oImage((second * observation.getNrSamplesPerSecond()) + sample, channel, 0, 0) = (value * 256) / diffMinMax;
			}
		}
	}
	oImage.display();
	oImage.save("./rawLOFARChannels.bmp");

	return 0;
}

