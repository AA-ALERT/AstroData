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
#include <ColorMap.hpp>
using isa::utils::ArgumentList;
using isa::OpenCL::GPUData;
using AstroData::readLOFAR;
using AstroData::Observation;
using AstroData::Color;
using AstroData::getColorMap;


int main(int argc, char *argv[]) {
	string headerFilename;
	string rawFilename;
	string outFilename;
	unsigned int firstSecond = 0;
	unsigned int nrOutputSeconds = 0;
	unsigned int magnifyingFactor = 0;

	// Parse command line
	if ( argc != 13 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -of <output_file> -fs <first_second> -os <output_seconds> -mf <magnifying_factor>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		outFilename = args.getSwitchArgument< string >("-of");
		firstSecond = args.getSwitchArgument< unsigned int >("-fs");
		nrOutputSeconds = args.getSwitchArgument< unsigned int >("-os");
		magnifyingFactor = args.getSwitchArgument< unsigned int >("-mf");
	}
	catch ( exception &err ) {
		cerr << err.what() << endl;
		return 1;
	}

	// Load input
	Observation< float > observation("LOFAR", "float");
	vector< GPUData< float > * > *input = new vector< GPUData< float > * >(1);

	readLOFAR(headerFilename, rawFilename, observation, *input);

	// Print some statistics
	cout << fixed << setprecision(3) << endl;
	cout << "Total seconds: \t\t" << observation.getNrSeconds() << endl;
	cout << "First output second: \t" << firstSecond << endl;
	cout << "Last output second: \t" << firstSecond + nrOutputSeconds << endl;
	cout << "Min frequency: \t\t" << observation.getMinFreq() << " MHz" << endl;
	cout << "Max frequency: \t\t" << observation.getMaxFreq() << " MHz" << endl;
	cout << "Nr. channels: \t\t" << observation.getNrChannels() << endl;
	cout << "Channel bandwidth: \t" << observation.getChannelBandwidth() << " MHz" << endl;
	cout << "Samples/second: \t" << observation.getNrSamplesPerSecond() << endl;
	cout << "Samples/second (pad): \t" << observation.getNrSamplesPerPaddedSecond() << endl;
	cout << "Min sample: \t\t" << observation.getMinValue() << endl;
	cout << "Max sample: \t\t" << observation.getMaxValue() << endl;
	cout << endl;	

	// Plot the output
	float minSample = numeric_limits< float >::max();
	float maxSample = numeric_limits< float >::min();
	double aCur = 0.0f;
	double aOld = 0.0f;
	double vCur = 0.0f;
	double vOld = 0.0f;
	
	for ( unsigned int second = firstSecond; second < firstSecond + nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			long long unsigned int element = ((second - firstSecond) * observation.getNrSamplesPerSecond()) + sample;
			float oSample = 0.0f;

			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				oSample += (input->at(second)->getHostData())[(channel * observation.getNrSamplesPerPaddedSecond()) + sample];
			}

			if ( oSample < minSample ) {
				minSample = oSample;
			}
			if ( oSample > maxSample ) {
				maxSample = oSample;
			}

			if ( element == 0 ) {
				aCur = oSample;
				vCur = 0.0;
			}
			else {
				aOld = aCur;
				vOld = vCur;

				aCur = aOld + ((oSample - aOld) / (element + 1));
				vCur = vOld + ((oSample - aOld) * (oSample - aCur));
			}
		}
	}
	
	cout << "Min: \t\t\t" << minSample << endl;
	cout << "Max: \t\t\t" << maxSample << endl;
	cout << "Average: \t\t" << aCur << endl;
	cout << "Variance: \t\t" << vCur / (nrOutputSeconds * observation.getNrSamplesPerSecond()) << endl;
	cout << "Standard deviation: \t" << sqrt(vCur / (nrOutputSeconds * observation.getNrSamplesPerSecond())) << endl;
	cout << endl;
	
	float diffMinMax = maxSample - minSample;
	CImg< unsigned char > oImage(nrOutputSeconds * observation.getNrSamplesPerSecond(), magnifyingFactor, 1, 3);
	Color *colorMap = getColorMap();
	
	for ( unsigned int second = firstSecond; second < firstSecond + nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			float oSample = 0.0f;
			
			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				oSample += (input->at(second)->getHostData())[(channel * observation.getNrSamplesPerPaddedSecond()) + sample];
			}
			oSample -= minSample;

			for ( unsigned int magnifier = 0; magnifier < magnifyingFactor; magnifier++ ) {
				oImage(((second - firstSecond) * observation.getNrSamplesPerSecond()) + sample, magnifier, 0, 0) = (colorMap[static_cast< unsigned int >((oSample * 257) / diffMinMax)]).getR();
				oImage(((second - firstSecond) * observation.getNrSamplesPerSecond()) + sample, magnifier, 0, 1) = (colorMap[static_cast< unsigned int >((oSample * 257) / diffMinMax)]).getG();
				oImage(((second - firstSecond) * observation.getNrSamplesPerSecond()) + sample, magnifier, 0, 2) = (colorMap[static_cast< unsigned int >((oSample * 257) / diffMinMax)]).getB();
			}
		}
	}
	oImage.save(outFilename.c_str());

	delete [] colorMap;

	return 0;
}

