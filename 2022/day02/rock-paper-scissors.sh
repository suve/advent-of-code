#!/bin/bash

SCORE=0
while read THEM US; do
	if [[ "${THEM}" == "A" ]]; then
		THEM=1
	elif [[ "${THEM}" == "B" ]]; then
		THEM=2
	elif [[ "${THEM}" == "C" ]]; then
		THEM=3
	else
		echo "Error: Unknown opponent move \"${THEM}\"" >&2
		exit 1
	fi

	if [[ "${US}" == "X" ]]; then
		US=1
	elif [[ "${US}" == "Y" ]]; then
		US=2
	elif [[ "${US}" == "Z" ]]; then
		US=3
	else
		echo "Error: Unknown player move \"${US}\"" >&2
		exit 1
	fi

	if [[ "${US}" -eq "${THEM}" ]]; then
		# Draw
		((SCORE+=US+3))
	elif [[ "${US}" -eq "$(expr "${THEM}" '+' 1)" ]] || [[ "${US}" -eq "$(expr "${THEM}" '-' 2)" ]]; then
		# Win
		((SCORE+=US+6))
	else
		((SCORE+=US))
	fi
done
echo "${SCORE}"
