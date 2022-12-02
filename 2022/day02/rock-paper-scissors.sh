#!/bin/bash

ONE=0
TWO=0
while read THEM US; do
	if [[ "${THEM}" == "A" ]]; then
		THEM=0
	elif [[ "${THEM}" == "B" ]]; then
		THEM=1
	elif [[ "${THEM}" == "C" ]]; then
		THEM=2
	else
		echo "Error: Unknown opponent move \"${THEM}\"" >&2
		exit 1
	fi

	if [[ "${US}" == "X" ]]; then
		US=0
	elif [[ "${US}" == "Y" ]]; then
		US=1
	elif [[ "${US}" == "Z" ]]; then
		US=2
	else
		echo "Error: Unknown player move \"${US}\"" >&2
		exit 1
	fi

	((WINNING=(THEM + 1) % 3))
	((LOSING=(THEM + 2) % 3))

	# -- Part 1 calculation

	if [[ "${US}" == "${THEM}" ]]; then
		ROUND=3
	elif [[ "${US}" == "${WINNING}" ]]; then
		ROUND=6
	else
		ROUND=0
	fi
	((ONE+=ROUND+US+1))

	# -- Part 2 calculation

	if [[ "${US}" == "0" ]]; then   # 'X' -> we should lose this one
		((TWO+=LOSING+1))
	elif [[ "${US}" == "1" ]]; then # 'Y' -> we should draw this one
		((TWO+=3+THEM+1))
	else                            # 'Z' -> we should win this one
		((TWO+=6+WINNING+1))
	fi
done

echo "Part 1: ${ONE}"
echo "Part 2: ${TWO}"
