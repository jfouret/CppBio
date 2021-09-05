# Define the project-specific arborescence
SRC=src
SRC_LIB=lib
SRC_TEST=test
BUILD_DIR=build

# Define default make variables
CXX=g++
CXXFLAGS=-O0 -std=c++17 -g3 -Wall
RM=rm -f
LDLIBS=
LDFLAGS=

# Define the name of the libraries to be built
LIBS_NAME=seq

# Get the path 
LIBS_STATIC_PATH=$(addprefix ${BUILD_DIR}/lib/,$(addsuffix .a,$(LIBS_NAME)))
LIBS_TEST_PATH=$(addprefix ${BUILD_DIR}/test/,$(LIBS_NAME))
LIBS_GCOV_PATH=$(addprefix ${BUILD_DIR}/coverage/,$(addsuffix .gcov,$(LIBS_NAME)))

# Define multiple target-specific default variables
comma := ,
$(eval $(LIBS_TEST_PATH): LDLIBS=-lboost_unit_test_framework)
$(eval $(LIBS_TEST_PATH): LDFLAGS=-Wl$(comma)--no-as-needed)
$(eval $(LIBS_TEST_PATH): CXXFLAGS=-O0 -std=c++17 -g3 -Wall -fprofile-arcs -ftest-coverage)

# Phony definition

.PHONY: lib
lib: ${LIBS_STATIC_PATH}

.PHONY: test
test: ${LIBS_TEST_PATH}

.PHONY: clean
clean:
	${RM} -r ${BUILD_DIR} *.gcda *.gcno *.gcov

.PHONY: doc
doc:
	doxygen doxyfile

.PHONY: coverage
coverage: ${BUILD_DIR}/coverage/index.html

# Manage arborescence

${BUILD_DIR}:
	mkdir -p $@
${BUILD_DIR}/src ${BUILD_DIR}/lib ${BUILD_DIR}/bin ${BUILD_DIR}/test ${BUILD_DIR}/coverage: ${BUILD_DIR}
	mkdir -p $@

# Build libraries

${LIBS_STATIC_PATH} : BASE=$(subst ${BUILD_DIR}/lib/,,$(subst .a,,$@))
${LIBS_STATIC_PATH} : ${BUILD_DIR}/lib 
	echo " base is '${BASE}'"
	${CXX} -I"./${SRC_LIB}" ${CXXFLAGS} -c ${LDFLAGS} ${LDLIBS} -o ${BUILD_DIR}/lib/${BASE}.o  ${SRC_LIB}/${BASE}.cpp
	ar -q $@ build/lib/${BASE}.o 

# Build and run tests

${LIBS_TEST_PATH} : BASE=$(subst ${BUILD_DIR}/test/,,$@)
${LIBS_TEST_PATH} : ${LIBS_STATIC_PATH} ${BUILD_DIR}/test
	${CXX} ${CXXFLAGS} -I"./${SRC_LIB}" -I"./${SRC_TEST}" ${LDFLAGS} ${LDLIBS} -o $@ ${SRC_TEST}/test_${BASE}.cpp ${SRC_LIB}/${BASE}.cpp
	chmod u+x $@
	export export BOOST_TEST_LOG_LEVEL=all ; valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes $@
	gcov -s $PWD -r ${BASE}.gcda
	
${BUILD_DIR}/coverage/index.html: ${LIBS_TEST_PATH}
	lcov --capture --directory ./ --output-file ${BUILD_DIR}/coverage.info --no-external
	genhtml ${BUILD_DIR}/coverage.info --output-directory out$(subst /index.html,,$@)
	
# Build test and 

${LIBS_GCOV_PATH}: BASE=$(subst ${BUILD_DIR}/coverage/,,$(subst .gcov,,$@))
${LIBS_GCOV_PATH}: ${BUILD_DIR}/coverage
	${CXX} ${CXXFLAGS} -I"./${SRC_LIB}" -I"./${SRC_TEST}" ${LDFLAGS} ${LDLIBS} -o ${BUILD_DIR}/coverage/run_${BASE} ${SRC_TEST}/test_${BASE}.cpp ${SRC_LIB}/${BASE}.cpp
	chmod u+x ${BUILD_DIR}/coverage/run_${BASE}
	export BOOST_TEST_LOG_LEVEL=error ; ${BUILD_DIR}/coverage/run_${BASE}
	export CODACY_PROJECT_TOKEN=8a244c65f9eb43269bb02c8865aa3177
	bash <(curl -Ls https://coverage.codacy.com/get.sh) report -r build/coverage.info








	