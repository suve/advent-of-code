#!/bin/bash

set -e -u

if [[ "$#" -eq 0 ]]; then
	echo "You must specify the input file as an argument (no stdin support, sorry!)" >&2
	exit 1
fi

if [[ ! -f "$1" ]]; then
	echo "File \"$1\" does not exist" >&2
	exit 1
fi
input=$(realpath "$1")

cd "$(dirname "${BASH_SOURCE[0]}")"
if [[ ! -f "./console" ]]; then
	rustc console.rs
fi

lines=$(wc -l "$input" | cut -f1 -d' ')
tempname=$(mktemp --dry-run "/tmp/aoc.2020.day08.part2.XXXXXX");

for i in $(seq 1 "$lines"); do
	sed -e "${i} s/nop /XXX /" -e "${i} s/jmp /nop /" -e "${i} s/XXX /jmp /" < "$input" > "$tempname"
	./console < "$tempname" 
done

rm "$tempname"
