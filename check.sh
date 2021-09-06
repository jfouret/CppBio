#!/usr/bin/env bash

g++ -dumpfullversion | awk '($1+0)<10.0{ print "minimum required version for g++ is 10.0 you have "$1 ; exit 1}'

