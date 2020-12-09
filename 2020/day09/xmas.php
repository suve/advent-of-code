#!/usr/bin/php
<?php

$skip = 25;
if(isset($argv[1])) $skip = (int)$argv[1];
echo "--> Preamble size: $skip\n";

$numbers = array_map(function($x) { return (int)$x; }, explode("\n", file_get_contents("/dev/stdin")));
$count = count($numbers);

// part 1

$previous = [];
for($i = 0; $i < $skip; ++$i) {
	$previous[$numbers[$i]] = 1;
}

$part1_i = null;
$part1_n = null;
for($i = $skip; $i < $count; ++$i) {
	$n = $numbers[$i];

	$match_found = false;
	foreach($previous as $p => $_ignored) {
		$other = $n - $p;
		if(isset($previous[$other])) {
			$match_found = true;
			break;
		}
	}
	
	if(!$match_found) {
		$part1_i = $i;
		$part1_n = $n;
		break;
	}

	$old = $numbers[$i - $skip];
	if($previous[$old] > 1) {
		$previous[$old] -= 1;
	} else {
		unset($previous[$old]);
	}

	if(isset($previous[$n])) {
		$previous[$n] += 1;
	} else {
		$previous[$n] = 1;
	}
}

if($part1_n === null) {
	echo "No suitable numbers found in part1\n";
	exit(1);
}

echo "Part1: $part1_n\n";

// part 2

function p2_cut_end($numbers, $sum, $search) {
	if($sum === $search) return $numbers;
	if($sum < $search) return null;
	
	while(count($numbers) > 2) {
		$sum -= array_pop($numbers);
		if($sum === $search) return $numbers;
		if($sum < $search) return null;
	}

	return null;
}

function p2_cut_start($numbers, $sum, $search) {
	if($sum === $search) return $numbers;
	if($sum < $search) return null;

	while(count($numbers) > 2) {
		$sum -= array_shift($numbers);
		if($sum < $search) return null;

		$res = p2_cut_end($numbers, $sum, $search);
		if($res !== null) return $res;
	}
	return null;
}

function p2(&$numbers, $split_pos) {
	$search = $numbers[$split_pos];

	$first = array_slice($numbers, 0, $split_pos - 1);
	$first_sum = array_reduce($first, function($c, $i) { return $c + $i; }, 0);	

	$first_result = p2_cut_start($first, $first_sum, $search);
	if($first_result !== null) return $first_result;

	$second = array_slice($numbers, $split_pos + 1);
	$second_sum = array_reduce($first, function($c, $i) { return $c + $i; }, 0);

	return p2_cut_start($second_result, $second_sum, $search);
}

function min_max(&$arr, &$min, &$max) {
	$min = $max = $arr[0];

	$count = count($arr);
	for($i = 1; $i < $count; ++$i) {
		if($arr[$i] < $min) {
			$min = $arr[$i];
		} else if($arr[$i] > $max) {
			$max = $arr[$i];
		}
	}
}

$part2 = p2($numbers, $part1_i);
if($part2 === null) {
	echo "No suitable numbers found in part2\n";
	exit(1);
}

$min;
$max;
min_max($part2, $min, $max);

echo "--> Smallest: $min; Largest: $max\n";
echo "Part2: " . ($min + $max) . "\n";
