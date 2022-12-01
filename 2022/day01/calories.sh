#!/bin/bash

(
	SUM=0
	while read ITEM; do
		if [[ "${ITEM}" == "" ]]; then
			echo "${SUM}"
			SUM=0
		else
			((SUM+=ITEM))
		fi
	done
	echo "${SUM}"
) | sort -nr | head -n 3 | (
	read FIRST
	read SECOND
	read THIRD

	((SUM=FIRST+SECOND+THIRD))
	echo "Part 1: ${FIRST}"
	echo "Part 2: ${SUM}"
)
