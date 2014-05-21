#!/bin/bash

BIN=rtapd
THISDIR=$(pwd)
TESTDIR=${THISDIR}/test

cd ${TESTDIR}
lcov --base-directory . --directory . --zerocounters -q
cd ${THISDIR}
make check
cd ${TESTDIR}
lcov --base-directory . --directory . -c -o ${THISDIR}/${BIN}_test.info
lcov --remove ${THISDIR}/${BIN}_test.info "/usr*" -o ${THISDIR}/${BIN}_test.info
lcov --remove ${THISDIR}/${BIN}_test.info "*_utest.cpp" -o ${THISDIR}/${BIN}_test.info
rm -rf ${THISDIR}/coverage
genhtml -o ${THISDIR}/coverage -t "${BIN} Test Coverage" --num-spaces 4 ${THISDIR}/${BIN}_test.info

