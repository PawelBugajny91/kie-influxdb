startT=$(($2/1000000000))
endT=$(($3/1000000000))
DS=$(date +'%Y-%m-%dT%H:%M:%SZ' -d "@$startT")
DE=$(date +'%Y-%m-%dT%H:%M:%SZ' -d "@$endT")
time="start: "$DS", stop: "$DE
cd ../kie-influxdb/
echo "Go telegraf" $1 $time
echo "./getDataAresT.sh $1 $time"
./getDataAresT.sh $1 $time
echo "Get data done"
if $(stat -c%s main.sh)>150 ; then
./csvToJson.out $1.csv $1.json $1
fi
echo "scv to json done"
./putDataKie.sh $1.json CreateBasicEvents 2> logs.dat
echo "put data done"
./getDataKie.sh CreateBasicEvents 2> logs.dat
echo "get data Kie done"
if $(stat -c%s main.sh)>150 ; then
./jsonToCsv.out Events.json BlindingEvent.csv BlindingEvent
fi
echo "json to csv done"
./writeData.sh BlindingEvent
echo "basic event complete"
./generateAlert.sh