#!/bin/sh
set -ex
cd sam
./sam-make.sh
cd ..
cd plugins
./plugins-make.sh
cd ..
./srv-make.sh
./cli-make.sh
