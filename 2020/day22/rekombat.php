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

function serialize_round($p1, $p2) {
	return implode(',', $p1) . ';' . implode(',', $p2);
}

function new_game(&$p1, &$p2) {
	$history = [];
	
	while(!empty($p1) && !empty($p2)) {
		$current = serialize_round($p1, $p2);
		if(in_array($current, $history)) return 1;
		$history[] = $current;

		$p1_top = array_shift($p1);
		$p1_enough = (count($p1) >= $p1_top);
	
		$p2_top = array_shift($p2);
		$p2_enough = (count($p2) >= $p2_top);
		
		if($p1_enough && $p2_enough) {
			$p1_sub = array_slice($p1, 0, $p1_top);
			$p2_sub = array_slice($p2, 0, $p2_top);

			if(new_game($p1_sub, $p2_sub) === 1) {
				array_push($p1, $p1_top, $p2_top);
			} else {
				array_push($p2, $p2_top, $p1_top);
			}
		} else {	
			if($p1_top > $p2_top) {
				array_push($p1, $p1_top, $p2_top);
			} else if($p2_top > $p1_top) {
				array_push($p2, $p2_top, $p1_top);
			} else {
				echo "Draw! No idea how to proceed.\n";
				exit(1);
			}
		}
	}
	
	return empty($p2) ? 1 : 2;
}

list($p1, $p2) = read_input();

$winner = new_game($p1, $p2);
$windeck = ($winner === 1) ? $p1 : $p2;
echo "Winner(", $winner, "): ", implode(', ', $windeck), "\n";
echo "Part2: ", calc_score($windeck), "\n";
