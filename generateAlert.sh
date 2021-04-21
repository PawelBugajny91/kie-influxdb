#! /bin/bash

#./menager.sh fd sci FDEvent
#./GoEvents.sh FDEvent

#./menager.sh ares ASA1321L BlindingEvent
./GoEvents.sh BlindingEvent 
echo "Go Alert"
./getDataKie.sh Cep_1
echo "Get data kie done"
#if $(stat -c%s Events.json)>150 ; then
./jsonToCsv.out Events.json AlertEvent.csv AlertEvent
#fi
echo "Alert Done"
./writeData.sh AlertEvent AlertEvent
echo "Alert saved"