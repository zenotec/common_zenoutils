#!/bin/bash

TOPDIR=$(pwd)
SRCDIR="${TOPDIR}/lib"
TESTDIR="${TOPDIR}/test"
TESTS="zLog zProgOpt zThread zTimer zSem zData zEvent zSocket zNode zMessage zSwitch zGpio"
COVDIR="${TOPDIR}/coverage"

rm -rf ${SRCDIR} ${TESTDIR} ${COVDIR}
mkdir -p ${SRCDIR} ${TESTDIR} ${COVDIR}
../configure --enable-gcov
cd ${SRCDIR}
lcov --base-directory . --directory . --zerocounters -q
cd ${TOPDIR}
make check

cd ${SRCDIR}

lcov  --directory . --capture --output-file ${COVDIR}/zeno_src.info

cd ${COVDIR}

lcov --remove zeno_src.info '/usr/*' -o zeno_src.info
genhtml -t "Zeno Utilities Test Coverage" --num-spaces 4  zeno_src.info

cd ${TOPDIR}

exit 0


