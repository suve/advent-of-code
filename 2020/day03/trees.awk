#!/bin/awk -f
function check(line,skip) {
	line_len=length($line);
	if(substr(line,1 + (skip % line_len),1) == "#") {
		return 1;
	}
	return 0;
}
BEGIN {
	count1=0;
	count3=0;
	count5=0;
	count7=0;
	count_half=0;
}
{
	row = NR - 1;
	count1 += check($0, row);
	count3 += check($0, 3*row);
	count5 += check($0, 5*row);
	count7 += check($0, 7*row);
	if(row % 2 == 0) {
		count_half += check($0, row / 2);
	}
}
END {
	printf("r1d1: %d\n", count1);
	printf("r3d1: %d\n", count3);
	printf("r5d1: %d\n", count5);
	printf("r7d1: %d\n", count7);
	printf("r1d2: %d\n", count_half);
	printf("----\nmul: %d\n", count1 * count3 * count5 * count7 * count_half);
}
