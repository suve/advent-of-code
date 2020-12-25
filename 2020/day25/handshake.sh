#!/bin/bash

read card_key
read door_key

card_rounds=0
door_rounds=0

value=1
round_no=0
while [[ "${card_rounds}" -eq 0 ]] || [[ "${door_rounds}" -eq 0 ]]; do
	((round_no=round_no+1))
	((value=(value*7)%20201227))
	
	if [[ "${value}" -eq "${card_key}" ]]; then
		echo "--> Card loop size: ${round_no}" >&2
		((card_rounds=round_no))
	fi
	if [[ "${value}" -eq "${door_key}" ]]; then
		echo "--> Door loop size: ${round_no}" >&2
		((door_rounds=round_no))
	fi
done

if [[ "${card_rounds}" -gt "${door_rounds}" ]]; then
	((subject_no=card_key))
	((rounds_left=door_rounds))
else
	((subject_no=door_key))
	((rounds_left=card_rounds))
fi

value=1
while [[ "${rounds_left}" -gt 0 ]]; do
	((rounds_left=rounds_left-1))
	((value=(value*subject_no)%20201227))
done
echo "Encryption key: ${value}"
