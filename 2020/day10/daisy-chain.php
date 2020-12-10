#!/usr/bin/php
<?php

$numbers = array_map(function($x) { return (int)$x; }, explode("\n", file_get_contents("/dev/stdin")));

// Add 0 (starting point) to the array
$numbers[] = 0;

// Reverse-sort the array
sort($numbers);
$numbers = array_reverse($numbers);

// Create an array that holds the number of possible arrangements for each starting point.
// Note this part of the puzzle description:
// > any given adapter can take an input 1, 2, or 3 jolts lower
// > than its rating and still produce its rated output joltage
// and this part:
// > your device has a built-in joltage adapter rated for 3 jolts higher
// > than the highest-rated adapter in your bag
// combined, these mean that the highest-rated adapter in our bag must be the finish point.
// Take the highest-rated adapter and put 1 as the possible number of arrangements.
$arrangement_count = [];
$largest = array_shift($numbers);
$arrangement_count[$largest] = 1;

// For each joltage rating, treat it as the starting point.
// The only valid steps from each points are going up +1, +2, or +3 jolts.
// This means, for each point, the number of possible arrangements
// is simply the sum of possible arrangements of points +1, +2, and +3.
foreach($numbers as $num) {
	$a = 0;
	for($i = 1; $i <= 3; ++$i) {
		if(isset($arrangement_count[$num+$i])) {
			$a += $arrangement_count[$num+$i];
		}
	}
	$arrangement_count[$num] = $a;
}

// Our answer is the number of possible arrangements when going from 0.
echo $arrangement_count[0], "\n";
