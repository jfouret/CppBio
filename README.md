# CppBio

![ci workflow](https://github.com/jfouret/cppbio/actions/workflows/c-cpp.yml/badge.svg)
![ci workflow](https://github.com/jfouret/cppbio/actions/workflows/docker-publish.yml/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/6e8947134e694e0f93628fce2a7f5071)](https://www.codacy.com/gh/jfouret/CppBio/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jfouret/CppBio&amp;utm_campaign=Badge_Grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/6e8947134e694e0f93628fce2a7f5071)](https://www.codacy.com/gh/jfouret/CppBio/dashboard?utm_source=github.com&utm_medium=referral&utm_content=jfouret/CppBio&utm_campaign=Badge_Coverage)

## Motivation

The purpose of building this set of libraries is pedagogic.

## Build the libraries

```shell
make lib
```

## Use the libraries

header files are in `./lib/*.hpp`

static libraires are in `./build/lib/*.a`

## Make the documentation

```shell
make doc
firefoc doc/html/index.html
```

## Test the libraries

You need cppunit with headers installed on your system to run the tests.

In addition to unit testing a memory leaks check is done with valgrind.

On ubuntu run 

```shell
sudo apt-get install libboost-test-dev valgrind
```

```shell
make test
```

## Run with logs

replace `SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_ERROR` by `SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG` or `SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE`

## clean the repository

```shell
make clean
```

## Credits

### Build

#### debian 
 
https://www.debian.org 

[**Licences**](https://www.debian.org/legal/licenses/)
#### docker 

https://www.docker.com 

[**Docker Software End User License Agreement**](https://www.docker.com/legal/docker-software-end-user-license-agreement)
#### g++ 
 
https://gcc.gnu.org/ 

[**GNU General Public License v3.0**](https://www.gnu.org/licenses/gpl-3.0.html)
#### make 
 
https://www.gnu.org/software/make/ 

[**GNU General Public License v3.0**](https://www.gnu.org/licenses/gpl-3.0.html)

### Test
#### Boost.Test library 

https://www.boost.org/doc/libs/1_77_0/libs/test/doc/html/index.html

[**Boost Software License, Version 1.0**](https://www.boost.org/LICENSE_1_0.txt).
#### valgring 

https://valgrind.org/ 

[**GNU General Public License v2.0**](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)

## License

This project is licensed under [**GNU General Public License v3.0**](./LICENSE)
