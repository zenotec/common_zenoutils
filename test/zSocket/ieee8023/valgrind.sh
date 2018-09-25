#!/bin/sh

DIR=zSocket/ieee8023
BIN=.libs/ieee8023/Ieee8023UnitTest
TEST=$(pwd)/${DIR}/${BIN}
LOG=$(pwd)/${DIR}/valgrind.log

export GLIBCXX_FORCE_NEW=1
export GLIBCPP_FORCE_NEW=1

if which valgrind > /dev/null 2> /dev/null
then
    valgrind --leak-check=yes --show-reachable=yes ${TEST} 2> ${LOG}
    cat ${LOG}
    if test "x`grep 'ERROR SUMMARY: 0 errors' ${LOG}`x" = xx
    then
        exit 1
    else
        exit 0
    fi
else
    echo "Can't find valgrind, skipping test."
    exit 77
fi
