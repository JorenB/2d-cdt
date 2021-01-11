#!/usr/bin/env bash

MAIN=cdt2d.x

rm -f ${MAIN} out/*.dat
make --no-print-directory -C ..
cp ../${MAIN} .
./${MAIN} config.dat
