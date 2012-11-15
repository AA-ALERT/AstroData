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
	unsigned int nrOutputSeconds = 0;

	// Parse command line
	if ( argc != 9 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -of <output_file> -os <output_seconds>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		outFilename = args.getSwitchArgument< string >("-of");
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

	// Print some statistics
	cout << fixed << setprecision(3) << endl;
	cout << "Total seconds: \t\t" << observation.getNrSeconds() << endl;
	cout << "Output seconds: \t\t" << nrOutputSeconds << endl;
	cout << "Min frequency: \t\t" << observation.getMinFreq() << " MHz" << endl;
	cout << "Max frequency: \t\t" << observation.getMaxFreq() << " MHz" << endl;
	cout << "Nr. channels: \t\t" << observation.getNrChannels() << endl;
	cout << "Channel bandwidth: \t" << observation.getChannelBandwidth() << " MHz" << endl;
	cout << "Samples/second: \t" << observation.getNrSamplesPerSecond() << endl;
	cout << "Samples/second (pad): \t" << paddedSecond << endl;
	cout << "Min sample: \t\t" << observation.getMinValue() << endl;
	cout << "Max sample: \t\t" << observation.getMaxValue() << endl;
	cout << "Average sample: \t\t" << observation.getAverage() << endl;
	cout << "Variance: \t\t" << observation.getVariance() << endl;
	cout << "Standard deviation:  \t" << observation.getStdDev() << endl;
	cout << endl;	

	// Plot the output
	ofstream oFile;
	
	oFile.open(outFilename.c_str());
	oFile << fixed;
	for ( unsigned int second = 0; second < nrOutputSeconds; second++ ) {
		for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample++ ) {
			float oSample = 0.0f;

			for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
				oSample += (input->at(second)->getHostData())[(channel * paddedSecond) + sample];
			}

			oFile << setprecision(6) << ((second * observation.getNrSamplesPerSecond()) + sample) * observation.getSamplingRate() << " " << setprecision(3) << oSample << endl;
		}
	}
	oFile.close();

	return 0;
}

