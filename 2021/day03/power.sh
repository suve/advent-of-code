#!/bin/bash

COUNT=0
ACC=()
while read LINE; do
	for ((i=0;i<${#LINE};i++)); do
		((ACC[i]+=${LINE:i:1}))
	done
	((COUNT+=1))
done
((HALF=COUNT/2))

GAMMA=0
EPSILON=0
for ((i=0;i<${#ACC[@]};i++)); do
	if [[ "${ACC[$i]}" -ge "${HALF}" ]]; then
		((GAMMA=GAMMA*2+1))
		((EPSILON*=2))
	else
		((GAMMA*=2))
		((EPSILON=EPSILON*2+1))
	fi
done

((POWER=GAMMA*EPSILON))
echo "${GAMMA} * ${EPSILON} = ${POWER}"
