#!/bin/bash
time="start: 2020-06-01T00:00:00Z, stop: 2020-11-27T12:00:00Z"
./getDataEvent.sh $1 $time
echo "events $1 load "
./csvToJson.out $1.csv $1.json $1
echo "events $1 transform"
./putDataKie.sh $1.json Cep_1 >> logs.dat
echo "events $1 put"
