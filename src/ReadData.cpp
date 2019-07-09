// Copyright 2017 Netherlands eScience Center and Netherlands Institute for Radio Astronomy (ASTRON)
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

#include <ReadData.hpp>

namespace AstroData
{

RingBufferError::RingBufferError(const std::string &message) : message(message) {}

RingBufferError::~RingBufferError() noexcept {}

const char *RingBufferError::what() const noexcept
{
    return message.c_str();
}

void readZappedChannels(Observation &observation, const std::string &inputFilename, std::vector<unsigned int> &zappedChannels)
{
    unsigned int nrChannels = 0;
    std::ifstream input;

    input.open(inputFilename);
    if (!input)
    {
        throw FileError("ERROR: impossible to open zapped channels file \"" + inputFilename + "\"");
    }
    while (!input.eof())
    {
        unsigned int channel = observation.getNrChannels();

        input >> channel;
        if (channel < observation.getNrChannels())
        {
            zappedChannels[channel] = 1;
            nrChannels++;
        }
    }
    input.close();
    observation.setNrZappedChannels(nrChannels);
}

void readIntegrationSteps(const Observation &observation, const std::string &inputFilename, std::set<unsigned int> &integrationSteps)
{
    std::ifstream input;

    input.open(inputFilename);
    if (!input)
    {
        throw FileError("ERROR: impossible to open integration steps file \"" + inputFilename + "\"");
    }
    while (!input.eof())
    {
        unsigned int step = observation.getNrSamplesPerBatch();

        input >> step;
        if (step < observation.getNrSamplesPerBatch())
        {
            integrationSteps.insert(step);
        }
    }
    input.close();
}

std::uint64_t getSIGPROCHeaderSize(const std::string &inputFilename)
{
    std::uint64_t headerSize = 0;
    std::uint8_t state = 0;
    char buffer = '\0';
    std::ifstream inputFile;

    inputFile.open(inputFilename.c_str(), std::ios::binary);
    inputFile.exceptions(std::ifstream::failbit);
    if ( !inputFile )
    {
        throw FileError("ERROR: impossible to open SIGPROC file \"" + inputFilename + "\".");
    }
    while ( inputFile.get(buffer) )
    {
        switch ( buffer )
        {
            case 'H':
                state = 1;
                break;
            case 'E':
                if ( (state == 1) || (state == 4) || (state == 7) )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'A':
                if ( state == 2 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'D':
                if ( (state == 3) || (state == 9) )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'R':
                if ( state == 5 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case '_':
                if ( state == 6 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'N':
                if ( state == 8 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            default:
                break;
        }
        headerSize++;
        if ( state == 10 )
        {
            break;
        }
    }
    inputFile.close();
    return headerSize;
}

void readSIGPROCHeader(const std::uint64_t headerSize, Observation & observation, const std::string & inputFilename, const unsigned int subbands)
{
    std::uint64_t bytesRead = 0;
    std::uint8_t state = 0;
    std::ifstream inputFile;
    // Temporary variables
    char buffer = '\0';
    char * temp32 = new char [4];
    char * temp64 = new char [8];
    double tsamp = 0.0;
    unsigned int nsamples = 0;
    double fch1 = 0.0;
    double foff = 0.0;
    unsigned int nchans = 0;


    inputFile.open(inputFilename.c_str(), std::ios::binary);
    inputFile.exceptions(std::ifstream::failbit);
    if ( !inputFile )
    {
        throw FileError("ERROR: impossible to open SIGPROC file \"" + inputFilename + "\".");
    }
    while ( inputFile.get(buffer) )
    {
        switch ( buffer )
        {
            case 't':
                if ( state == 0 )
                {
                    state = 1;
                }
                break;
            case 's':
                if ( (state == 1) || (state == 5) )
                {
                    state++;
                }
                else if ( state == 11 )
                {
                    // nsamples : int
                    inputFile.read(temp32, 4);
                    nsamples = *(reinterpret_cast<unsigned int *>(temp32));
                    bytesRead += 4;
                    state = 0;
                }
                else if ( state == 20 )
                {
                    // nchans : int
                    inputFile.read(temp32, 4);
                    nchans = *(reinterpret_cast<unsigned int *>(temp32));
                    bytesRead += 4;
                    state = 0;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'a':
                if ( (state == 2) || (state == 6) || (state == 18) )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'm':
                if ( (state == 3) || (state == 7) )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'p':
                if ( state == 4 )
                {
                    // tsamp : double
                    inputFile.read(temp64, 8);
                    tsamp = *(reinterpret_cast<double *>(temp64));
                    bytesRead += 8;
                    state = 0;
                }
                else if ( state == 8 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'n':
                if ( state == 0 )
                {
                    state = 5;
                }
                else if ( state == 19 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'l':
                if ( state == 9 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'e':
                if ( state == 10 )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'f':
                if ( state == 0 )
                {
                    state = 12;
                }
                else if ( state == 15 )
                {
                    state++;
                }
                else if ( state == 16 )
                {
                    // foff : double
                    inputFile.read(temp64, 8);
                    foff = *(reinterpret_cast<double *>(temp64));
                    bytesRead += 8;
                    state = 0;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'c':
                if ( state == 12 )
                {
                    state++;
                }
                else if ( state == 5 )
                {
                    state = 17;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'h':
                if ( (state == 13) || (state == 17) )
                {
                    state++;
                }
                else
                {
                    state = 0;
                }
                break;
            case '1':
                if ( state == 14 )
                {
                    // fch1 : double
                    inputFile.read(temp64, 8);
                    fch1 = *(reinterpret_cast<double *>(temp64));
                    bytesRead += 8;
                    state = 0;
                }
                else
                {
                    state = 0;
                }
                break;
            case 'o':
                if ( state == 12 )
                {
                    state = 15;
                }
                else
                {
                    state = 0;
                }
                break;
            default:
                break;
        }
        bytesRead++;
        if ( bytesRead >= headerSize )
        {
            break;
        }
    }
    inputFile.close();
    observation.setNrSamplesPerBatch(nsamples);
    observation.setSamplingTime(tsamp);
    observation.setFrequencyRange(subbands, nchans, fch1 + (foff * (nchans - 1)), -foff);
}

#ifdef HAVE_PSRDADA
void readPSRDADAHeader(Observation &observation, dada_hdu_t &ringBuffer)
{
    // Staging variables for the header elements
    unsigned int uintValue = 0;
    float floatValue[2] = {0.0f, 0.0f};
    // Header string
    uint64_t headerBytes = 0;
    char *header = 0;

    header = ipcbuf_get_next_read(ringBuffer.header_block, &headerBytes);
    if ((header == 0) || (headerBytes == 0))
    {
        throw RingBufferError("ERROR: impossible to read the PSRDADA header");
    }
    ascii_header_get(header, "SAMPLES_PER_BATCH", "%d", &uintValue);
    observation.setNrSamplesPerBatch(uintValue);
    ascii_header_get(header, "NCHAN", "%d", &uintValue);
    ascii_header_get(header, "MIN_FREQUENCY", "%f", &floatValue[0]);
    ascii_header_get(header, "CHANNEL_BANDWIDTH", "%f", &floatValue[1]);
    observation.setFrequencyRange(observation.getNrSubbands(), uintValue, floatValue[0], floatValue[1]);
    ascii_header_get(header, "TSAMP", "%f", &floatValue[0]);
    observation.setSamplingTime(floatValue[0]);
    if (ipcbuf_mark_cleared(ringBuffer.header_block) < 0)
    {
        throw RingBufferError("ERROR: impossible to mark the PSRDADA header as cleared");
    }
}
#endif // HAVE_PSRDADA

} // namespace AstroData
