#!/bin/bash

TOPDIR=$(pwd)
SRCDIR="${TOPDIR}/src"
TESTDIR="${TOPDIR}/test"
TESTS="zLog zProgOpt zSem zThread zData zEvent zSignal zTimer zConfig zGpio zSerial zLed zSocket"
COVDIR="${TOPDIR}/coverage"

rm -rf ${SRCDIR} ${TESTDIR} ${COVDIR}
mkdir -p ${SRCDIR} ${TESTDIR} ${COVDIR}
../configure --enable-gcov
cd ${TESTDIR}
lcov --base-directory . --directory . --zerocounters -q
cd ${TOPDIR}
make check

for test in ${TESTS}; do
  cd ${SRCDIR}/${test}
  lcov --base-directory . --directory . -c -o ${COVDIR}/${test}_src.info
  lcov --remove ${COVDIR}/${test}_src.info "/usr*" -o ${COVDIR}/${test}_src.info
  cd ${TESTDIR}/${test}
  lcov --base-directory . --directory . -c -o ${COVDIR}/${test}_test.info
  lcov --remove ${COVDIR}/${test}_test.info "/usr*" -o ${COVDIR}/${test}_test.info
done

genhtml -o ${COVDIR} -t "Zeno Utilities Test Coverage" --num-spaces 4 ${COVDIR}/*.info

cd ${TOPDIR}

exit 0


