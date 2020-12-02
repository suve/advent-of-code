#!/bin/awk -f
BEGIN {
	FS="[ :-]";
	matches=0;
}
{
	first=$1;
	second=$2;
	lett=$3;
	pass=$5;
	
	first=substr(pass,first,1);
	second=substr(pass,second,1);
	if((first != second) && ((first == lett) || (second == lett))) {
		printf("%c-%c %s\n",first,second,pass);
		matches+=1;
	}	
}
END {
	print matches;
}
