#! /bin/bash
time="start: 2019-01-01T00:00:00Z, stop: 2020-02-01T12:00:00Z"
if [ "$1" = "ares" -o "$1" = "Ares" ] ; then
./getDataAres.sh $2 $time
elif [ "$1" = "fd" -o "$1" = "Fd" ] ; then
./getDataFD.sh $2 $time
else
echo "unsuported"
fi
echo "Get data done"
./csvToJson.out $2.csv $2.json $2
echo "scv to json done"
./putDataKie.sh $2.json CreateBasicEvents 2> logs.dat
echo "put data done"
./getDataKie.sh CreateBasicEvents 2> logs.dat
echo "get data Kie done"
./jsonToCsv.out Events.json $3.csv $3
echo "json to csv done"
./writeData.sh $3
echo "basic event complete"