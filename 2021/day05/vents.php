#!/usr/bin/php
<?php

function parse_line($l) {
	[$p1, $p2] = explode(' -> ', $l);
	[$x1, $y1] = explode(',', $p1);
	[$x2, $y2] = explode(',', $p2);
	return [(int)$x1, (int)$y1, (int)$x2, (int)$y2];
}

function sgnabs($number) {
	$abs = abs($number);
	$sgn = $number / $abs;
	return [$sgn, $abs];
}

function mark_point(&$map, $x, $y) {
	if(!isset($map[$y])) {
		$map[$y] = [$x => 1];
	} else {
		if(!isset($map[$y][$x])) {
			$map[$y][$x] = 1;
		} else {
			$map[$y][$x] += 1;
		}
	}
}

function mark_line(&$map1, &$map2, $x1, $y1, $x2, $y2) {
	$xStep = 0;
	$yStep = 0;
	$steps = 0;
	if($x1 !== $x2) {
		[$xStep, $steps] = sgnabs($x2 - $x1);
	}
	if($y1 !== $y2) {
		// Lines are guaranteed to be at a 45 degree angle,
		// which means the same number of x-steps and y-steps,
		// so we can safely overwrite whatever's in the "steps" variable
		[$yStep, $steps] = sgnabs($y2 - $y1);
	}
	for($i = 0; $i <= $steps; ++$i) {
		$x = $x1 + ($i * $xStep);
		$y = $y1 + ($i * $yStep);
		if(($xStep == 0) || ($yStep === 0)) {
			mark_point($map1, $x, $y);
		}
		mark_point($map2, $x, $y);
	}
}

function count_points(&$map, $threshold) {
	$count = 0;
	foreach($map as $row) {
		foreach($row as $pt) {
			if($pt >= $threshold) {
				$count += 1;
			}
		}
	}
	return $count;
}

$map1 = [];
$map2 = [];
while(($line = fgets(STDIN)) !== false) {
	[$x1, $y1, $x2, $y2] = parse_line($line);
	mark_line($map1, $map2, $x1, $y1, $x2, $y2);
}

echo "Part1: ", count_points($map1, 2), "\nPart2: ", count_points($map2, 2), "\n";
