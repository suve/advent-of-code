#!/usr/bin/php
<?php

function report_is_safe($report) {
	$diff = intval($report[1]) - intval($report[0]);
	if($diff > 0) {
		$dir = +1;
		if($diff > 3) return false;
	} else if($diff < 0) {
		$dir = -1;
		if($diff < -3) return false;
	} else {
		return false;
	}
	
	$count = count($report);
	for($i = 2; $i < $count; ++$i) {
		$diff = intval($report[$i]) - intval($report[$i-1]);
		if(($diff <=> 0) !== $dir) return false;
		if($diff * $dir > 3) return false;
	}

	return true;
}

$safe_count = 0;
while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === '') continue;

	$line = explode(' ', $line);
	$safe_count += report_is_safe($line);
}

echo "Part1: {$safe_count}\n";
