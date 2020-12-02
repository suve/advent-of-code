#!/bin/awk -f
BEGIN {
	FS="[ :-]";
	matches=0;
}
{
	min=$1;
	max=$2;
	lett=$3;
	pass=$5;
	regex=sprintf("^([^%c]*%c[^%c]*){%d,%d}$", lett, lett, lett, min, max);
	if(match(pass, regex)) {
		printf("%s %s\n",pass,regex);
		matches+=1;
	}	
}
END {
	print matches;
}
