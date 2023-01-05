# HmnIllumina

[![Github Version](https://img.shields.io/github/v/release/guillaume-gricourt/HmnIllumina?display_name=tag&sort=semver)](version) [![Conda Release](https://img.shields.io/conda/vn/bioconda/hmnillumina.svg)](https://anaconda.org/bioconda/hmnillumina)  
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black) [![GitHub Super-Linter](https://github.com/guillaume-gricourt/HmnIllumina/workflows/Tests/badge.svg)](https://github.com/marketplace/actions/super-linter)  
[![License](https://img.shields.io/github/license/guillaume-gricourt/HmnIllumina)](license)

Parse `InterOp` folder from Illumina run to keep useful informations.

## Install

```sh
conda install -n bioconda hmnillumina
```

## Use

```sh
HmnIllumina \
  --input <FOLDER> \
  --output <FILE>
```

## Test

*pytest* is required:
```sh
make test
```

## Built with these main libraries

* [interop](https://github.com/illumina/interop) - InterOp library
* [rapidjson](https://github.com/tencent/rapidjson) - Json library

## Authors

* **Guillaume Gricourt**
