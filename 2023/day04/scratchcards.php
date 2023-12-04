#!/usr/bin/php
<?php

function str_squeeze($input, $charset = ' ') {
	$len = strlen($input);
	if($len === 0) return "";
	if($len === 1) return $input;

	$output = $input[0];
	$prevChr = $input[0];
 	$prevBad = strstr($charset, $prevChr) !== false;
	for($i = 1; $i < $len; ++$i) {
		$c = $input[$i];
		if(($prevBad) && ($prevChr === $c)) {
			// quack
		} else {
			$output .= $c;
		}
		
		$prevChr = $c;
 		$prevBad = strstr($charset, $prevChr) !== false;
	}
	return $output;
}

function arrayOfString_to_arrayOfNumber($input) {
	$output = [];
	foreach($input as $key => $value) {
		$output[$key] = intval($value);
	}
	return $output;
}

$idx = 0;
$copiesByIdx = [];

$sum = 0;
while(($line = fgets(STDIN)) !== false) {
	$line = str_squeeze(trim($line));
	if($line === "") continue;

	$parts = explode(': ', $line);
	if(count($parts) !== 2) {
		fprintf(STDERR, "Failed to parse line: splitting by ': ' yielded %d parts, expected 2\n", count($parts));
		die(1);
	}

	[$winning, $player] = explode(' | ', $parts[1]);
	if(count($parts) !== 2) {
		fprintf(STDERR, "Failed to parse line: splitting by ' | ' yielded %d parts, expected 2\n", count($parts));
		die(1);
	}

	$winning = arrayOfString_to_arrayOfNumber(explode(' ', $winning));
	$player = arrayOfString_to_arrayOfNumber(explode(' ', $player));

	$value = 0;
	$matches = 0;
	foreach($player as $p) {
		foreach($winning as $w) {
			if($p === $w) {
				$value = ($value === 0) ? 1 : ($value * 2);
				$matches += 1;
			}
		}
	}
	$sum += $value;
	
	if(!isset($copiesByIdx[$idx])) $copiesByIdx[$idx] = 1;
	$copies = $copiesByIdx[$idx];
	// fprintf(STDERR, "Have %d copies of card %d\n", $copies, $idx+1);

	while($matches > 0) {
		if(!isset($copiesByIdx[$idx + $matches])) {
			$copiesByIdx[$idx + $matches] = 1;
		}
		$copiesByIdx[$idx + $matches] += $copies;
		--$matches;
	}
	$idx += 1;
}
echo "Part1: ", $sum, "\n";
echo "Part2: ", array_sum($copiesByIdx), "\n";
