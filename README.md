# CoreGen Integrated Developement Environment

[![Documentation](https://codedocs.xyz/opensocsysarch/CoreGen.svg)](https://codedocs.xyz/opensocsysarch/CoreGen/)
[![GitHub license](https://img.shields.io/badge/license-APACHE2-blue.svg)](https://raw.githubusercontent.com/opensocsysarch/CoreGen/master/LICENSE)



## Getting Started

## Prerequisites
* C++14 Compiler (LLVM/Clang and GCC are tested)
* CMake 3.4.3+
* LLVM 7.X or LLVM 8.X
* Curl
* Doxygen (for docs and install)

## Building

The majority of the CoreGen library and tool dependencies are contained
within the source tree.  The only outside library dependencies are met 
with the libstdc++ (from GCC or LLVM) and the LLVM libraries (for 
StoneCutter support).  As a result, building CoreGen is generally 
considered to be fairly simple.  The current tested platforms 
are listed below, each with their respective build options.  
Generally speaking, CoreGen is built using the following steps:

1. Clone the CoreGen repository.  There are currently no git dependencies
2. Create a ``build`` directory within the CoreGen source tree (and change to that directory)
3. Execute cmake to generate the target-specific makefiles
4. Execute the build


### Building with Clang/LLVM

Building CoreGen with Clang/LLVM can be done by setting the CC and CXX environment variables.

1. Override default compiler
```
export CC=clang
export CXX=clang++
```

### Ubuntu

We currently support building and installing on LTS versions of Ubuntu.  While it 
is certainly possible to build/install on intermediate release and/or development 
versions, we do not currently test these scenarios.

#### Ubuntu 16.04 (Xenial)

1.  Install the necessary apt repositories
```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 8
```
2.  Install the system dependencies:
```
sudo apt-get install -y libllvm-8-ocaml-dev libllvm8 llvm-8 llvm-8-dev llvm-8-doc llvm-8-examples llvm-8-runtime clang-8 clang-tools-8 clang-8-doc libclang-common-8-dev libclang-8-dev libclang1-8 clang-format-8 python-clang-8 zlib1g zlib1g-dev bison libcurl4-gnutls-dev zlib1g-dev
```
3. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
4. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
5. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake ../
```
6. Execute the build
```
make
```


#### Ubuntu 18.04 (Bionic)

1.  Install the necessary apt repositories
```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 8
```
2.  Install the system dependencies:
```
sudo apt-get install -y clang-8 lldb-8 lld-8 libllvm8 llvm-8 llvm-8-dev llvm-8-doc llvm-8-examples llvm-8-runtime python-dev libcurl4 libcurl4-gnutls-dev zlib1g-dev
```
3. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
4. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
5. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake ../
```
6. Execute the build
```
make
```

#### Ubuntu 20.04 (Focal)

1.  Install the necessary apt repositories
```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 8
```
2.  Install the system dependencies:
```
sudo apt-get install -y clang-8 lldb-8 lld-8 libllvm8 llvm-8 llvm-8-dev llvm-8-doc llvm-8-examples llvm-8-runtime python-dev libcurl4 libcurl4-gnutls-dev
```
3. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
4. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
5. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-8/cmake ../
```
6. Execute the build
```
make
```


### Centos7

Centos7 (CentOS 7.4, 1704) has somewhat of an older toolchain.  As a result, 
the build requires some rather unique system packages.  Utilize the following 
steps on vanilla CentOS 7.4 systems.

1.  Install the system dependencies:
```
sudo yum -y install cmake centos-release-scl texlive texlive-*.noarch llvm-static llvm-toolset-7-llvm-devel llvm-toolset-7-build llvm-toolset-7-cmake llvm-toolset-7-lldb-devel llvm-toolset-7-libomp-devel llvm-toolset-7-clang-devel llvm-toolset-7-llvm-static zlib-devel devtoolset-8 python-devel bison libcurl-devel
```
2. [Optional] For those wishing to build CentOS7 RPMS, you also need the following packages.
```
sudo yum install rpm-build rpmdevtools
```
3. Enable the SCL LLVM environment (this will create a fresh bash shell with the correct paths)
``
scl enable llvm-toolset-7 devtoolset-8 bash
``
4. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
5. Setup your build tree
```
cd CoreGen
mkdir build
cd build
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/rh/llvm-toolset-7.0/root/lib64/
```
6. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/opt/rh/llvm-toolset-7.0/root/usr/lib64/cmake/llvm ../
```
7. Execute the build
```
make
```

### Centos8

Centos8 has a native toolchain that is much more adept at building modern C++ applications.
Utilize the following steps on vanilla CentOS 8 systems.

1.  Install the system dependencies:
```
sudo yum -y install cmake gcc-toolset-9 texlive texlive-*.noarch bison libcurl-devel zlib-devel llvm llvm-toolset llvm-libs llvm-devel llvm-static graphviz ncurses-*
```
2. [Optional] For those wishing to build Centos8 RPMS, you also need the following packages.
```
sudo yum install rpm-build rpmdevtools
```
3. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
4. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
5. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib64/cmake/llvm ../
```
6. Execute the build
```
make
```


### Darwin (OSX)

The Darwin (OSX) build is currently tested on 10.13.6.  However, these
build steps are also known to work on older versions of Darwin providing 
that the necessary packages are installed.  For this tutorial, we use 
the ``HomeBrew`` package manager in order to fulfill these dependencies.

1. Install the system dependencies (assuming HomeBrew is installed):
```
brew install --cask mactex
brew install cmake
brew install llvm@8
brew install pkg-config
```
2. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
3. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
4. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/local/opt/llvm\@8/lib/cmake/llvm ../
```
5. Execute the build
```
make
```

## Post Installation
Once the CoreGen infrastructure has been installed, it is advisable that you initialize the CoreGen archive.  
The CoreGen archive provides LLVM compiler templates and libraries that will be made available to all 
CoreGen-derived projects without repeatedly downloading large packages.  The contents of the 
archive is determined by a master YAML file.  The master 
YAML file is located in the installation directory at `~/INSTALL_PREFIX/archive/master.yaml`.  Initializing 
the archive will initiate downloads from a number of public source code repositories, including LLVM.  Note 
that anyone initializing the archive must ensure that sufficient disk space is available and they 
have the necessary permissions to populate the archive directory.  All the archive contents will be downloaded 
to `~/INSTALL_PREFIX/archive/`.  Initializing the archive can be performed using the CGCLI tool as follows:

```
cgcli --init /path/to/installation/archive/master.yaml
```

## CMake Build Options

### Standard Build Options
* -DCMAKE\_BUILD\_TYPE=Release : Builds a release build with optimization (Default)
* -DCMAKE\_BUILD\_TYPE=Debug : Builds with debugging options (-O0 -g, etc)
* -DCMAKE\_INSTALL\_PREFIX=/path/to/install : Forces alternate install locations
* -DLLVM\_DIR=/path/to/llvm/cmake/scripts : Sets the path for the LLVM cmake scripts.  This must be set for the build to be successful.  This is generally difficult to derive automatically.

### Documentation Options
* -DBUILD\_DOCUMENTATION=ON  : Enables Doxygenized documentaition.  ``make doc`` will build the documentation (requires Doxygen to be installed)

### Developer Build Options
* -DYAML\_PATH=/path/to/alternate/yaml-cpp : Set an alternate yaml-cpp library path
* -DCOREGEN\_BUILD\_RPM=ON : Enable RPM builds (CentOS)
* -DCOREGEN\_BUILD\_DEB=ON : Enable DEB builds (Ubuntu)
* -DCOREGEN\_BUILD\_DMG=ON : Enable DMG builds (Darwin)
* -DCOREGEN\_BUILD\_TGZ=ON : Enable TGZ builds (generic)

### Debugging Options
* -DCOREGEN\_MEMORY\_SANITY=ON : Enables Clang's memory sanitizer testing.  You MUST be using Clang as your target C++ compiler!!

### Test Options
* -DBUILD\_ALL\_TESTING=ON : Enable ALL unit tests for CoreGen
* -DBUILD\_COREGEN\_CGCLI\_TESTING=ON : Enable CGCLI test harness
* -DBUILD\_COREGEN\_CGCLI\_SPECDOC\_TESTING=ON : Enable CGCLI SpecDoc pass test harness
* -DBUILD\_COREGEN\_DAG\_TESTING=ON : Enable IR DAG test harness
* -DBUILD\_COREGEN\_DAG\_DOT\_TESTING=ON : Enable DAG's DOT file test harness
* -DBUILD\_COREGEN\_DAG\_FULLPASS\_TESTING=ON : Enable DAG's full pass test harness
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_TESTING=ON : Enable DAG's individual pass test harness
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_TESTING=ON : Enable DAG's individual pass test harness for known correct tests
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_STATSPASS\_TESTING=ON : Enable DAG's individual pass test harness for the StatsPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_MULTSOCPASS\_TESTING=ON : Enable DAG's individual pass test harness for the MultSoCPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_DANGLINGREGIONPASS\_TESTING=ON : Enable DAG's individual pass test harness for the DanglingRegionPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_DANGLINGNODEPASS\_TESTING=ON : Enable DAG's individual pass test harness for the DanglingNodePass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_ENCODINGCOLLISIONPASS\_TESTING=ON : Enable DAG's individual pass test harness for the EncodingCollisionPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_REGIDXPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegIdxPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_CACHELEVELPASS\_TESTING=ON : Enable DAG's individual pass test harness for the CacheLevelPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_REGSAFETYPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_SPADSAFETYPASS\_TESTING=ON : Enable DAG's individual pass test harness for the SpadSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_ICACHECHECKERPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_L1SHAREDPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_REGFIELDPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_CORESAFETYPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_COMMSAFETYPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_MANDATORYFIELDPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_ENCODINGGAPPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_PINSTSAFETYPASS\_TESTING=ON : Enable DAG's individual pass test harness for the RegSafetyPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNFAIL\_TESTING=ON : Enable DAG's individual pass test harness for known fail tests
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNFAIL\_ASP\_TESTING=ON : Enable DAG's individual ASP pass test harness for known fail tests
* -DBUILD\_COREGEN\_DAG\_DELETENODE\_TESTING=ON : Enable DAG's node deletion test harness
* -DBUILD\_COREGEN\_PLUGIN\_TESTING=ON : Enable plugin test harness
* -DBUILD\_COREGEN\_YAML\_TESTING=ON : Enable yaml test harness
* -DBUILD\_COREGEN\_YAML\_READER\_TESTING=ON : Enable yaml reader test harness
* -DBUILD\_COREGEN\_YAML\_READER\_MISSING\_DATA\_TESTING=ON : Enable yaml reader test harness for yaml with known missing data
* -DBUILD\_COREGEN\_YAML\_READER\_INDENT\_FAIL\_TESTING=ON : Enable yaml reader test harness for yaml with known issues in indentations
* -DBUILD\_COREGEN\_YAML\_READER\_CONV\_FAIL\_TESTING=ON : Enable yaml reader test harness for yaml with known issues in type conversions 
* -DBUILD\_COREGEN\_YAML\_WRITER\_TESTING=ON : Enable yaml writer test harness
* -DBUILD\_COREGEN\_STONECUTTER\_TESTING=ON : Enables the StoneCutter test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CLI\_TESTING=ON : Enables the StoneCutter CLI test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PIPELINER\_TESTING=ON : Enables the StoneCutter pipeliner test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PARSER\_TESTING=ON : Enables the StoneCutter parser test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PARSER\_KNOWNFAIL\_TESTING=ON : Enables the StoneCutter parser known fail test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PARSER\_KNOWNPASS\_MULTIFILE\_TESTING=ON : Enables the StoneCutter parser multi-file test harness
* -DBUILD\_COREGEN\_STONECUTTER\_SIGMAP\_TESTING=ON : Enables the StoneCutter SigMap pass testing
* -DBUILD\_COREGEN\_STONECUTTER\_CODEGEN\_TESTING=ON : Enables the StoneCutter codegen test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CODEGEN\_KNOWNPASS\_TESTING=ON : Enables the StoneCutter codegen knownpass test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CODEGEN\_KNOWNFAIL\_TESTING=ON : Enables the StoneCutter codegen knownfail test harness
* -DBUILD\_COREGEN\_STONECUTTER\_SCSIGV\_TESTING=ON : Enables SCSigV sigmap test harness
* -DBUILD\_COREGEN\_STONECUTTER\_SCSIGV\_KNOWNPASS\_TESTING=ON : Enables SCSigV sigmap knowpass test harness
* -DBUILD\_COREGEN\_STONECUTTER\_SCSIGV\_KNOWNFAIL\_TESTING=ON : Enables SCSigV sigmap knowfail test harness
* -DBUILD\_COREGEN\_CODEGEN\_TESTING=ON : Enables the CodeGen test harness
* -DBUILD\_COREGEN\_CODEGEN\_STONECUTTERIMPL\_TESTING=ON : Enables the CodeGen StoneCutter implementation test harness
* -DBUILD\_COREGEN\_ARCHIVE\_TESTING=ON : Enables teh archive library test harness
* -DBUILD\_COREGEN\_POAR\_TESTING=ON : Enables all the POAR tests
* -DBUILD\_COREGEN\_POAR\_CLI\_TESTING=ON : Enable the POAR CLI tests
* -DBUILD\_COREGEN\_POAR\_TEXT\_TESTING=ON : Enable the POAR text output tests
* -DBUILD\_COREGEN\_POAR\_YAML\_TESTING=ON : Enable the POAR YAML output tests
* -DBUILD\_COREGEN\_POAR\_XML\_TESTING=ON : Enable the POAR XML output tests
* -DBUILD\_COREGEN\_POAR\_LATEX\_TESTING=ON : Enable the POAR LaTeX output tests
* -DBUILD\_COREGEN\_POAR\_SIGMAP\_TESTING=ON : Enable the POAR sigmap output tests
* -DBUILD\_COREGEN\_DHDT\_TESTING=ON : Enables all the DHDT tests
* -DBUILD\_COREGEN\_DHDT\_CLI\_TESTING=ON : Enables the DHDT CLI tests
* -DBUILD\_COREGEN\_DHDT\_POWER\_TESTING=ON : Enables the DHDT Power tests
* -DBUILD\_COREGEN\_DHDT\_HAZARD\_TESTING=ON : Enables the DHDT Hazard tests

## Contributing
We welcome outside contributions from corporate, acaddemic and individual developers.  However, 
there are a number of fundamental ground rules that you must adhere to in order to participate.  These 
rules are outlined as follows:

* All code must adhere to the existing C++ coding style.  While we are somewhat flexible in basic style, you will 
adhere to what is currently in place.  This includes camel case C++ methods and inline comments.  Uncommented, 
complicated algorithmic constructs will be rejected.
* We support compilaton and adherence to C++11 methods.  We do not currently accept C++14 and beyond.
* All new methods and variables contained within public, private and protected class methods must be commented 
using the existing Doxygen-style formatting.  All new classes must also include Doxygen blocks in the new header 
files.  Any pull requests that lack these features will be rejected.
* All changes to functionality and/or the API infrastructure must be accompanied by complementary tests
* All external pull requests **must** target the *devel* branch.  No external pull requests will be accepted 
to the master branch.
* All external pull requests must contain sufficient documentation in the pull request comments in order to 
be accepted.
* All external pull requests that update the CoreGen IR specification or the StoneCutter language specification 
(et al. syntax) must contain complementary pull requests for the CoreGen IR spec or the 
StoneCutter language spec.
* All pull requests will be reviewed by the core development staff.  Any necessary changes will be marked
in the respective pull request comments.  All pull requests will be tested against in the Tactical 
Computing Laboratories development infrastructure.  This includes tests against all supported platforms.  
Any failures in the test infrastructure will be noted in the pull request comments.

## Testing
All tests must be formatted using standard CMake directives.  See the existing CMakeLists for 
examples on how these tests are to be formatted.  Any updates to the test infrastructure (new test 
directory structure) must include complementary, unique directives as outlined in the Test Options 
section above.  Any new directives must be documented in this Readme file within the respective pull 
request.

## License
CoreGen is licensed under an Apache-style license - see the [LICENSE](LICENSE) file for details

## External Packages
* CoreGen makes use of the yaml-cpp (https://github.com/jbeder/yaml-cpp) library for its internal IR parsing.  yaml-cpp carries an MIT license.
* CoreGen's StoneCutter infrastructure makes use of the LLVM compiler (http://www.llvm.org).  LLVM carries a 
UIUC, BSD-style license.

## Authors
* *John Leidel* - *Chief Scientist* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *David Donofrio* - *Chief Hardware Architect* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *Ryan Kabrick* - *Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *Frank Conlon* - *Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)

## Acknowledgements
* None at this time
