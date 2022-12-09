#!/usr/bin/php
<?php

$headX = 0;
$headY = 0;
$tailX = 0;
$tailY = 0;

$visited = [];
$visited[$tailY] = [];
$visited[$tailY][$tailX] = 1;
$count = 1;

while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === "") continue;

	$moveX = 0;
	$moveY = 0;

	$dir = $line[0];
	$steps = (int)substr($line, 2);
	if($dir === "U") {
		$moveY = +1;
	} else if ($dir === "D") {
		$moveY = -1;
	}
	if($dir === "R") {
		$moveX = +1;
	} else if($dir === "L") {
		$moveX = -1;
	}

	for($i = 0; $i < $steps; ++$i) {
		$headX += $moveX;
		$headY += $moveY;

		$distX = abs($headX - $tailX);
		$distY = abs($headY - $tailY);
		if(($distX < 2) && ($distY < 2)) continue;

		if($distX === 0) {
			$tailY += ($headY > $tailY) ? +1 : -1;
		} else if($distY === 0) {
			$tailX += ($headX > $tailX) ? +1 : -1;
		} else {
			$tailY += ($headY > $tailY) ? +1 : -1;
			$tailX += ($headX > $tailX) ? +1 : -1;
		}

		if(!array_key_exists($tailY, $visited)) $visited[$tailY] = [];
		if(!array_key_exists($tailX, $visited[$tailY])) {
			$visited[$tailY][$tailX] = 1;
			++$count;
		}
	}
}

echo "Part 1: ", $count, "\n";
