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

while(($line = fgets(STDIN)) !== false) {
	if(sscanf($line, "move %d from %d to %d", $count, $from, $to) !== 3) continue;

	for($i = 0; $i < $count; ++$i) {
		$stacks[$to][] = array_pop($stacks[$from]);
	}
}

$part1 = "";
for($i = 1; $i <= 9; ++$i) {
	$count = count($stacks[$i]);
	if($count > 0) {
		$part1 .= $stacks[$i][$count-1];
	} else {
		$part1 .= " ";
	}
}
echo "Part1: {$part1}\n";
