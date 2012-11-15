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
#include <cmath>
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
using std::sqrt;
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
	string outFilename;
	unsigned int paddedSecond = 0;
	unsigned int firstSecond = 0;
	unsigned int nrOutputSeconds = 0;
	unsigned int channelMagnifyingFactor = 0;
	unsigned int timeIntegrationFactor = 0;

	// Parse command line
	if ( argc != 15 ) {
		cerr << "Usage: " << argv[0] << " -hf <header_file> -rf <raw_file> -of <output_file > -fs <first_second> -os <output_seconds> -cm <channel_magnifying_factor> -if <time_integration_factor>" << endl;
		return 1;
	}
	try {
		ArgumentList args(argc, argv);

		headerFilename = args.getSwitchArgument< string >("-hf");
		rawFilename = args.getSwitchArgument< string >("-rf");
		outFilename = args.getSwitchArgument< string >("-of");
		firstSecond = args.getSwitchArgument< unsigned int >("-fs");
		nrOutputSeconds = args.getSwitchArgument< unsigned int >("-os");
		channelMagnifyingFactor = args.getSwitchArgument< unsigned int >("-cm");
		timeIntegrationFactor = args.getSwitchArgument< unsigned int >("-if");
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
	float diffMinMax = observation.getMaxValue() - observation.getMinValue();
	float *aCur = new float [observation.getNrChannels()];
	float *aOld = new float [observation.getNrChannels()];
	float *vCur = new float [observation.getNrChannels()];
	float *vOld = new float [observation.getNrChannels()];
	CImg< unsigned char > oImage(nrOutputSeconds * (observation.getNrSamplesPerSecond() / timeIntegrationFactor), observation.getNrChannels() * channelMagnifyingFactor, 1, 1);

	for ( unsigned int second = firstSecond; second < firstSecond + nrOutputSeconds; second++ ) {
		for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
			for ( unsigned int sample = 0; sample < observation.getNrSamplesPerSecond(); sample += timeIntegrationFactor ) {
				unsigned int counter = 0;
				float value = 0.0f;
				
				for ( unsigned int time = 0; time < timeIntegrationFactor; time++ ) {
					long long unsigned int element = (channel * observation.getNrSamplesPerSecond()) + (sample + time);

					if ( (sample + time) < observation.getNrSamplesPerSecond() ) {
						float temp = (input->at(second)->getHostData())[(channel * paddedSecond) + (sample + time)] - observation.getMinValue();

						value += temp;
						counter++;

						if ( element == 0 ) {
							aCur[channel] = temp;
							vCur[channel] = 0.0f;
						}
						else {
							aOld[channel] = aCur[channel];
							vOld[channel] = vCur[channel];

							aCur[channel] = aOld[channel] + ((temp - aOld[channel]) / (element + 1));
							vCur[channel] = vOld[channel] + ((temp - aOld[channel]) * (temp - aCur[channel]));
						}
					}
					else {
						break;
					}
				}
				value /= counter;
				
				for ( unsigned int magnifier = 0; magnifier < channelMagnifyingFactor; magnifier++ ) {
					oImage(((second * observation.getNrSamplesPerSecond()) + sample) / timeIntegrationFactor, (channel * channelMagnifyingFactor) + magnifier, 0, 0) = 256 - static_cast< unsigned int >((value * 256) / diffMinMax);
				}
			}
		}
	}
	oImage.save(outFilename.c_str());

	cout << "Interval statistics" << endl;
	for ( unsigned int channel = 0; channel < observation.getNrChannels(); channel++ ) {
		cout << "Average channel " << channel << ": \t" << aCur[channel] << endl;
		cout << "Variance channel " << channel << ": \t" << vCur[channel] / (nrOutputSeconds * observation.getNrChannels() * observation.getNrSamplesPerSecond()) << endl;
		cout << "Std. dev. channel " << channel << ": \t" << sqrt(vCur[channel] / (nrOutputSeconds * observation.getNrChannels() * observation.getNrSamplesPerSecond())) << endl;
	}
	cout << endl;

	return 0;
}

