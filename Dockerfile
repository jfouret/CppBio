FROM debian:stable-slim

LABEL version="1.0"
LABEL authors="Julien FOURET <julien.fouret.fr@gmail.com>"
LABEL description="Set of tools to build cpp projects"

RUN apt-get update ; apt-get upgrade -y

RUN apt-get install -y g++ make valgrind libboost-test-dev doxygen

RUN apt-get clean
