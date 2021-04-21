#! /bin/bash

./menager.sh fd sci FDEvent Fd 
./GoEvents.sh FDEvent

./getDataKie.sh Cep_1

./jsonToCsv.out Events.json AlertEvent.csv AlertEvent

./writeData.sh AlertEvent AlertEvent