#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/lib:

# To be used on lxplus
# This is for setting up HSG7 ROOT, which include commonly used custom classes and some RooFit improvements. Users can also setup their preferred version of ROOT
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/Gcc/gcc493_x86_64_slc6/slc6/gcc49/setup.sh
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-HiggsComb-x86_64-slc6-gcc49-opt/bin/thisroot.sh

# Allow XMLReader to be executed everywhere
export _BIN_PATH=${PWD}/exe
export PATH=${_BIN_PATH}:${PATH}
