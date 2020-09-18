#!/bin/bash

source params.sh

DIRNAME=${PWD##*/}  

for VOLUME in "${VOLUMES[@]}"
do
for O in "${OBSERVABLES[@]}"
do
cat out/$O-$DIRNAME-$VOLUME-* > out/agg-$O-$DIRNAME-$VOLUME.dat
cp out/agg-$O-$DIRNAME-$VOLUME.dat "$TARGET_DIR/agg-$DIRNAME-$O-$VOLUME.dat"
done
done
