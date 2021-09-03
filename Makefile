# Define the project-specific arborescence
SRC=src
SRC_LIB=lib
SRC_TEST=test
BUILD_DIR=build

# Define default make variables
CXX=g++
CXXFLAGS=-O0 -g3 -Wall
RM=rm -f

# Define the name of the libraries to be built
LIBS_NAME=seq


# Get the path 
LIBS_STATIC_PATH=$(addprefix ${BUILD_DIR}/lib/,$(addsuffix .a,$(LIBS_NAME)))
LIBS_TEST_PATH=$(addprefix ${BUILD_DIR}/test/,$(LIBS_NAME))

# Define multiple target-specific default variables
comma := ,
$(eval $(LIBS_TEST_PATH): LDLIBS=-lcppunit)
$(eval $(LIBS_TEST_PATH): LDFLAGS=-Wl$(comma)--no-as-needed)

# Phony definition

.PHONY: lib
lib: ${LIBS_STATIC_PATH}

.PHONY: test
test: ${LIBS_TEST_PATH}

.PHONY: clean
clean:
	${RM} -r ${BUILD_DIR}

.PHONY: doc
doc:
	doxygen doxyfile

# Manage arborescence

${BUILD_DIR}:
	mkdir -p $@
${BUILD_DIR}/src ${BUILD_DIR}/lib ${BUILD_DIR}/bin ${BUILD_DIR}/test: ${BUILD_DIR}
	mkdir -p $@

# Build libraries

${LIBS_STATIC_PATH} : BASE=$(subst ${BUILD_DIR}/lib/,,$(subst .a,,$@))
${LIBS_STATIC_PATH} : ${BUILD_DIR}/lib 
	echo " base is '${BASE}'"
	${CXX} -I"./${SRC_LIB}" ${CXXFLAGS} -c -o ${BUILD_DIR}/lib/${BASE}.o  ${SRC_LIB}/${BASE}.cpp
	ar -q $@ build/lib/${BASE}.o 

# Buidl and run tests

${LIBS_TEST_PATH} : BASE=$(subst ${BUILD_DIR}/test/,,$@)
${LIBS_TEST_PATH} : ${LIBS_STATIC_PATH} ${BUILD_DIR}/test
	${CXX} ${CXXFLAGS} -I"./${SRC_LIB}" -I"./${SRC_TEST}" ${LDFLAGS} ${LDLIBS} -o $@ ${SRC_TEST}/test_${BASE}.cpp ${BUILD_DIR}/lib/${BASE}.a
	chmod u+x $@
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes $@

	