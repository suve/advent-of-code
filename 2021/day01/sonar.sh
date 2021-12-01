#!/bin/bash

((INC=0))
((DEC=0))
((NCH=0))

read PREV
while read NEXT; do
	if [[ "${NEXT}" -gt "${PREV}" ]]; then
		((INC+=1))
	elif [[ "${NEXT}" -lt "${PREV}" ]]; then
		((DEC+=1))
	else
		((NCH+=1))
	fi
	PREV="${NEXT}"
done

echo "${INC} increases, ${DEC} decreases, ${NCH} no-changes"
