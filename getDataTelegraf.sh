#! /bin/bash
#getDataTelegraf.sh parameterNameFromInfluxDB(and file_out)
queryb='from(bucket: "telegraf/autogen")
  |> range('"$2 $3 $4 $5"')
  |> filter(fn: (r) => r._measurement == "mustlink" and r.name == "'
querye='"and r._field == "value")'
query=$queryb$1$querye
echo $query
curl -XPOST 'localhost:8086/api/v2/query?db=Fd&epoch=ns' -sS -H 'Accept:application/json' -H 'Content-type:application/vnd.flux' -d \
"$query" > $1.csv


  #./csv2json fd.csv fd.json prefix.json suffix.json

#  from(bucket: "telegraf/autogen")
#  |> range(start: dashboardTime)
#  |> filter(fn: (r) => r._measurement == "mustlink" and r.name == "ASA1321L" and r._field == "value")