#!/bin/bash

cd ..
mkdir -p lib
cd lib

cmake ..
make -j9
