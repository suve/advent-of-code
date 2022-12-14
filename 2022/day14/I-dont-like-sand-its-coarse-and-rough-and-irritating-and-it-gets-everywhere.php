#!/usr/bin/php
<?php

function set_point(&$map, $x, $y) {
	if(!array_key_exists($y, $map)) {
		$map[$y] = [$x => 1];
	} else {
		$map[$y][$x] = 1;
	}
}

function get_point(&$map, $x, $y) {
	if(!array_key_exists($y, $map)) {
		return 0;
	}
	if(!array_key_exists($x, $map[$y])) {
		return 0;
	}
	return 1;
}

function parse_input() {
	$map = [];
	$lowest = PHP_INT_MIN;
	while(($line = fgets(STDIN)) !== false) {
		$points = array_map(function($part) { return array_map("intval", explode(",", $part)); }, explode(" -> ", $line));

		[$x, $y] = $points[0];
		if($y > $lowest) $lowest = $y;

		$count = count($points);
		for($i = 1; $i < $count; ++$i) {
			[$endX, $endY] = $points[$i];
			if($endY > $lowest) $lowest = $endY;

			while(1) {
				set_point($map, $x, $y);
				if($endX > $x) ++$x; else
				if($endX < $x) --$x; else
				if($endY > $y) ++$y; else
				if($endY < $y) --$y; else break;
			}
		}
	}

	return [$map, $lowest];
}

function pour_that_sand(&$map, $lowest, $startX, $startY) {
	$x = $startX;
	$y = $startY;

	while(true) {
		if(!get_point($map, $x, $y+1)) {
			++$y;
			if($y > $lowest) return false;
		} else if(!get_point($map, $x-1, $y+1)) {
			--$x; ++$y;
		} else if(!get_point($map, $x+1, $y+1)) {
			++$x; ++$y;
		} else {
			set_point($map, $x, $y);
			return true;
		}
	}
}

# -- script proper

[$map, $lowest] = parse_input();

for($i = 0; pour_that_sand($map, $lowest, 500, 0); ++$i);
echo "Part 1: ", $i, "\n";
