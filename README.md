# AstroData

Set of C++ classes to read data and generate fake data for the pulsar search and FRB pipeline.

# Installation

Set the `SOURCE_ROOT` environment variable to the location of the pipeline sourcode.
If this package is installed in `$HOME/Code/APERTIF/AstroData` this would be:

```bash
 $ export SOURCE_ROOT=$HOME/Code/APERTIF
```

Then build and test as follows:

```bash
 $ make
```

# Included classes

## ReadData.hpp

Data io functions:
 
 * *readZappedChannels* Zapped channels (excluded from computation)
 * *readIntegrationSteps* Integration steps
 * *readSIGPROC* SIGPROC data
 * *readLOFAR* LOFAR data
 * *readPSRDadaHeader* PSRDADA buffer
 * *readPSRDada* PSRDADA data

## Platform.hpp

Classes and readers for:

 * *paddingConf* Memory padding
 * *readPaddingConf* 
 * *vectorWidthConf* Vector unit width
 * readVectorWidthConf

## Observation.hpp

A class to hold physical observations parameters and search configuration.

## Generator.hpp

Generator for fake data, useful for for testing.

 * *generatePulsar* Generates a periodic single signal, not too relastic.
 * *generateSinglePulse* Generates a single pulse

## ColorMap

 * *getColorMap()* Returns an array of colors, used in the pulsar pipeline for plotting.

# License

Licensed under the Apache License, Version 2.0.
