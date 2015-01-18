#!/bin/bash

TOPDIR=$(pwd)
TESTDIR="${TOPDIR}/test"
TESTS="zLog zProgOpt"

rm -rf ${TOPDIR}/src ${TOPDIR}/test ${TOPDIR}/coverage
../configure --enable-gcov
cd ${TESTDIR}
lcov --base-directory . --directory . --zerocounters -q
cd ${TOPDIR}
make check

for test in ${TESTS}; do
  echo ${test}
  cd ${TESTDIR}/${test}
  lcov --base-directory . --directory . -c -o ${TESTDIR}/${test}_test.info
  lcov --remove ${TESTDIR}/${test}_test.info "/usr*" -o ${TESTDIR}/${test}_test.info
  genhtml -o ${TOPDIR}/coverage -t "${test} Test Coverage" --num-spaces 4 ${TESTDIR}/${test}_test.info
done

cd ${TOPDIR}

exit

lcov --remove ${THISDIR}/${BIN}_test.info "*_utest.cpp" -o ${THISDIR}/${BIN}_test.info

