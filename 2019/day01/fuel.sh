#!/bin/bash

SUM=0
while read MASS; do
	FUEL=$(expr '(' "$MASS" / 3 ')' - 2)
	SUM=$(expr "$SUM" + "$FUEL")
done

echo "$SUM"
