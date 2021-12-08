#!/usr/bin/php
<?php

function find_with_count($patterns, $num) {
	$count = ['a' => 0, 'b' => 0, 'c' => 0, 'd' => 0, 'e' => 0, 'f' => 0, 'g' => 0];
	foreach($patterns as $pat) {
		$len = strlen($pat);
		for($i = 0; $i < $len; ++$i) {
			++$count[$pat[$i]];
		}
	}
	
	$result = [];
	foreach($count as $idx => $val) {
		if($val === $num) {
			$result[] = $idx;
		}
	}
	return $result;
}

function find_common(...$patterns) {
	return find_with_count($patterns, count($patterns));
}

function find_unique(...$patterns) {
	return find_with_count($patterns, 1);
}

if(!function_exists('str_contains')) {
    function str_contains($haystack, $needle) {
        return strpos($haystack, $needle) !== false;
    }
}

function decode($pattern, $sig) {
	/*
 	 *   0:      1:      2:      3:      4:
 	 *  aaaa    ....    aaaa    aaaa    ....
 	 * b    c  .    c  .    c  .    c  b    c
 	 * b    c  .    c  .    c  .    c  b    c
 	 *  ....    ....    dddd    dddd    dddd
 	 * e    f  .    f  e    .  .    f  .    f
 	 * e    f  .    f  e    .  .    f  .    f
 	 *  gggg    ....    gggg    gggg    ....
 	 * 
	 *   5:      6:      7:      8:      9:
 	 *  aaaa    aaaa    aaaa    aaaa    aaaa
 	 * b    .  b    .  .    c  b    c  b    c
 	 * b    .  b    .  .    c  b    c  b    c
 	 *  dddd    dddd    ....    dddd    dddd
 	 * .    f  e    f  .    f  e    f  .    f
 	 * .    f  e    f  .    f  e    f  .    f
 	 *  gggg    gggg    ....    gggg    gggg
 	 *
 	 * Observations:
 	 * - a 2-segment pattern must be a "1"
 	 * - a 3-segment pattern must be a "7"
 	 * - a 4-segment pattern must be a "4"
 	 * - a 5-segment pattern can be "2", "3", or "5". Out of these three:
	 *   - segments "c" and "e" are active only in "2"
	 *   - segments "c" and "f" are active only in "3"
	 *   - segments "b" and "f" are active only in "5"
 	 * - a 6-segment pattern can be "0", "6", or "9". Out of these three:
	 *   - the "d" segment is inactive only in "0"
	 *   - the "c" segment is inactive only in "6"
	 *   - the "e" segment is inactive only in "9"
 	 * - a 7-segment pattern must be an "8".
 	 */
	switch(strlen($pattern)) {
		case 2:
			return 1;
		case 3:
			return 7;
		case 4:
			return 4;
		case 5:
			if((str_contains($pattern, $sig['c'])) && (str_contains($pattern, $sig['e']))) {
				return 2;
			}
			if((str_contains($pattern, $sig['c'])) && (str_contains($pattern, $sig['f']))) {
				return 3;
			}
			if((str_contains($pattern, $sig['b'])) && (str_contains($pattern, $sig['f']))) {
				return 5;
			}
			break;
		case 6:
			if(!str_contains($pattern, $sig['d'])) {
				return 0;
			}
			if(!str_contains($pattern, $sig['c'])) {
				return 6;
			}
			if(!str_contains($pattern, $sig['e'])) {
				return 9;
			}
			break;
		case 7:
			return 8;
	}
	return -1;
}

$sum = 0;
while(($line = fgets(STDIN)) !== false) {
	[$input, $output] = explode(' | ', trim($line));
	$input = explode(' ', $input);
	$output = explode(' ', $output);

	/*
 	 * As mentioned before (check the "decode()" function):
 	 * - a 2-segment pattern must be a "1".
 	 * - a 3-segment pattern must be a "7".
 	 * - a 4-segment pattern must be a "4".
 	 * - a 5-segment pattern can be "2", "3", or "5".
 	 * - a 6-segment pattern can be "0", "6" or "9".
 	 * - a 7-segment pattern must be an "8".
 	 */
	$pat1 = null;
	$pat7 = null;
	$pat4 = null;
	$pat235 = [];
	$pat069 = [];
	$pat8 = null;
	foreach($input as $in) {
		switch(strlen($in)) {
			case 2:
				$pat1 = $in;
				break;
			case 3:
				$pat7 = $in;
				break;
			case 4:
				$pat4 = $in;
				break;
			case 5:
				$pat235[] = $in;
				break;
			case 6:
				$pat069[] = $in;
				break;
			case 7:
				$pat8 = $in;
				break;
			default:
				// We should also check if pattern counts match, and if no letters outside a-g are used, but whatever
				fprintf(STDERR, "Invalid input: detected a %d-segment input pattern (\"%s\")\n", len($in), $in);
				exit(1);
		}
	}

	/* 
 	 *   1:      7:   
 	 *  ....    AAAA
 	 * .    c  .    c 
 	 * .    c  .    c 
 	 *  ....    ....  
 	 * .    f  .    f 
 	 * .    f  .    f 
 	 *  ....    ....  
 	 * 
 	 * Check which signal is active in "7", but not in "1".
 	 * This signal must correspond to the "a" segment.
 	 */
	$sig = [];
	$sig['a'] = find_unique($pat1, $pat7)[0];

	/* 
 	 *   2:      3:      5:      4:
 	 *  aaaa    aaaa    aaaa    ....
 	 * .    c  .    c  b    .  b    c
 	 * .    c  .    c  b    .  b    c
 	 *  DDDD    DDDD    DDDD    DDDD
 	 * e    .  .    f  .    f  .    f
 	 * e    .  .    f  .    f  .    f
 	 *  gggg    gggg    gggg    ....
 	 * 
 	 * Check which signal is active in "2", "3", "5" AND in "4".
 	 * This signal must correspond to the "d" segment.
 	 */
	$sig['d'] = find_common($pat4, ...$pat235)[0];

	/* 
 	 *   2:      3:      5:
 	 *  AAAA    AAAA    AAAA
 	 * .    c  .    c  b    .
 	 * .    c  .    c  b    .
 	 *  DDDD    DDDD    DDDD
 	 * e    .  .    f  .    f
 	 * e    .  .    f  .    f
 	 *  GGGG    GGGG    GGGG
 	 * 
 	 * Check which three signals are active in "2", "3" and "5".
 	 * These correspond to the "a", "d" and "g" segments.
 	 * Since we already know the signals for "a" and "d",
 	 * the third signal must correspond to "g".
 	 */
	foreach(find_common(...$pat235) as $common) {
		if(($common !== $sig['a']) && ($common !== $sig['d'])) {
			$sig['g'] = $common;
			break;
		}
	}

	/*
 	 *   0:      6:      9:      1:
 	 *  aaaa    aaaa    aaaa    ....
 	 * b    c  b    .  b    c  .    c
 	 * b    c  b    .  b    c  .    c
 	 *  ....    dddd    dddd    ....
 	 * e    F  e    F  .    F  .    F
 	 * e    F  e    F  .    F  .    F
 	 *  gggg    gggg    gggg    ....
 	 *
 	 * Check which signal is active in "0", "6", "9" AND in "1".
 	 * This signal must correspond to the "f" segment.
 	 */
	$sig['f'] = find_common($pat1, ...$pat069)[0];

	/*
 	 *   1:
 	 *  ....
 	 * .    C
 	 * .    C
 	 *  ....
 	 * .    f
 	 * .    f
 	 *  ....
 	 *
 	 * Since "1" is composed of only two signals,
 	 * and we know which signal corresponds to "f",
 	 * the other signal must correspond to "c".
 	 */
	$sig['c'] = ($pat1[0] === $sig['f']) ? $pat1[1] : $pat1[0];

	/* 
 	 *   4:
 	 *  ....
 	 * B    c
 	 * B    c
 	 *  dddd
 	 * .    f
 	 * .    f
 	 *  ....
 	 * 
 	 * Since "4" is composed of four signals,
 	 * and we know three of them ("c", "d", "f"),
 	 * the last one must correspond to "b".
 	 */
	for($i = 0; $i < 4; ++$i) {
		$s = $pat4[$i];
		if(($s !== $sig['c']) && ($s !== $sig['d']) && ($s !== $sig['f'])) {
			$sig['b'] = $s;
			break;
		}
	}

	/* 
 	 * There is only one unmapped signal left - it must correspond to "e".
 	 */
	$sig['e'] = find_unique("abcdefg", implode('', $sig))[0];

	/*
	 * All done! Time to decode the input and the output.
	 */
	$input = array_map(function($x) use ($sig) { return decode($x, $sig); }, $input);
	$output = array_map(function($x) use ($sig) { return decode($x, $sig); }, $output);
	echo implode(' ', $input), " | ", implode(' ', $output), "\n";

	$out_val = 0;
	foreach($output as $o) {
		$out_val = ($out_val * 10) + $o;
	}
	$sum += $out_val;
}

echo "Sum of all output values: {$sum}\n";
