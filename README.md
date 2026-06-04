# JHCli

Command-line interface to fetch the latest COVID-19 data from Johns Hopkins University.

## Overview

JHCli is a C++ CLI tool that retrieves and displays COVID-19 pandemic statistics from the Johns Hopkins University CSSE data repository. It provides quick access to case counts, deaths, and recovery data by region.

## Features

- Fetch latest COVID-19 statistics from Johns Hopkins CSSE
- Query data by country or region
- Command-line interface for easy scripting and automation
- Caches data locally to reduce API calls

## Prerequisites

- C++20 compiler
- CMake 3.16+
- libcurl

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Run

```bash
./jhcli
```

## Tests

```bash
cd build
ctest
```

## License

CC0 1.0 Universal - see [LICENSE](LICENSE)

## Author

**Rich Nistuk** - [rnistuk](https://github.com/rnistuk)
