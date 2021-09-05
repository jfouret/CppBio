#!/usr/bin/env bash

curl https://coverage.codacy.com/get.sh > dll_codacy_get.sh

bash dll_codacy_get.sh report -r build/coverage.info
