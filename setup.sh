#!/bin/bash

mkdir ~/tmp/data/$1
mkdir ~/tmp/data/$1/in
mkdir ~/tmp/data/$1/out

make all

cp main.x ~/tmp/data/$1/
cp params.sh ~/tmp/data/$1/
cp run.sh ~/tmp/data/$1/
cp batch.sh ~/tmp/data/$1/
cp copy.sh ~/tmp/data/$1/
