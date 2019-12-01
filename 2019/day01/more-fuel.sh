#!/bin/bash

function calc_required_fuel() {
	local mass="$1"
	local fuel=$(expr '(' "$mass" / 3 ')' - 2)

	if [ "$fuel" -ge 9 ]; then
		local more_fuel=$(calc_required_fuel "$fuel")
		echo $(expr "$fuel" + "$more_fuel")
	else
		echo "$fuel"
	fi
}

SUM=0
while read MASS; do
	SUM=$(expr "$SUM" + $(calc_required_fuel "$MASS"))
done

echo "$SUM"
