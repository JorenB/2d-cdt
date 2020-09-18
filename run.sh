#!/bin/bash
#SBATCH --partition=hef -w cn96 --time=02-00:00:00

source params.sh

FILE="in/conf-$1.ini"

DIRNAME=${PWD##*/}  

echo "vol: $VOLUME"
FID="$DIRNAME-$VOLUME-$1"
echo $FID
cat <<EOM >$FILE
[geometry]
targetVolume = $VOLUME
slices = $SLICES
sphere = $SPHERE

[simulation]
fileID = $FID
measurements = $MEASUREMENTS
EOM
./main.x $FILE
if [ $? -ne 0 ]
then
	exit 1
fi

for O in "${OBSERVABLES[@]}"
do
cat out/$O-$DIRNAME-$VOLUME-* > out/agg-$O-$DIRNAME-$VOLUME.dat
cp out/agg-$O-$DIRNAME-$VOLUME.dat "$TARGET_DIR"
done
