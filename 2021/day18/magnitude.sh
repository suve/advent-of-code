#!/bin/bash

cd "$(dirname "${0}")"

NUMBERS=()
while read NUM; do
	NUMBERS=("${NUMBERS[@]}" "${NUM}")
done

BEST=0
COUNT=${#NUMBERS[@]}
for ((i=0;i<COUNT;++i)); do
	for ((j=0;j<COUNT;++j)); do
		if [[ "${i}" -eq "${j}" ]]; then
			continue
		fi
		
		SCORE="$({ echo "${NUMBERS[$i]}"; echo "${NUMBERS[$j]}"; } | ./addition | tail -n 1)"
		if [[ "${SCORE}" -gt "${BEST}" ]]; then
			BEST="${SCORE}"
		fi
	done
done

echo "${BEST}"
