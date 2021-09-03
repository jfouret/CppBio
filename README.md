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

CppUnit is licensed under LGPL v2.1, and the full license text can be found on https://www.gnu.org/licenses/lgpl-2.1.html.

# License

This project is licensed under [**GNU General Public License v3.0**](./LICENSE)
