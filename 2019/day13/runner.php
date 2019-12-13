#!/usr/bin/php
<?php

const INTCODE_PATH = __DIR__ . '/../day09/intcode.py';

const TILES = [
	0 => ' ',
	1 => '█',
	2 => '#',
	3 => '━',
	4 => '•',
];


$descriptorspec = [
	0 => ["file", "/dev/stdin", "r"],
	1 => ["pipe", "w"],
	2 => ["file", "/dev/stderr", "w"],
];

$pipes;
$process = proc_open(INTCODE_PATH . " --input 0", $descriptorspec, $pipes);

if($process === FALSE) {
	fprintf(STDERR, "Failed to launch intcode interpreter\n");
	exit(1);
}

$screen = [];
$maxX = 0;
$maxY = 0;

$output = stream_get_contents($pipes[1]);
echo $output;
echo "\n\n";

$output = explode(',', $output);
for($i = 0; $i < count($output); $i += 3) {
	$x = $output[$i+0];
	$y = $output[$i+1];
	$type = $output[$i+2];

	if(!isset($screen[$y])) $screen[$y] = [];
	if($maxY < $y) $maxY = $y;

	$screen[$y][$x] = $type;
	if($maxX < $x) $maxX = $x;
}

$count = [0, 0, 0, 0, 0];
for($y = 0; $y < $maxY; ++$y) {
	if(!isset($screen[$y])) {
		$count[0] += $maxX;
		echo "\n";
		continue;
	}

	for($x = 0; $x < $maxX; ++$x) {
		if(!isset($screen[$y][$x])) {
			$count[0] += 1;
			echo ' ';
			continue;
		}

		$tile = $screen[$y][$x];
		$count[$tile] += 1;
		echo TILES[$tile];
	}
	echo "\n";
}

echo "\n";
for($t = 0; $t < count($count); ++$t) {
	echo $count[$t] . " x '" . TILES[$t] . "'\n";
}
