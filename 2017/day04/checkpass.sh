#!/bin/bash

MODE=0
if [ $# -lt 1 ]; then
	echo "You must provide the comparison mode: either --part1 or --part2"
	exit
elif [ "$1" == "--part1" ]; then
	MODE=1
elif [ "$1" == "--part2" ]; then
	MODE=2
else
	echo "Unknown comparison mode - should be --part1 or --part2"
	exit
fi

function ord { 
	printf "%d" "'$1"
}

function words_match() {
	if [ "$MODE" -eq 1 ]; then
		if [ "$1" == "$2" ]; then
			echo 1
		else
			echo 0
		fi
		exit
	fi
	
	
	WORD_1="$1"
	WORD_2="$2"
	LENGTH_1=${#WORD_1} 
	LENGTH_2=${#WORD_2}
	
	# Check if length matches
	if [ "$LENGTH_1" -ne "$LENGTH_2" ]; then
		echo 0
		exit
	fi
	
	LETTERS_1=()
	LETTERS_2=()
	for ((i = 97; i <= 122; ++i)); do
		((LETTERS_1[i] = 0))
		((LETTERS_2[i] = 0))
	done
	
	for ((i = 0; i < LENGTH_1; ++i)); do
		LET="${WORD_1:$i:1}"
		ORD=`ord "$LET"`
		((LETTERS_1[ORD] += 1))
	done
	for ((i = 0; i < LENGTH_2; ++i)); do
		LET="${WORD_2:$i:1}"
		ORD=`ord "$LET"`
		((LETTERS_2[ORD] += 1))
	done
	
	#for ((i = 97; i <= 122; ++i)); do
	#	echo "letter $i: ${LETTERS_1[$i]} vs ${LETTERS_2[$i]}" >&2
	#done
	
	for ((i = 97; i <= 122; ++i)); do
		if [ "${LETTERS_1[$i]}" -ne "${LETTERS_2[$i]}" ]; then
			echo 0
			exit
		fi
	done
	
	echo 1
}

function is_valid_pass() {
	LINE="$1"
	WORDS=($LINE)
	LENGTH=${#WORDS[@]}
	
	for ((i = 0; i < LENGTH-1; ++i)); do
		for ((j = i+1; j < LENGTH; ++j)); do
			MATCH=`words_match "${WORDS[$i]}" "${WORDS[$j]}"`
			if [ "$MATCH" -eq 1 ]; then
				echo 0
				exit
			fi
		done
	done
	
	echo 1
}

VALID_TOTAL=0
while read LINE; do
	VALID_NOW=`is_valid_pass "$LINE"`
	
	echo "line: '$LINE'; valid: $VALID_NOW"
	
	((VALID_TOTAL = VALID_TOTAL + VALID_NOW))
done

echo "> $VALID_TOTAL"
