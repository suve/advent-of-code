#!/bin/bash

JUMPS=$(sort --numeric-sort < /dev/stdin | (
	# Start with 1 in the 3-difference counter, because:
	# "Your device's built-in adapter is always 3 higher than the highest adapter"
	JUMPS=(0 0 0 1)

	PREVIOUS="0"
	while read NUMBER; do
		((DIFF=NUMBER-PREVIOUS))
		((JUMPS[DIFF]+=1))
		PREVIOUS=${NUMBER}
	done
	
	echo "${JUMPS[@]}"
))
JUMPS=(${JUMPS})

echo "--> 1-jolt jumps: ${JUMPS[1]}"
echo "--> 3-jolt jumps: ${JUMPS[3]}"
echo "Part1:" "$(expr "${JUMPS[1]}" '*' "${JUMPS[3]}")"

