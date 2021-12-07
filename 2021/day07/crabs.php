#!/usr/bin/php
<?php

function calc_p1(&$count, $dest) {
	$cost = 0;
	foreach($count as $pos => $val) {
		$cost += $val * abs($dest - $pos);
	}
	return $cost;
}

function calc_p2(&$count, $dest) {
	$cost = 0;
	foreach($count as $pos => $val) {
		$dist = abs($dest - $pos);
		$cost += $val * ($dist * ($dist + 1) / 2);
	}
	return $cost;
}

$count = [];
$positions = [];
while(($line = fgets(STDIN)) !== false) {
	foreach(explode(',', trim($line)) as $num) {
		if(!isset($count[$num]))
			$count[$num] = 1;
		else
			++$count[$num];

		$positions[] = $num;
	}
}

// Part1: move to median

sort($positions);
if(count($positions) % 2) {
	$index = (int)(count($positions) / 2);
	$cost = calc_cost($count, $positions[$index]);
} else {
	$i1 = (int)(count($positions) / 2);
	$c1 = calc_p1($count, $positions[$i1]);

	$i2 = $i1 + 1;
	$c2 = calc_p1($count, $positions[$i2]);

	if($c1 < $c2) {
		$index = $i1;
		$cost = $c1;
	} else {
		$index = $i2;
		$cost = $c2;
	}
}

echo "Part1: move to {$positions[$index]} at cost of {$cost}\n";

// Part2: move to average

$sum = 0;
$total = 0;
foreach($count as $pos => $val) {
	$sum += ($pos * $val);
	$total += $val;
}

$best_pos = null;
$best_cost = PHP_INT_MAX;

$average = round($sum / $total);
for($offset = -1; $offset <= +1; ++$offset) {
	$pos = $average + $offset;
	$cost = calc_p2($count, $pos);

	if($cost < $best_cost) {
		$best_cost = $cost;
		$best_pos = $pos;
	}
}

echo "Part2: move to {$best_pos} at cost of {$best_cost}\n";
