#!/usr/bin/php
<?php

$stacks = [];
for($i = 1; $i <= 9; ++$i) $stacks[$i] = [];

while(($line = fgets(STDIN)) !== false) {
	for($i = 0; $i < 9; ++$i) {
		$item = substr($line, $i*4, 3);
		$item = trim($item, "[ ]\n");
		if($item !== "") {
			if($item === "1") break 2;			
			$stacks[$i+1][] = $item;
		}
	}
}

for($i = 1; $i <= 9; ++$i) {
	$stacks[$i] = array_reverse($stacks[$i]);
}

$p1 = $stacks;
$p2 = $stacks;

while(($line = fgets(STDIN)) !== false) {
	if(sscanf($line, "move %d from %d to %d", $count, $from, $to) !== 3) continue;

	for($i = 0; $i < $count; ++$i) {
		$p1[$to][] = array_pop($p1[$from]);
	}

	$items = array_splice($p2[$from], -$count);
	array_push($p2[$to], ...$items);
}

function answer($stacks) {
	$result = "";
	for($i = 1; $i <= 9; ++$i) {
		$count = count($stacks[$i]);
		if($count > 0) {
			$result .= $stacks[$i][$count-1];
		} else {
			$result .= " ";
		}
	}
	return $result;
}
echo "Part 1: ", answer($p1), "\n";
echo "Part 2: ", answer($p2), "\n";
