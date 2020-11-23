#!/bin/bash
where='http://localhost:8086/write?db=Events&precision=ms'
what=$1.csv
curl -i -XPOST "$where" --data-binary @"$what"