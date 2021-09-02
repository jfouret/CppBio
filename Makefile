
SRC_DIR=src
INTERNAL_LIB=lib
TEST_LIB=test
BUILD_DIR=build

CXX=g++

GXX_OPTS=-O0 -g3 -Wall

LIBS_NAME= seq 
LIBS=$(addprefix ${BUILD_DIR}/lib/,$(addsuffix .a,$(LIBS_NAME)))
LIBS_test=$(addprefix ${BUILD_DIR}/test/,$(LIBS_NAME))


.PHONY: lib
lib: ${LIBS}

.PHONY: test
test: ${LIBS_test}

.PHONY: clean
clean:
	rm -r build

.PHONY: doc
doc:
	doxygen doxyfile

${BUILD_DIR}:
	mkdir -p $@
	
${BUILD_DIR}/src ${BUILD_DIR}/lib ${BUILD_DIR}/bin ${BUILD_DIR}/test: ${BUILD_DIR}
	mkdir -p $@

${LIBS} : BASE=$(subst ${BUILD_DIR}/lib/,,$(subst .a,,$@))
${LIBS} : ${BUILD_DIR}/lib 
	echo " base is '${BASE}'"
	${CXX} -I"./${INTERNAL_LIB}" ${GXX_OPTS} -c -o ${BUILD_DIR}/lib/${BASE}.o  ${INTERNAL_LIB}/${BASE}.cpp
	ar -q $@ build/lib/${BASE}.o 

${LIBS_test} : BASE=$(subst ${BUILD_DIR}/test/,,$@)
${LIBS_test} : ${LIBS} ${BUILD_DIR}/test
	${CXX} -I"./${INTERNAL_LIB}" -I"./${TEST_LIB}" -Wl,--no-as-needed -lcppunit -o $@ ${TEST_LIB}/test_${BASE}.cpp ${BUILD_DIR}/lib/${BASE}.a
	chmod u+x $@
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes $@

build/bin/test : build/bin build/lib/seq.a
	${CXX} -I";/${INTERNAL_LIB}" -o $@ src/test.cpp build/lib/seq.a -lcppunit
	chmod u+x $@

