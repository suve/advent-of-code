#!/bin/bash

MINIMUM=382345
MAXIMUM=843167

PART1=$(mktemp "aoc.day04.part1.XXXXXX")
PART2=$(mktemp "aoc.day04.part2.XXXXXX")

seq "$MINIMUM" "$MAXIMUM" | \
	grep -E -e '(11|22|33|44|55|66|77|88|99)' | \
	grep -v -E -e '(10)' | \
	grep -v -E -e '(20|21)' | \
	grep -v -E -e '(30|31|32)' | \
	grep -v -E -e '(40|41|42|43)' | \
	grep -v -E -e '(50|51|52|53|54)' | \
	grep -v -E -e '(60|61|62|63|64|65)' | \
	grep -v -E -e '(70|71|72|73|74|75|76)' | \
	grep -v -E -e '(80|81|82|83|84|85|86|87)' | \
	grep -v -E -e '(90|91|92|93|94|95|96|97|98)' > "$PART1"

REGEX=""
for D in $(seq 1 9); do
	REGEX="${REGEX}|((^|[^$D]{1})$D{2}([^$D]{1}|\$))"
done
REGEX=${REGEX:1:999999}

grep -E -e "$REGEX" < "$PART1" > "$PART2"

wc -l "$PART1"
wc -l "$PART2"

rm "$PART1" "$PART2"
