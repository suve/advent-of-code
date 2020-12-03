#!/bin/awk -f
BEGIN {
	x=0;
	safe=0;
	unsafe=0;
}
{
	line_len=length($0);
	if(substr($0,1 + (x % line_len),1) == "#") {
		unsafe += 1;
	} else {
		safe += 1;
	}
	x += 3;
}
END {
	printf("safe: %d; unsafe: %d\n", safe, unsafe);
}
