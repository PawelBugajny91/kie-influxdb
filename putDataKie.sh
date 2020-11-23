#! /bin/bash
pre="done/prefix.json"
sur="done/suffix.json"
where='http://172.17.0.3:8080/kie-server/services/rest/server/containers/instances/'$2'_1.0.0-SNAPSHOT'

cat $1 | while read linia
do
   echo $linia > done/mid.json
   cat $pre "done/mid.json" $sur > done/result.json
   curl -u 'kieserver:kieserver1!' -H "Accept: application/json" -H "Content-Type: application/json" -XPOST "$where" -d @'done/result.json'
done

