#!/usr/bin/php
<?php

$forbidden = [];
for($i = 10; $i <= 99; ++$i) {
	$forbidden[$i] = [];
	for($j = 10; $j <= 99; ++$j) {
		$forbidden[$i][$j] = false;
	}
}

while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === '') break;

	[$left, $right] = explode('|', $line);
	$forbidden[intval($right)][intval($left)] = true;
}

$score = 0;
while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	
	$pages = explode(',', $line);
	foreach($pages as &$value) { $value = intval($value); }
	
	$count = count($pages);
	$valid = true;
	for($i = 0; $i < $count; ++$i) {
		for($j = $i + 1; $j < $count; ++$j) {
			if($forbidden [$pages[$i]] [$pages[$j]]) {
				$valid = false;
				break;
			}
		}
	}
	if($valid) $score += $pages[$count / 2];
}
echo "Part1: {$score}\n";
