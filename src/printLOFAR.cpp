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
using AstroData::readLOFAR;
using AstroData::Observation;


int main(int argc, char *argv[]) {
	string headerFilename;
	string rawFilename;
	string outFilename;
	unsigned int paddedSecond = 0;
	unsigned int firstSecond = 0;
	unsigned int nrOutputSeconds = 0;

	// Parse command line
	if ( argc != 11 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -of <output_file> -fs <first_second> -os <output_seconds>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		outFilename = args.getSwitchArgument< string >("-of");
		firstSecond = args.getSwitchArgument< unsigned int >("-fs");
		nrOutputSeconds = args.getSwitchArgument< unsigned int >("-os");
	}
	catch ( exception &err ) {
		cerr << err.what() << endl;
		return 1;
	}

	// Load input
	Observation< float > observation("LOFAR", "float");
	vector< GPUData< float > * > *input = new vector< GPUData< float > * >(1);

	readLOFAR(headerFilename, rawFilename, observation, &paddedSecond, *input);

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
	cout << "Samples/second (pad): \t" << paddedSecond << endl;
	cout << "Min sample: \t\t" << observation.getMinValue() << endl;
	cout << "Max sample: \t\t" << observation.getMaxValue() << endl;
	cout << "Average sample: \t" << observation.getAverage() << endl;
	cout << "Variance: \t\t" << observation.getVariance() << endl;
	cout << "Standard deviation: \t" << observation.getStdDev() << endl;
	cout << endl;	

	// Plot the output
	float aCur = 0.0f;
	float aOld = 0.0f;
	float vCur = 0.0f;
	float vOld = 0.0f;
	ofstream oFile;
	
	oFile.open(outFilename.c_str());
	oFile << fixed;
	for ( unsigned int second = firstSecond; second < firstSecond + nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			long long unsigned int element = ((second - firstSecond) * observation.getNrSamplesPerSecond()) + sample;
			float oSample = 0.0f;

			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				oSample += (input->at(second)->getHostData())[(channel * paddedSecond) + sample];
			}

			if ( element == 0 ) {
				aCur = oSample;
				vCur = 0.0f;
			}
			else {
				aOld = aCur;
				vOld = vCur;

				aCur = aOld + ((oSample - aOld) / (element + 1));
				vCur = vOld + ((oSample - aOld) * (oSample - aCur));
			}

			oFile << setprecision(6) << ((second * observation.getNrSamplesPerSecond()) + sample) * observation.getSamplingRate() << " " << setprecision(3) << oSample << endl;
		}
	}
	oFile.close();

	cout << "Interval statistics" << endl;
	cout << "Average: \t\t" << aCur << endl;
	cout << "Variance: \t\t " << vCur / (nrOutputSeconds * observation.getNrSamplesPerSecond()) << endl;
	cout << "Standard deviation: \t" << sqrt(vCur / (nrOutputSeconds * observation.getNrSamplesPerSecond())) << endl;
	cout << endl;

	return 0;
}

