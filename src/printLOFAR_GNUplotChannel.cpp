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
	string outFilename;
	unsigned int firstSecond = 0;
	unsigned int nrOutputSeconds = 0;
	unsigned int channel = 0;

	// Parse command line
	if ( argc != 13 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -of <output_file> -fs <first_second> -os <output_seconds> -ch <channel>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		outFilename = args.getSwitchArgument< string >("-of");
		firstSecond = args.getSwitchArgument< unsigned int >("-fs");
		nrOutputSeconds = args.getSwitchArgument< unsigned int >("-os");
		channel = args.getSwitchArgument< unsigned int >("-ch");
	}
	catch ( exception &err ) {
		cerr << err.what() << endl;
		return 1;
	}

	// Load input
	Observation< float > observation("LOFAR", "float");
	vector< GPUData< float > * > *input = new vector< GPUData< float > * >(1);

	readLOFAR(headerFilename, rawFilename, observation, *input, nrOutputSeconds, firstSecond);

	// Print some statistics
	cout << fixed << setprecision(3) << endl;
	cout << "Total seconds: \t\t" << observation.getNrSeconds() << endl;
	cout << "First output second: \t" << firstSecond << endl;
	cout << "Last output second: \t" << firstSecond + nrOutputSeconds << endl;
	cout << "Min frequency: \t\t" << observation.getMinFreq() << " MHz" << endl;
	cout << "Max frequency: \t\t" << observation.getMaxFreq() << " MHz" << endl;
	cout << "Nr. channels: \t\t" << observation.getNrChannels() << endl;
	cout << "Nr. channels (pad): \t" << observation.getNrPaddedChannels() << endl;
	cout << "Channel bandwidth: \t" << observation.getChannelBandwidth() << " MHz" << endl;
	cout << "Samples/second: \t" << observation.getNrSamplesPerSecond() << endl;
	cout << "Samples/second (pad): \t" << observation.getNrSamplesPerPaddedSecond() << endl;
	cout << "Min sample: \t\t" << observation.getMinValue() << endl;
	cout << "Max sample: \t\t" << observation.getMaxValue() << endl;
	cout << endl;	

	if ( channel >= observation.getNrChannels() ) {
		cerr << "It is not possible to print channel " << channel << "." << endl;
		return 1;
	}

	// Plot the output
	float minSample = numeric_limits< float >::max();
	float maxSample = numeric_limits< float >::min();
	double aCur = 0.0f;
	double aOld = 0.0f;
	double vCur = 0.0f;
	double vOld = 0.0f;
	ofstream oFile;
	
	oFile.open(outFilename.c_str());
	oFile << fixed;
	for ( unsigned int second = 0; second < observation.getNrSeconds(); second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			long long unsigned int element = (second * observation.getNrSamplesPerSecond()) + sample;
			float oSample = (input->at(second)->getHostData())[(channel * observation.getNrSamplesPerPaddedSecond()) + sample];

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

			oFile << setprecision(6) << (((firstSecond + second) * observation.getNrSamplesPerSecond()) + sample) * observation.getSamplingRate() << " " << setprecision(3) << oSample << endl;
		}
	}
	oFile.close();

	cout << "Min: \t\t\t" << minSample << endl;
	cout << "Max: \t\t\t" << maxSample << endl;
	cout << "Average: \t\t" << aCur << endl;
	cout << "Standard deviation: \t" << sqrt(vCur / (observation.getNrSeconds() * observation.getNrSamplesPerSecond())) << endl;
	cout << endl;

	return 0;
}

