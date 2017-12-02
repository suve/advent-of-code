#!/bin/bash

CHECKSUM=0
while read LINE; do
	NUMBERS=($LINE)
	COUNT=${#NUMBERS[@]}
	
	MIN=${NUMBERS[0]}
	MAX=${NUMBERS[0]}
	
	IDX=1
	while [ $IDX -lt $COUNT ]; do
		if [ ${NUMBERS[$IDX]} -lt $MIN ]; then
			MIN=${NUMBERS[$IDX]}
		elif [ ${NUMBERS[$IDX]} -gt $MAX ]; then
			MAX=${NUMBERS[$IDX]}
		fi
		
		((IDX = IDX + 1))
	done
	
	((DIFF = MAX - MIN))
	echo "- $DIFF"
	
	((CHECKSUM = CHECKSUM + DIFF))
done

echo "> $CHECKSUM"
