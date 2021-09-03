![ci workflow](https://github.com/jfouret/cppbio/actions/workflows/c-cpp.yml/badge.svg)

# CppBio

## Motivation

The purpose of building this set of libraries is pedagogic.

## Build the libraries

```
make lib
```

## Use the libraries

header files are in `./lib/*.hpp`

static libraires are in `./build/lib/*.a`

## Make the documentation

```
make doc
firefoc doc/html/index.html
```

## Test the libraries

You need cppunit with headers installed on your system to run the tests.

On ubuntu run `sudo apt-get install libcppunit-dev`

In addition to unit testing a memory leaks check is done with valgrind.

```
make test
```

## clean the repository

```
make clean
```

# Credits

## Build
- debian 
 
https://www.debian.org 

[**Licences**](https://www.debian.org/legal/licenses/)
- docker 

https://www.docker.com 

[**Docker Software End User License Agreement**](https://www.docker.com/legal/docker-software-end-user-license-agreement)
- g++ 
 
https://gcc.gnu.org/ 

[**GNU General Public License v3.0**](https://www.gnu.org/licenses/gpl-3.0.html)
- make 
 
https://www.gnu.org/software/make/ 

[**GNU General Public License v3.0**](https://www.gnu.org/licenses/gpl-3.0.html)

## Test
- CppUnit 

https://sourceforge.net/p/cppunit/wiki/Home/

[**GNU Lesser General Public License v2.1**](https://www.gnu.org/licenses/lgpl-2.1.html).
- valgring 

https://valgrind.org/ 

[**GNU General Public License v2.0**](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

# License

This project is licensed under [**GNU General Public License v3.0**](./LICENSE)
