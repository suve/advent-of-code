#!/usr/bin/awk -f
BEGIN {
	p1depth=0
	p1pos=0

	p2aim=0
	p2depth=0
	p2pos=0
}
/^forward/ {
	p1pos+=$2
	p2pos+=$2
	p2depth+=$2 * p2aim
}
/^down/ {
	p1depth+=$2
	p2aim+=$2
}
/^up/ {
	p1depth-=$2
	p2aim-=$2
}
END {
	printf("part1: depth %d * pos %d -> %d\n", p1depth, p1pos, p1depth * p1pos);
	printf("part2: depth %d * pos %d -> %d\n", p2depth, p2pos, p2depth * p2pos);
}
