#!/bin/bash

function find_substr() {
	local HAYSTACK=$1
	local NEEDLE=$2
	local OFFSET=$3
	
	if [[ "$OFFSET" -gt 0 ]]; then
		HAYSTACK=${HAYSTACK:$OFFSET:9999999999}
	else
		OFFSET=0
	fi
	
	local RESULT=$(awk 'BEGIN {print index("'"$HAYSTACK"'", "'"$NEEDLE"'")}')
	if [[ "$RESULT" -gt 0 ]]; then
		((RESULT=RESULT-1+OFFSET))
		echo "$RESULT"
	else
		echo "-1"
	fi
}

function repeat_char() {
	local CHAR=$1
	local TIMES=$2
	
	local RESULT=''
	while [[ TIMES -gt 0 ]]; do
		RESULT="${RESULT}${CHAR}"
		((TIMES=TIMES-1))
	done
	
	echo "$RESULT"
}

function add_transform() {
	local NEW_PATTERN=$1
	local NEW_RESULT=$2
	
	local Ti=0
	while [[ $Ti -lt $TRANSFORM_NUM ]]; do
		local PATTERN=${TRANSFORM_PATTERN[$Ti]}
		if [[ "$NEW_PATTERN" == "$PATTERN" ]]; then
			return
		fi
		
		((Ti=Ti+1))
	done
	
	TRANSFORM_PATTERN[$TRANSFORM_NUM]=$NEW_PATTERN
	TRANSFORM_RESULT[$TRANSFORM_NUM]=$NEW_RESULT
	((TRANSFORM_NUM=TRANSFORM_NUM+1))
	
	echo "--> added new transformation (#$TRANSFORM_NUM): '$NEW_PATTERN' -> '$NEW_RESULT'"
}


read IGNORE1 IGNORE2 INITIAL_STATE

STATE=".......................................................${INITIAL_STATE}.........."
FIRST_POT=-55

TRANSFORM_PATTERN=()
TRANSFORM_RESULT=()
TRANSFORM_NUM=0

readarray -s 1 -t TRANSFORM_RAW
T=0
while [[ $T -lt ${#TRANSFORM_RAW[@]} ]]; do
	RAW_PATTERN=${TRANSFORM_RAW[$T]:0:5}
	RAW_RESULT=${TRANSFORM_RAW[$T]:9:1}
	
	TRANSFORM_PATTERN[$TRANSFORM_NUM]=$RAW_PATTERN
	TRANSFORM_RESULT[$TRANSFORM_NUM]=$RAW_RESULT
	((TRANSFORM_NUM=TRANSFORM_NUM+1))
	((T=T+1))
done

echo "--> $TRANSFORM_NUM transformation rules read"

echo "  0: $STATE"


GENERATION=1
LIMIT=50000000000
while [[ $GENERATION -le $LIMIT ]]; do
	NEWSTATE=$(repeat_char '.' ${#STATE})
	
	((T=TRANSFORM_NUM-1))
	while [[ $T -ge 0 ]]; do
		OFFSET=0
		PATTERN=${TRANSFORM_PATTERN[$T]}
		while [[ 1 ]]; do
			INDEX=$(find_substr "$STATE" "$PATTERN" "$OFFSET")
			if [[ "$INDEX" -lt 0 ]]; then break; fi
			
			REPLACE=${TRANSFORM_RESULT[$T]}
			REPLACE_LEN=${#REPLACE}
			
			((POS_BEFORE=INDEX+2))
			((POS_AFTER=INDEX+2+REPLACE_LEN))
			NEWSTATE="${NEWSTATE:0:$POS_BEFORE}${REPLACE}${NEWSTATE:$POS_AFTER:9999999999}"
			
			# echo "--> pattern '$PATTERN' matched; replacing '$REPLACE' at index $INDEX"
			
			((OFFSET=INDEX+REPLACE_LEN))
		done
		
		((T=T-1))
	done
	
#	OFFSET=0
#	while [[ 1 ]]; do
#		LEFT_INDEX=$(find_substr "$STATE" "..#" "$OFFSET")
#		if [[ "$LEFT_INDEX" -lt 0 ]]; then break; fi
#		
#		RIGHT_INDEX=$(find_substr "$STATE" "#.." "$LEFT_INDEX")
#		if [[ "$RIGHT_INDEX" -lt 0 ]]; then break; fi
#		
#		((PATTERN_LEN=RIGHT_INDEX+3-LEFT_INDEX))
#		PATTERN=${STATE:$LEFT_INDEX:PATTERN_LEN}
#		
#		((REPLACE_POS=LEFT_INDEX+2))
#		((REPLACE_LEN=PATTERN_LEN-4))
#		REPLACE=${NEWSTATE:$REPLACE_POS:$REPLACE_LEN}
#		
#		add_transform "$PATTERN" "$REPLACE"
#		((OFFSET=LEFT_INDEX+REPLACE_LEN))
#	done
	
	OLDSTATE=$STATE
	STATE=$NEWSTATE
	printf "%3d: %s\n" "$GENERATION" "$STATE"
	
	STATE="${STATE:1:9999999999}."
	
	if [[ "$STATE" == "$OLDSTATE" ]]; then
		echo "reached stable state after $GENERATION generations!"
		((SHIFT=LIMIT-GENERATION))
		
		echo "fast-forwarding $SHIFT generations..."
		((FIRST_POT=FIRST_POT+SHIFT))
		((GENERATION=GENERATION+SHIFT))
	fi
	
	((GENERATION=GENERATION+1))
	((FIRST_POT=FIRST_POT+1))
done

SUM=0
POS=0
while [[ $POS -lt ${#STATE} ]]; do
	FRAGMENT=${STATE:$POS:1}
	
	if [[ "$FRAGMENT" == '#' ]]; then
		((VALUE=POS+FIRST_POT))
		((SUM=SUM+VALUE))
		echo "--> pot: ${VALUE}"
	fi
	
	((POS=POS+1))
done
echo "--> sum: $SUM"
