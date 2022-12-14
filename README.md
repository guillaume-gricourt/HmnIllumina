# HmnIllumina

[![Github Version](https://img.shields.io/github/v/release/guillaume-gricourt/HmnIllumina?display_name=tag&sort=semver)](version) [![Conda Release](https://img.shields.io/conda/vn/bioconda/hmnillumina.svg)](https://anaconda.org/bioconda/hmnillumina)  
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black) [![GitHub Super-Linter](https://github.com/guillaume-gricourt/HmnIllumina/workflows/Tests/badge.svg)](https://github.com/marketplace/actions/super-linter)  
[![DOI](https://zenodo.org/badge/577830976.svg)](https://zenodo.org/badge/latestdoi/577830976)  

Parse `InterOp` folder from Illumina run to keep useful informations.

## Install

```sh
conda install -n bioconda hmnillumina
```

## Usage

```sh
HmnIllumina \
  -i/--input <FOLDER> \
  -o/--output <FILE>
```

The `--input` argument refers to an output directory produced by an Illumina sequencer named like `20200101_M0000_*` for a Miseq run.
The directory must contains:
* a `RunInfo.xml` file.
* a `RunParameters.xml` file.
* an `InterOp` directory with at least these files: `ErrorMetricsOut.bin`,  `IndexMetricsOut.bin`,  `QMetricsOut.bin`,  `TileMetricsOut.bin`.

The `--output` argument refers to a JSON file.  
Undefined values are denoted by `""` or `"NA"`.

## Test

*pytest* is required:
```sh
make test
```

## Built with these main libraries

* [interop](https://github.com/illumina/interop) - InterOp library
* [rapidjson](https://github.com/tencent/rapidjson) - Json library
