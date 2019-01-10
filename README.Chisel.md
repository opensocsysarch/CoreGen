# CoreGen Chisel Installation Guide

## Getting Started

This installation guide will provide users instructions on 
installing all of the necessary system depdendecies to utilize 
the Chisel HDL environment on your respective system.  This 
includes installation of Scala, Verilator and other system 
packages.

## Prerequisites
* Java JDK
* git
* make
* autoconf
* g++
* flex
* bison
* perl-doc (for Verilator help menu)

## Installation

### Ubuntu (16.04/18.04)

The Scala and Verilator (3.922) are not included in the default 
package repositories.  As a result, we are forced to utilize external 
package repositories for the Scala packages and build Verilator from 
source.

1. Install system packages
```
sudo apt-get install git make autoconf g++ flex bison perl-doc
```

2. Install SBT
```
echo "deb https://dl.bintray.com/sbt/debian /" | sudo tee -a /etc/apt/sources.list.d/sbt.list
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 642AC823
sudo apt-get update
sudo apt-get install sbt
```

3. Install Verilator 3.922 from source
```
git clone http://git.veripool.org/git/verilator
cd verilator
git pull
git checkout verilator_3_922
unset VERILATOR_ROOT
autoconf
./configure
make
sudo make install
```

### Centos7

The Scala and Verilator (3.922) are not included in the default 
package repositories.  As a result, we are forced to utilize external 
package repositories for the Scala packages and build Verilator from 
source.

1. Install the system packages
```
sudo yum install git make autoconf g++ flex bison perl-doc
```

2. Install SBT
```
curl https://bintray.com/sbt/rpm/rpm | sudo tee /etc/yum.repos.d/bintray-sbt-rpm.repo
sudo yum install sbt
```

3. Install Verilator 3.922 from source
```
git clone http://git.veripool.org/git/verilator
cd verilator
git pull
git checkout verilator_3_922
unset VERILATOR_ROOT
autoconf
./configure
make
sudo make install
```

### Darwin (OSX)

The Darwin (OSX) installation is currently tested on 10.13.6.  However, 
these build steps are also known to work on older versions of Darwin providing 
that the necessary packages are installed.  For this installation, we use the 
``HomeBrew`` package manager in order to fulfill all the necessary system 
dependencies.  

1. Install Scala and Verilator
```
brew install sbt verilator
```

## Authors
* *John Leidel* - *Chief Scientist* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *Frank Conlon* - *Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)

## Acknowledgements
* These instructions are derived from the Chisel installation guide located at: https://github.com/freechipsproject/chisel3/#installation
