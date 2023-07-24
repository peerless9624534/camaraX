#!/bin/bash

rm out/*
cd main;make clean
cd ../daemon/;make clean
cd ../reset_key/;make clean
cd ../ota/;make clean
cd ..