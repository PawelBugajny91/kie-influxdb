startT = $(($2/1000000000))
endT = $(($3/1000000000))

time = "start: "+'date +'%Y-%m-%dT%H:%M:%SZ' -d "@$startT"'+", stop: "+'date +'%Y-%m-%dT%H:%M:%SZ' -d "@$endT"'

./getDataAresT.sh $1 $time
echo "Get data done"
./csvToJson.out $1.csv $1.json 
echo "scv to json done"
./putDataKie.sh $1.json CreateBasicEvents 2> logs.dat
echo "put data done"
./getDataKie.sh CreateBasicEvents 2> logs.dat
echo "get data Kie done"
./jsonToCsv.out Events.json BlindingEvent.csv BlindingEvent
echo "json to csv done"
./writeData.sh BlindingEvent
echo "basic event complete"