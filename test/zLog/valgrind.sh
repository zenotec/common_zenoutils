#!/bin/sh

DIR=zLog
BIN=.libs/zLogUnitTest
TEST=$(pwd)/${DIR}/${BIN}
LOG=$(pwd)/${DIR}/valgrind.log

if which valgrind > /dev/null 2> /dev/null
then
    valgrind --leak-check=full --show-reachable=yes ${TEST} 2> ${LOG}
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
