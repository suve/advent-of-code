#!/bin/bash

if [ $# -lt 1 ]; then
	echo "You must provide a mode: either --next or --halfway"
elif [ "$1" == "--next" ]; then
	MODE_HALFWAY=0
elif [ "$1" == "--halfway" ]; then
	MODE_HALFWAY=1
else
	echo "Unknown mode (should be --next or --halfway)"
fi

while read LINE; do
	LEN=${#LINE}
	((HALF_LEN = LEN / 2))
	
	LINE="${LINE}${LINE}"
	
	POS=0
	SUM=0
	while [ $POS -lt $LEN ]; do
		if [ $MODE_HALFWAY -gt 0 ]; then
			((PAIR_POS = POS + HALF_LEN))
			
			DIG1=${LINE:$POS:1}
			DIG2=${LINE:$PAIR_POS:1}
		else
			PAIR=${LINE:$POS:2}
			
			DIG1=${PAIR:0:1}
			DIG2=${PAIR:1:1}
		fi
		
		if [ $DIG1 == $DIG2 ]; then
			((SUM = SUM + DIG1))
		fi
		
		((POS = POS + 1))
	done
	echo "> $SUM"
done
