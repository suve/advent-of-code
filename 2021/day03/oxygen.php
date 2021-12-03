#!/usr/bin/php
<?php

function calc($lines, $idx, $prefer_zero) {
	if(count($lines) == 1) {
		return array_pop($lines);
	}

	$zero_count=0;
	$one_count=0;
	foreach($lines as $l) {
		if($l[$idx] === '0') {
			$zero_count += 1;
		} else {
			$one_count += 1;
		}
	}

	if($prefer_zero) {
		$select = ($zero_count <= $one_count) ? '0' : '1';
	} else {
		$select = ($one_count >= $zero_count) ? '1' : '0';
	}

	$lines = array_filter($lines, function($l) use ($idx, $select) { return $l[$idx] === $select; });
	return calc($lines, $idx + 1, $prefer_zero);
}

function calc_oxygen($lines) {
	return calc($lines, 0, false);
}

function calc_carbondioxide($lines) {
	return calc($lines, 0, true);
}

$lines = array_filter(
	array_map(
		function($i) { return trim($i); },
		explode("\n", file_get_contents("/dev/stdin"))
	),
	function($i) { return strlen($i) > 0; }
);

$o2_bin = calc_oxygen($lines);
$o2_dec = bindec($o2_bin);
$co2_bin = calc_carbondioxide($lines);
$co2_dec = bindec($co2_bin);

echo "Oxygen: {$o2_bin} ({$o2_dec})\n";
echo "Carbon: {$co2_bin} ({$co2_dec})\n";
echo $o2_dec * $co2_dec, "\n";
