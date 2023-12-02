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

$sum = 0;
while(($line = fgets(STDIN)) !== FALSE) {
	$line = trim($line);
	if($line === "") continue;

	[$gameId, $reveals] = parse_line($line);
	foreach($reveals as $rev) {
		if(($rev['red'] > RED) || ($rev['green'] > GREEN) || ($rev['blue'] > BLUE)) {
			continue 2;
		}
	}
	$sum += $gameId;
}
echo "Part1: ", $sum, "\n";
