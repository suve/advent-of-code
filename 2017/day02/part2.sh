#!/bin/bash

function evenly_divisible() {
	if [ "$1" -gt "$2" ]; then
		HI=$1
		LO=$2
	else
		HI=$2
		LO=$1
	fi
	
	((DIVIDED = HI / LO))
	((RESULT = DIVIDED * LO))
	
	if [ "$RESULT" -eq "$HI" ]; then
		echo $DIVIDED
	else
		echo 0
	fi
}

function check_row() {
	NUMBERS=($1)
	COUNT=${#NUMBERS[@]}
	((COUNT_1 = COUNT - 1))
	
	I=0
	while [ $I -lt $COUNT_1 ]; do
		I_NUM=${NUMBERS[$I]}
		
		((J = I + 1))
		while [ $J -lt $COUNT ]; do
			J_NUM=${NUMBERS[$J]}
			
			EVENLY=$(evenly_divisible $I_NUM $J_NUM)
			if [ "$EVENLY" -gt 0 ]; then
				echo $EVENLY
				exit
			fi
			
			((J = J+1))
		done
		
		((I = I + 1))
	done
	
	echo "-1"
}

CHECKSUM=0
while read LINE; do
	ROW=$(check_row "$LINE")
	echo "- $ROW"
	
	((CHECKSUM = CHECKSUM + ROW))
done

echo "> $CHECKSUM"
