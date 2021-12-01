#!/bin/bash

((INC=0))
((DEC=0))

read PREV
while read NEXT; do
	if [[ "${NEXT}" -gt "${PREV}" ]]; then
		((INC+=1))
	else
		((DEC+=1))
	fi
	PREV="${NEXT}"
done

echo "${INC} increases, ${DEC} decreases"
