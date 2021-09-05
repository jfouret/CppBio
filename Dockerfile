FROM debian:stable-slim

LABEL version="1.0"
LABEL authors="Julien FOURET <julien.fouret.fr@gmail.com>"
LABEL description="Set of tools to build cpp projects"

RUN apt-get update \
 &&  apt-get install -y --no-install-recommends libspdlog-dev g++ make valgrind libboost-test-dev doxygen lcov curl ca-certificates \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

RUN apt-get clean
