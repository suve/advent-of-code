#!/usr/bin/php
<?php

const STARTING_BAG = 'shiny gold';

$bags = [];
while(($line = fgets(STDIN)) !== FALSE) {
	$line = trim($line, " \t\n\r.");
	$line = str_replace([' bags', ' bag'], ['', ''], $line);
	
	$line = explode(" contain ", $line);
	$container = $line[0];

	if(!isset($bags[$container])) {
		$bags[$container] = [
			'children' => [],
			'parents' => [],
		];
	}	

	if($line[1] === 'no other') continue;

	foreach(explode(", ", $line[1]) as $item) {
		[$number, $type] = explode(" ", $item, 2);
		$bags[$container]['children'][] = [
			'number' => $number,
			'type' => $type,
		];

		if(!isset($bags[$type])) {
			$bags[$type] = [
				'children' => [],
				'parents' => [],
			];
		}
		$bags[$type]['parents'][] = $container;
	}
}


function find_outer_bags(&$bags, $current_bag) {
	$solutions = [];
	foreach($bags[$current_bag]['parents'] as $parent) {
		$solutions = array_merge($solutions, [$parent], find_outer_bags($bags, $parent));
	}
	return $solutions;
}

$solution = array_values(array_unique(find_outer_bags($bags, STARTING_BAG)));
print_r($solution);
echo "possible outer bags: " . count($solution) . "\n";


function count_inner_bags(&$bags, $current_bag) {
	$inner_bags = 0;
	foreach($bags[$current_bag]['children'] as $child) {
		$inner_bags += $child['number'] * (1 + count_inner_bags($bags, $child['type']));
	}
	return $inner_bags;
}
echo "inner bags: " . count_inner_bags($bags, STARTING_BAG) . "\n";
