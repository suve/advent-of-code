#!/usr/bin/php
<?php

function read_input() {
	return array_map(
		function($a) {
			$a = explode("\n", trim($a));
			array_shift($a);
			return $a;
		},
		explode("\n\n", file_get_contents("/dev/stdin"))
	);
}

function calc_score($a) {
	$sum = 0;
	
	$count = count($a);
	for($i = 0; $i < $count; ++$i) {
		$sum += ($count - $i) * $a[$i];
	}
	return $sum;
}


list($p1, $p2) = read_input();
while(!empty($p1) && !empty($p2)) {
	$p1_top = array_shift($p1);
	$p2_top = array_shift($p2);
	if($p1_top > $p2_top) {
		$p1[] = $p1_top;
		$p1[] = $p2_top;
	} else if($p2_top > $p1_top) {
		$p2[] = $p2_top;
		$p2[] = $p1_top;
	} else {
		echo "Draw! The instructions didn't specify how to handle this.\n";
		exit(1);
	}
}

$winner = empty($p2) ? $p1 : $p2;
echo "Winner: ", implode(', ', $winner), "\n";
echo "Part1: ", calc_score($winner), "\n";
