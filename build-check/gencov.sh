#!/bin/bash -x

TOPDIR=$(pwd)
SRCDIR="${TOPDIR}/src"
TESTDIR="${TOPDIR}/test"
TESTS="zLog zProgOpt zThread zTimer zSem zData zEvent zSocket zNode zMessage zSwitch zGpio"
COVDIR="${TOPDIR}/coverage"

rm -rf ${TOPDIR}/src ${TOPDIR}/test ${COVDIR}
../configure --enable-gcov --enable-valgrind
cd ${TESTDIR}
lcov --base-directory . --directory . --zerocounters -q
cd ${TOPDIR}
make check

for test in ${TESTS}; do
  echo ${test}
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


