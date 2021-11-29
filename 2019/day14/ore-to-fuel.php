#!/usr/bin/php
<?php

function parse_input() {
	$recipes = [];
	while(($line = fgets(STDIN)) !== false) {
		$line = trim($line);
		if($line === '') continue;

		[$input, $output] = explode('=>', $line);
		[$out_count, $out_name] = str_to_ingredient($output);

		$inputs = [];
		foreach(explode(',', $input) as $i) {
			[$count, $name] = str_to_ingredient($i);
			$inputs[$name] = $count;
		}

		$recipes[$out_name] = [
			'count' => $out_count,
			'input' => $inputs,
		];
	}
	return $recipes;
}

function str_to_ingredient($str) {
	$result = explode(' ', trim($str));
	$result[0] = (int)$result[0];
	return $result;
}

function print_list($header, $list) {
	$fmt = [];
	foreach($list as $name => $count) {
		if($count > 0) {
			$fmt[] = $count . ' ' . $name;
		}
	}

	if(count($fmt) > 0) {
		echo $header, ': ', implode(', ', $fmt), "\n";
	} else {
		echo $header, ": (nothing)\n";
	}
}

function use_recipe($recipes, $name, &$need, &$have) {
	if($name === 'ORE') return false;

	$count = $need[$name];
	if($count === 0) return false;

	$reactions = ceil($count / $recipes[$name]['count']);
	foreach($recipes[$name]['input'] as $in_name => $in_count) {
		$in_need = $reactions * $in_count;
		if(isset($have[$in_name])) {
			if($have[$in_name] >= $in_need) {
				$have[$in_name] -= $in_need;
				$in_need = 0;
			} else {
				$in_need -= $have[$in_name];
				$have[$in_name] = 0;
			}
		}

		if($in_need > 0) {
			if(!isset($need[$in_name])) {
				$need[$in_name] = 0;
			}
			$need[$in_name] += $in_need;
		}
	}

	if(!isset($have[$name])) {
		$have[$name] = 0;
	}
	$have[$name] += ($reactions * $recipes[$name]['count']) - $count;
	$need[$name] = 0;

	return true;
}

function calc_ore_for_fuel($recipes, $fuel_count, $print = false) {
	$need = ['FUEL' => $fuel_count];
	$have = [];

	while(true) {
		if($print) {
			print_list('Need', $need);
			print_list('Have', $have);
		}

		$changed = false;
		foreach($need as $name => $_) {
			$changed = $changed || use_recipe($recipes, $name, $need, $have);
		}
		if(!$changed) break;
	}
	return $need['ORE'];
}

# -- part 1

$recipes = parse_input();
$ore = calc_ore_for_fuel($recipes, 1, true);
echo '=> Part 1: need ', $ore, " ORE to produce 1 FUEL\n";

# -- part 2
# the "ore cost for X units of fuel" calculation is quite cheap,
# so just binary-search for the answer

const TRILLION = 1000000000000;

$min_fuel = (int)floor(TRILLION / $ore); // Naive answer
$max_fuel = $min_fuel * 2; // Gotta guess somehow

while($min_fuel !== $max_fuel) {
	$mid_fuel = (int)ceil(($min_fuel + $max_fuel) / 2); // Round up because we want to prefer lowering max over raising min
	$mid_cost = calc_ore_for_fuel($recipes, $mid_fuel);

	printf('Checking fuel cost for range %d - %d: need %13d ORE for %d FUEL%s', $min_fuel, $max_fuel, $mid_cost, $mid_fuel, "\n");
	if($mid_cost <= TRILLION) {
		$min_fuel = $mid_fuel;
	} else {
		$max_fuel = $mid_fuel - 1;
	}
}
echo '=> Part 2: need ', calc_ore_for_fuel($recipes, $min_fuel), ' ORE to produce ', $min_fuel, " FUEL\n";

