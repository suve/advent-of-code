#!/bin/bash

while read NUMBER; do
	RADIUS=1
	SQUARE=1
	D_X=0
	D_Y=0
	
	# Find out which square are we in
	while [ $SQUARE -lt $NUMBER ]; do
		((D_X = D_X + 1))
		((D_Y = D_Y + 1))
		
		((RADIUS = RADIUS + 2))
		((SQUARE = RADIUS * RADIUS))
	done
	
	# Go left until number matches
	STEP=1
	while [ $SQUARE -ne $NUMBER ] && [ $STEP -lt $RADIUS ]; do
		((SQUARE = SQUARE - 1))
		((D_X = D_X - 1))
		
		((STEP = STEP + 1))
	done
	
	# Go up until number matches
	STEP=1
	while [ $SQUARE -ne $NUMBER ] && [ $STEP -lt $RADIUS ]; do
		((SQUARE = SQUARE - 1))
		((D_Y = D_Y - 1))
		
		((STEP = STEP + 1))
	done
	
	# Go right until number matches
	STEP=1
	while [ $SQUARE -ne $NUMBER ] && [ $STEP -lt $RADIUS ]; do
		((SQUARE = SQUARE - 1))
		((D_X = D_X + 1))
		
		((STEP = STEP + 1))
	done
	
	# Go down until number matches
	STEP=1
	while [ $SQUARE -ne $NUMBER ] && [ $STEP -lt $RADIUS ]; do
		((SQUARE = SQUARE - 1))
		((D_Y = D_Y + 1))
		
		((STEP = STEP + 1))
	done
	
	
	# Calculate distance
	D_SUM=0
	if [ $D_X -gt 0 ]; then
		((D_SUM = D_SUM + D_X))
	else
		((D_SUM = D_SUM - D_X))
	fi
	if [ $D_Y -gt 0 ]; then
		((D_SUM = D_SUM + D_Y))
	else
		((D_SUM = D_SUM - D_Y))
	fi
	
	echo "> $NUMBER: $D_X:$D_Y = $D_SUM"
done
