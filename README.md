# CoreGen Integrated Developement Environment

[![Documentation](https://codedocs.xyz/opensocsysarch/CoreGen.svg)](https://codedocs.xyz/opensocsysarch/CoreGen/)
[![GitHub license](https://img.shields.io/badge/license-APACHE2-blue.svg)](https://raw.githubusercontent.com/opensocsysarch/CoreGen/master/LICENSE)



## Getting Started

## Prerequisites
* C++11 Compiler (LLVM/Clang and GCC are tested)
* CMake 3.4.3+
* LLVM 5.X

## Building

The majority of the CoreGen library and tool dependencies are contained
within the source tree.  The only outside library dependencies are met 
with the libstdc++ (from GCC or LLVM) and the LLVM libraries (for 
StoneCutter support).  As a result, building CoreGen is generally 
considered to be fairly simple.  The current tested platforms 
are listed below, each with their respective build options.  
Generally speaking, CoreGen is built using the following steps:

1. Clone the CoreGen repository.  There are currently no git dependencies
1. Create a ``build`` directory within the CoreGen source tree (and change to that directory)
1. Execute cmake to generate the target-specific makefiles
1. Execute the build


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
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main"
sudo apt-get update
```
1.  Install the system dependencies:
```
sudo apt-get install -y libllvm-6.0-ocaml-dev libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime clang-6.0 clang-tools-6.0 clang-6.0-doc libclang-common-6.0-dev libclang-6.0-dev libclang1-6.0 clang-format-6.0 python-clang-6.0 re2c lua5.3 liblua5.3-dev
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
cmake -DLLVM_DIR=/usr/lib/llvm-6.0/cmake ../
```
5. Execute the build
```
make
```


#### Ubuntu 18.04 (Bionic)

1.  Install the necessary apt repositories
```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-6.0 main"
sudo apt-get update
```
2.  Install the system dependencies:
```
sudo apt-get install -y clang-6.0 lldb-6.0 lld-6.0 libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime re2c lua5.3 liblua5.3-dev
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
cmake -DLLVM_DIR=/usr/lib/llvm-6.0/cmake ../
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
sudo yum -y install cmake centos-release-scl texlive texlive-*.noarch llvm-static llvm-toolset-7-llvm-devel llvm-toolset-7-build llvm-toolset-7-cmake llvm-toolset-7-lldb-devel llvm-toolset-7-libomp-devel llvm-toolset-7-clang-devel llvm-toolset-7-llvm-static zlib-devel re2c lua lua-devel devtoolset-7
```
2. [Optional] For those wishing to build CentOS7 RPMS, you also need the following packages.
```
sudo yum install rpm-build rpmdevtools
```
3. Enable the SCL LLVM environment (this will create a fresh bash shell with the correct paths)
``scl enable llvm-toolset-7 devtoolset-7 bash``
4. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
5. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
6. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/opt/rh/llvm-toolset-7/root/usr/lib64/cmake/llvm ../
```
7. Execute the build
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
brew cask install mactex
brew install cmake
brew install llvm@5
brew install re2c
brew install lua
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
cmake -DLLVM_DIR=/usr/local/opt/llvm\@5/lib/cmake/llvm ../
```
5. Execute the build
```
make
```

#### Arch Linux

1.  Install the system dependencies:
```
sudo pacman -S git cmake make llvm6 llvm6-libs clang
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
cmake -DLLVM_DIR=/usr/lib/cmake/llvm ../
```
5. Execute the build
```
make
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
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_DanglingRegionPass\_TESTING=ON : Enable DAG's individual pass test harness for the DanglingRegionPass
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNFAIL\_TESTING=ON : Enable DAG's individual pass test harness for known fail tests
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNFAIL\_ASP\_TESTING=ON : Enable DAG's individual ASP pass test harness for known fail tests
* -DBUILD\_COREGEN\_PLUGIN\_TESTING=ON : Enable plugin test harness
* -DBUILD\_COREGEN\_YAML\_TESTING=ON : Enable yaml test harness
* -DBUILD\_COREGEN\_YAML\_READER\_TESTING=ON : Enable yaml reader test harness
* -DBUILD\_COREGEN\_YAML\_READER\_MISSING\_DATA\_TESTING=ON : Enable yaml reader test harness for yaml with known missing data
* -DBUILD\_COREGEN\_YAML\_WRITER\_TESTING=ON : Enable yaml writer test harness
* -DBUILD\_COREGEN\_STONECUTTER\_TESTING=ON : Enables the StoneCutter test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CLI\_TESTING=ON : Enables the StoneCutter CLI test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PARSER\_TESTING=ON : Enables the StoneCutter parser test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PARSER\_KNOWNFAIL\_TESTING=ON : Enables the StoneCutter parser known fail test harness
* -DBUILD\_COREGEN\_STONECUTTER\_PARSER\_KNOWNPASS\_MULTIFILE\_TESTING=ON : Enables the StoneCutter parser multi-file test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CODEGEN\_TESTING=ON : Enables the StoneCutter codegen test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CODEGEN\_KNOWNPASS\_TESTING=ON : Enables the StoneCutter codegen knownpass test harness
* -DBUILD\_COREGEN\_STONECUTTER\_CODEGEN\_KNOWNFAIL\_TESTING=ON : Enables the StoneCutter codegen knownfail test harness
* -DBUILD\_COREGEN\_CODEGEN\_TESTING=ON : Enables the CodeGen test harness
* -DBUILD\_COREGEN\_CODEGEN\_STONECUTTERIMPL\_TESTING=ON : Enables the CodeGen StoneCutter implementation test harness

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
* *Frank Conlon* - *Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)

## Acknowledgements
* None at this time
