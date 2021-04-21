#! /bin/bash
#getDataAres.sh parameterNameFromInfluxDB(and file_out) date
query='from(bucket: "SimulatedAresData/autogen")
  |> range('"$2 $3 $4 $5"')
  |> filter(fn: (r) => r._measurement == "mustlink" and (r._field == "value"))'
echo $2 $3 $4 $5
curl -XPOST 'localhost:8086/api/v2/query?db=Fd&epoch=ns' -sS -H 'Accept:application/json' -H 'Content-type:application/vnd.flux' -d \
"$query" > $1.csv


  #./csv2json fd.csv fd.json prefix.json suffix.json