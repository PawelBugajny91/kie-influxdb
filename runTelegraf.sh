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
#if $(stat -c%s $1.csv)>150 ; then
./csvToJson.out $1.csv $1.json $1
#fi
echo "scv to json done"
./putDataKie.sh $1.json CreateBasicEvents 2> logsput.dat
echo "put data done"
./getDataKie.sh CreateBasicEvents 2> logsget.dat
echo "get data Kie done"
./jsonToCsv.out Events.json BlindingEvent.csv BlindingEvent
echo "json to csv done"
./writeData.sh BlindingEvent
echo "basic event complete" 
./GoEvents.sh BlindingEvent $time
echo "Go Alert"
./getDataKie.sh Cep_1
echo "Get data kie done"
./jsonToCsv.out Events.json AlertEvent.csv AlertEvent
echo "Alert Done"
./writeData.sh AlertEvent AlertEvent
echo "Alert saved"