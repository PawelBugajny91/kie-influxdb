#!/bin/bash

what='{"lookup" : null, "commands" : [{"get-objects":{"out-identifier":"sci"}}]}'
where='http://172.17.0.3:8080/kie-server/services/rest/server/containers/instances/'$1'_1.0.0-SNAPSHOT'

curl -u 'kieserver:kieserver1!' -H "Accept: application/json" -H "Content-Type: application/json" -XPOST "$where" \
 -d "$what" > Events.json
