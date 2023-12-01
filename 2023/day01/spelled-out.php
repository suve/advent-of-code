#!/usr/bin/php
<?php

function extract_first_last_digit($input) {
	$digits = [];
	$length = strlen($input);
	for($i = 0; $i < $length; ++$i) {
		if((ord($input[$i]) >= 48) && (ord($input[$i] <= 57))) {
			$digits[] = $input[$i];
		}
	}

	$count = count($digits);
	if($count === 0) return "";

	return $digits[0] . $digits[$count-1];
}

function convert_spelled_digits($input) {
	$output = "";
	$digits = [
		"zero",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine"
	];

	$length = strlen($input);
	$pos = 0;
	while($pos < $length) {
		foreach($digits as $digit => $word) {
			$wlen = strlen($word);
			if(substr($input, $pos, $wlen) === $word) {
				$output .= $digit;
				$pos += 1; // $wlen;
				continue 2;
			}
		}
		$output .= $input[$pos];
		$pos += 1;
	}
	return $output;
}

$part1 = 0;
$part2 = 0;
while(true) {
	$line = fgets(STDIN);
	if($line === FALSE) break;

	$line = trim($line);
	if($line === "") continue;

	$digits1 = extract_first_last_digit($line);
	if($digits1 !== "") $part1 += $digits1;

	$digits2 = extract_first_last_digit(convert_spelled_digits($line));
	if($digits2 !== "") $part2 += $digits2;
}
echo "Part1: ", $part1, "\n";
echo "Part2: ", $part2, "\n";
