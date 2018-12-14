#!/bin/bash

read IGNORE1 IGNORE2 INITIAL_STATE

STATE="..........${INITIAL_STATE}.............................."

OFFSET=10
END=${#STATE}
((END=END-4))

readarray -s 1 -t TRANSFORM
TRANSFORM_NUM=${#TRANSFORM[@]}

echo "--> $TRANSFORM_NUM transformation rules read"


echo " 0: $STATE"
GENERATION=1
while [[ $GENERATION -le 20 ]]; do
	NEWSTATE=$STATE
	
	POS=0
	while [[ $POS -lt $END ]]; do
		T=0
		REPLACED=0
		while [[ $T -lt $TRANSFORM_NUM ]]; do
			FRAGMENT=${STATE:$POS:5}
			PATTERN=${TRANSFORM[$T]:0:5}
			RESULT=${TRANSFORM[$T]:9:1}
			
			if [[ "$FRAGMENT" == "$PATTERN" ]]; then
				# echo "--> '${FRAGMENT}' matched '${PATTERN}', new result is '${RESULT}'"
				
				((POS_BEFORE=POS+2))
				((POS_AFTER=POS+3))
				NEWSTATE="${NEWSTATE:0:$POS_BEFORE}${RESULT}${NEWSTATE:$POS_AFTER:$END}"
				
				((REPLACED=1))
				((T=TRANSFORM_NUM))
			fi
			
			((T=T+1))
		done
		
		if [[ $REPLACED -eq 0 ]]; then
			((POS_BEFORE=POS+2))
			((POS_AFTER=POS+3))
			NEWSTATE="${NEWSTATE:0:$POS_BEFORE}.${NEWSTATE:$POS_AFTER:$END}"
		fi
		
		((POS=POS+1))
	done
	
	STATE=$NEWSTATE
	printf "%2d: %s\n" "$GENERATION" "$STATE"
	
	((GENERATION=GENERATION+1))
done

SUM=0
POS=0
while [[ $POS -le $END ]]; do
	FRAGMENT=${STATE:$POS:1}
	
	if [[ "$FRAGMENT" == '#' ]]; then
		((SUM=SUM+POS-OFFSET))
	fi
	
	((POS=POS+1))
done
echo "--> $SUM"
