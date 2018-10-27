# CoreGen Integrated Developement Environment

[![Documentation](https://codedocs.xyz/opensocsysarch/CoreGen.svg)](https://codedocs.xyz/opensocsysarch/CoreGen/)

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
sudo apt-get install -y libllvm-6.0-ocaml-dev libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime clang-6.0 clang-tools-6.0 clang-6.0-doc libclang-common-6.0-dev libclang-6.0-dev libclang1-6.0 clang-format-6.0 python-clang-6.0
```
1. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
1. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
1. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-6.0/cmake ../
```
1. Execute the build
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
1.  Install the system dependencies:
```
sudo apt-get install -y clang-6.0 lldb-6.0 lld-6.0 libllvm6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-doc llvm-6.0-examples llvm-6.0-runtime
```
1. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
1. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
1. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/lib/llvm-6.0/cmake ../
```
1. Execute the build
```
make
```


### Centos7

Centos7 (CentOS 7.4, 1704) has somewhat of an older toolchain.  As a result, 
the build requires some rather unique system packages.  Utilize the following 
steps on vanilla CentOS 7.4 systems.

1.  Install the system dependencies:
```
sudo yum -y install cmake centos-release-scl texlive texlive-*.noarch llvm-static llvm-toolset-7-llvm-devel llvm-toolset-7-build llvm-toolset-7-cmake llvm-toolset-7-lldb-devel llvm-toolset-7-libomp-devel llvm-toolset-7-clang-devel llvm-toolset-7-llvm-static zlib-devel
```
1. [Optional] For those wishing to build CentOS7 RPMS, you also need the following packages.
```
sudo yum install rpm-build rpmdevtools
```
1. Enable the SCL LLVM environment (this will create a fresh bash shell with the correct paths)
``scl enable llvm-toolset-7 bash``
1. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
1. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
1. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/opt/rh/llvm-toolset-7/root/usr/lib64/cmake/llvm ../
```
1. Execute the build
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
```
1. Clone the CoreGen repository
```
git clone https://github.com/opensocsysarch/CoreGen.git
```
1. Setup your build tree
```
cd CoreGen
mkdir build
cd build
```
1. Execute CMake to generate the makefiles
```
cmake -DLLVM_DIR=/usr/local/opt/llvm\@5/lib/cmake/llvm ../
```
1. Execute the build
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
* -DBUILD\_DOCUMENTATION=ON  : Enables Doxygenized documentaition.  ``make docs`` will build the documentation (requires Doxygen to be installed)

### Developer Build Options
* -DYAML\_PATH=/path/to/alternate/yaml-cpp : Set an alternate yaml-cpp library path
* -DCOREGEN\_BUILD\_RPM=ON : Enable RPM builds (CentOS)
* -DCOREGEN\_BUILD\_DEB=ON : Enable DEB builds (Ubuntu)
* -DCOREGEN\_BUILD\_DMG=ON : Enable DMG builds (Darwin)
* -DCOREGEN\_BUILD\_TGZ=ON : Enable TGZ builds (generic)

### Debugging Options
* -DBUILD\_COREGEN\_MEMORY\_SANITY=ON : Enables Clang's memory sanitizer testing.  You MUST be using Clang as your target C++ compiler!!

### Test Options
* -DBUILD\_ALL\_TESTING=ON : Enable ALL unit tests for CoreGen
* -DBUILD\_COREGEN\_CGCLI\_TESTING=ON : Enable CGCLI test harness
* -DBUILD\_COREGEN\_CGCLI\_SPECDOC\_TESTING=ON : Enable CGCLI SpecDoc pass test harness
* -DBUILD\_COREGEN\_DAG\_TESTING=ON : Enable IR DAG test harness
* -DBUILD\_COREGEN\_DAG\_DOT\_TESTING=ON : Enable DAG's DOT file test harness
* -DBUILD\_COREGEN\_DAG\_FULLPASS\_TESTING=ON : Enable DAG's full pass test harness
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_TESTING=ON : Enable DAG's individual pass test harness
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNPASS\_TESTING=ON : Enable DAG's individual pass test harness for known correct tests
* -DBUILD\_COREGEN\_DAG\_INDIVIDUALPASS\_KNOWNFAIL\_TESTING=ON : Enable DAG's individual pass test harness for known fail tests
* -DBUILD\_COREGEN\_PLUGIN\_TESTING=ON : Enable plugin test harness
* -DBUILD\_COREGEN\_YAML\_TESTING=ON : Enable yaml test harness
* -DBUILD\_COREGEN\_YAML\_READER\_TESTING=ON : Enable yaml reader test harness
* -DBUILD\_COREGEN\_YAML\_READER\_MISSING\_DATA\_TESTING=ON : Enable yaml reader test harness for yaml with known missing data
* -DBUILD\_COREGEN\_YAML\_WRITER\_TESTING=ON : Enable yaml writer test harness

## Contributing

## Testing

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
