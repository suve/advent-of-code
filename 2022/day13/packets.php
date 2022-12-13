#!/usr/bin/php
<?php

function compare($a, $b) {
	if(is_int($a)) {
		if(is_int($b)) {
			return $a <=> $b;
		} else {
			return compare([$a], $b);
		}
	} else if(is_int($b)) {
		return compare($a, [$b]);
	}

	$countA = count($a);
	$countB = count($b);
	for($i = 0; ; ++$i) {
		if(($i >= $countA)) {
			return ($countA < $countB) ? -1 : 0;
		}
		if($i >= $countB) {
			return +1;
		}
		$cmp = compare($a[$i], $b[$i]);
		if($cmp !== 0) return $cmp;
	}
}

$p1 = [];

$packets = [];
$pairNo = 1;
while(true) {
	$first = fgets(STDIN);
	if($first === false) break;

	$first = trim($first);
	if($first === "") continue; // Allow any number of empty lines

	$second = fgets(STDIN);
	if($second === false) {
		fprintf(STDERR, "Error: Unexpected end of input\n");
		die(1);
	}
	$second = trim($second);

	// yeah yeah, eval() is evil, yadda yadda - fight me, mate
	$first = eval("return " . $first . ";");
	$second = eval("return " . $second . ";");

	if(compare($first, $second) === -1) {
		$p1[] = $pairNo;
	}

	array_push($packets, $first, $second);
	++$pairNo;
}

echo "Part 1: ", array_sum($p1), " (", implode(', ', $p1), ")\n";

# -- part 2

$twoPack = [[2]];
$sixPack = [[6]];

array_push($packets, $twoPack, $sixPack);
usort($packets, "compare");

$twoPos = null;
$sixPos = null;

foreach($packets as $idx => $pack) {
	if(($twoPos === null) && ($pack === $twoPack)) {
		$twoPos = ($idx + 1);
		if($sixPos === null) continue; else break;
	}
	if(($sixPos === null) && ($pack === $sixPack)) {
		$sixPos = ($idx + 1);
		if($twoPos === null) continue; else break;
	}
}

echo "Part 2: ", $twoPos * $sixPos, " (", $twoPos, ", ", $sixPos, ")\n";
