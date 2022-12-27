#!/bin/bash
(
	IFS=$'\n,'
	while read X Y Z; do
		((XX=X+1))
		((YY=Y+1))
		((ZZ=Z+1))

		echo "x=${X}@y=${Y},z=${Z}"
		echo "x=${XX}@y=${Y},z=${Z}"
		echo "y=${Y}@x=${X},z=${Z}"
		echo "y=${YY}@x=${X},z=${Z}"
		echo "z=${Z}@x=${X},y=${Y}"
		echo "z=${ZZ}@x=${X},y=${Y}"
	done
) | sort | uniq -u | wc -l
