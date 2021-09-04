FROM debian:stable-slim

LABEL version="1.0"
LABEL authors="Julien FOURET <julien.fouret.fr@gmail.com>"
LABEL description="Set of tools to build cpp projects"

RUN apt-get update \
 &&  apt-get install -y --no-install-recommends g++==4:10.2.1-1 make==4.3-4.1 valgrind==1:3.16.1-1 libboost-test-dev==1.74.0.3 doxygen==1.9.1-1 lcov==1.14-2 \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

RUN apt-get clean
