#! /bin/bash

#./menager.sh fd sci FDEvent
#./GoEvents.sh FDEvent

#./menager.sh ares ASA1321L BlindingEvent
./GoEvents.sh BlindingEvent
echo "Go Alert"
./getDataKie.sh Cep_1
echo "Get data kie done"
./jsonToCsv.out Events.json AlertEvent.csv AlertEvent
echo "Alert Done"
./writeData.sh AlertEvent AlertEvent
echo "Done"