#!/bin/bash

empty_seats=($(seq 0 1023))

highest=0
while read bpass; do
	row_min=0
	row_max=127
	for ((i = 0; i < 7; ++i)); do
		if [[ "${bpass:i:1}" == "F" ]]; then
			((row_max-=(row_max-row_min + 1)/2))
		else
			((row_min+=(row_max-row_min + 1)/2))
		fi
	done

	col_min=0
	col_max=7
	for ((i = 7; i < 10; ++i)); do
		if [[ "${bpass:i:1}" == "L" ]]; then
			((col_max-=(col_max-col_min + 1)/2))
		else
			((col_min+=(col_max-col_min + 1)/2))
		fi
	done

	((sid=row_min*8 + col_min))
	echo "${bpass}: row ${row_min}, column ${col_min}, seat ID ${sid}"
	
	if [[ "${sid}" -gt "${highest}" ]]; then
		highest="${sid}"
	fi

	unset empty_seats[sid]
done

echo "Highest seat ID: ${highest}"

echo -n "Empty seats:"
for ((i = 0; i < 1024; ++i)); do
	if [[ ! -z "${empty_seats[i]}" ]]; then
		echo -n " ${i}"
	fi
done
echo
