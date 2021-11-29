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

# -- main()

$recipes = parse_input();

$need = ['FUEL' => 1];
$have = [];

while(true) {
	print_list('Need', $need);
	print_list('Have', $have);

	$changed = false;
	foreach($need as $name => $count) {
		if($name === 'ORE') continue;

		// I can't find this mentioned anywhere in the docs,
		// but PHP seems to copy the array for foreach's purposes.
		// What this means is that if, while processing $need['some_early_index']
		// we modify $need['some_late_index'], when the loop gets around
		// to some_late_index, $count will have the value old value
		// back from loop start, not the new, modified value!
		$count = $need[$name];
		if($count === 0) continue;

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

		$changed = true;
	}
	if(!$changed) break;
}

echo 'Need ', $need['ORE'], " ORE to produce 1 FUEL\n";
