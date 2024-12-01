#!/bin/bash

LEFT_STR=""
RIGHT_STR=""
while read LEFT RIGHT; do
	LEFT_STR="${LEFT_STR} ${LEFT}"
	RIGHT_STR="${RIGHT_STR} ${RIGHT}"
done

LEFT_ARRAY=( $(echo "${LEFT_STR}" | tr ' ' $'\n' | sort) )
RIGHT_ARRAY=( $(echo "${RIGHT_STR}" | tr ' ' $'\n' | sort) )

COUNT="${#LEFT_ARRAY[@]}"
TOTAL=0
for ((i=0; i < COUNT; ++i)); do
	LEFT="${LEFT_ARRAY[i]}"
	RIGHT="${RIGHT_ARRAY[i]}"
	if [[ "${LEFT}" -gt "${RIGHT}" ]]; then
		((TOTAL+=LEFT-RIGHT))
	else
		((TOTAL+=RIGHT-LEFT))
	fi
done

echo "Part1: ${TOTAL}"
