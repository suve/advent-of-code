#!/usr/bin/php
<?php

define("RED", 12);
define("GREEN", 13);
define("BLUE", 14);

function parse_line($line) {
	$parts = explode(": ", $line);
	if(count($parts) !== 2) {
		fprintf(STDERR, "Failed to parse line \"%s\"\n", $line);
		exit(1);
	}

	$gameId = intval(substr($parts[0], 5)); // strlen("Game "): 5
	$reveals = [];

	$hands = explode("; ", $parts[1]);
	foreach($hands as $hand) {
		$rev = ['red' => 0, 'green' => 0, 'blue' => 0];
		$cubes = explode(", ", $hand);
		foreach($cubes as $cb) {
			[$number, $colour] = explode(" ", $cb);
			$rev[$colour] = intval($number);
		}
		$reveals[] = $rev;
	}

	return [$gameId, $reveals];
}

$part1 = 0;
$part2 = 0;
while(($line = fgets(STDIN)) !== FALSE) {
	$line = trim($line);
	if($line === "") continue;

	[$gameId, $reveals] = parse_line($line);

	$valid = TRUE;
	$minimum = ['red' => 0, 'green' => 0, 'blue' => 0];
	foreach($reveals as $rev) {
		foreach(['red', 'green', 'blue'] as $colour) {
			if($rev[$colour] > $minimum[$colour]) $minimum[$colour] = $rev[$colour];
		}
		if(($rev['red'] > RED) || ($rev['green'] > GREEN) || ($rev['blue'] > BLUE)) {
			$valid = FALSE;
		}
	}

	if($valid) $part1 += $gameId;

	$power = $minimum['red'] * $minimum['green'] * $minimum['blue'];
	$part2 += $power;
}
echo "Part1: ", $part1, "\n";
echo "Part2: ", $part2, "\n";
