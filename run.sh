#!/bin/bash
#SBATCH --partition=hef -w cn96 --time=02-00:00:00

source params.sh

FILE="in/conf-$1.dat"

DIRNAME=${PWD##*/}  

echo "vol: $VOLUME"
FID="$DIRNAME-$VOLUME-$1"
echo $FID
cat <<EOM >$FILE
lambda			0.693147
targetVolume	$VOLUME
slices			$SLICES
sphere			$SPHERE
seed			$1
fileID			$FID
measurements	$MEASUREMENTS
EOM
srun -u main.x $FILE # for running with slurm
#./main.x $FILE # for running locally
if [ $? -ne 0 ]
then
	exit 1
fi

for O in "${OBSERVABLES[@]}"
do
cat out/$O-$DIRNAME-$VOLUME-* > out/agg-$O-$DIRNAME-$VOLUME.dat
cp out/agg-$O-$DIRNAME-$VOLUME.dat "$TARGET_DIR/agg-$DIRNAME-$O-$VOLUME.dat"
done
