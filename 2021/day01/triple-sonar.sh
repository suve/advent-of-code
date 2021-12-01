#!/bin/bash

cd "$(dirname "${0}")"
(
	WINDOW=3
	HIST=()
	SUM=0
	for ((ROW=0;ROW<WINDOW;ROW+=1)); do
		read NUM
		((HIST[ROW]=NUM))
		((SUM+=NUM))
	done
	echo "${SUM}"

	((ROW=-1))
	while read NUM; do
		((ROW+=1))
		((SUM=SUM - HIST[ROW % WINDOW] + NUM))
		((HIST[ROW % WINDOW]=NUM))
		echo "${SUM}"
	done
) | ./sonar.sh
