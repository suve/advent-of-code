#!/bin/bash

MODE=0
if [ $# -lt 1 ]; then
	echo "You must provide the jump mode: either --part1 or --part2"
	exit
elif [ "$1" == "--part1" ]; then
	MODE=1
elif [ "$1" == "--part2" ]; then
	MODE=2
else
	echo "Unknown jump mode - should be --part1 or --part2"
	exit
fi


NUMBERS=()
while read LINE; do
	NUMBERS[${#NUMBERS[@]}]="$LINE"
done

LEN=${#NUMBERS[@]}
POS=0

STEPS=0
while [ $POS -ge 0 ] && [ $POS -lt $LEN ]; do
	((NEWPOS = POS + NUMBERS[POS]))
	
	if [ $MODE -eq 1 ] || [ "${NUMBERS[$POS]}" -lt 3 ]; then
		((NUMBERS[POS] += 1))
	else
		((NUMBERS[POS] -= 1))
	fi
	# echo "> $POS: '${NUMBERS[$POS]}'"
	
	((POS = NEWPOS))
	((STEPS += 1))
done

echo "= $STEPS"
