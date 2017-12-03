#!/bin/bash

NUMBERS=()

function coords_to_pos() {
	X=$1
	Y=$2
	
	if [ $X -gt 0 ]; then
		((D_X = X))
	else
		((D_X = -X))
	fi
	if [ $Y -gt 0 ]; then
		((D_Y = Y))
	else
		((D_Y = -Y))
	fi
	
	if [ $D_X -gt $D_Y ]; then
		((SIZE_PLUS = D_X))
		((SIZE_MINUS = 0 - D_X))
	else
		((SIZE_PLUS = D_Y))
		((SIZE_MINUS = 0 - D_Y))
	fi
	((SIDE = SIZE_PLUS + SIZE_PLUS))
	((SQUARE = (SIDE + 1) * (SIDE + 1)))
	
	
	# Check if we're on highest Y (bottom border)
	if [ $Y -eq $SIZE_PLUS ]; then
		((RESULT = SQUARE - (SIZE_PLUS - X)))
		echo $RESULT
		exit
	fi 
	
	# Check if we're on lowest X (left border)
	if [ $X -eq $SIZE_MINUS ]; then
		((RESULT = SQUARE - SIDE - (SIZE_PLUS - Y)))
		echo $RESULT
		exit
	fi
	
	# Check if we're on lowest Y (top border)
	if [ $Y -eq $SIZE_MINUS ]; then
		((RESULT = SQUARE - SIDE - SIDE + (SIZE_MINUS - X)))
		echo $RESULT
		exit
	fi 
	
	# Check if we're on highest X (right border)
	if [ $X -eq $SIZE_PLUS ]; then
		((RESULT = SQUARE - SIDE - SIDE - SIDE + (SIZE_MINUS - Y)))
		echo $RESULT
		exit
	fi
}

function get_number_from_array() {
	POS=$1
	if [ $POS -lt ${#NUMBERS[@]} ]; then
		echo ${NUMBERS[$POS]}
	else
		echo 0
	fi
	exit
}

function calculate_new_number() {
	X=$1
	Y=$2
	SUM=0
	
	for ((iy = -1; iy <= 1; iy++)); do
		for ((ix = -1; ix <= 1; ix++)); do
			if [ $ix -eq 0 ] && [ $iy -eq 0 ]; then
				continue
			fi
			
			((XPOS = X + ix))
			((YPOS = Y + iy))
			
			POS=`coords_to_pos $XPOS $YPOS`
			echo "coords to pos: $XPOS:$YPOS = $POS" >&2
			
			NUMBER=`get_number_from_array $POS`
			((SUM = SUM + NUMBER))
		done
	done
	
	echo $SUM
	exit
}

function fill_numbers() {
	SEARCH=$1
	
	
	((POS = ${#NUMBERS[@]} - 1))
	((CURRENT = ${NUMBERS[$POS]}))
	
	
	DIR="S"
	SIZE_PLUS=1
	SIZE_MINUS=-1
	X=0
	Y=0
	
	while [ $CURRENT -le $SEARCH ]; do
		case $DIR in
			S)
				((X = X+1))
				DIR="U"
			;;
			
			U)
				((Y = Y-1))
				if [ $Y -eq $SIZE_MINUS ]; then
					DIR="L"
				fi
			;;
			
			L)
				((X = X-1))
				if [ $X -eq $SIZE_MINUS ]; then
					DIR="D"
				fi
			;;
			
			D)
				((Y = Y+1))
				if [ $Y -eq $SIZE_PLUS ]; then
					DIR="R"
				fi
			;;
			
			R)
				((X = X+1))
				if [ $X -eq $SIZE_PLUS ]; then
					DIR="S"
					((SIZE_PLUS = SIZE_PLUS + 1))
					((SIZE_MINUS = SIZE_MINUS - 1))
				fi
			;;
			
			*)
				echo "watafugg" >&2
				exit
			;;
		esac
		
		CURRENT=`calculate_new_number $X $Y`
		echo "new number at pos $X:$Y = $CURRENT" >&2
		
		((POS = POS + 1))
		(( NUMBERS[POS] = CURRENT ))
	done
	
	echo $CURRENT
	exit
}

while read INPUT; do
	NUMBERS=(1 1)
	RESULT=`fill_numbers $INPUT`
	
	echo "> $INPUT --- $RESULT"
done
