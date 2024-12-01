#!/usr/bin/php
<?php

$list = [];
$count = [];

while(($result = fscanf(STDIN, "%d %d", $left, $right)) !== false) {
	$list[] = $left;
	if(array_key_exists($right, $count)) {
		$count[$right] += 1;
	} else {
		$count[$right] = 1;
	}
}

$score = 0;
foreach($list as $number) {
	if(array_key_exists($number, $count)) {
		$score += $number * $count[$number];
	}
}

echo "Part2: {$score}\n";
