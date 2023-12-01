#!/bin/bash

set -eu -o pipefail

SUM=0
while read LINE; do
	LINE="$(echo "${LINE}" | tr -cd '[0-9]')"
	if [[ "${#LINE}" -eq 0 ]]; then
		continue
	fi

	if [[ "${#LINE}" -eq 1 ]]; then
		# Transform single-digit numbers into two-digit numbers
		LINE="${LINE}${LINE}"
	elif [[ "${#LINE}" -ge 3 ]]; then
		# Extract first and last digit if there's more than two
		FIRST="${LINE:0:1}"
		LAST="${LINE: -1}"
		LINE="${FIRST}${LAST}"
	fi

	((SUM+=LINE))
done

echo "Part1: ${SUM}"
