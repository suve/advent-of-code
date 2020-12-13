#!/usr/bin/php
<?php

class Elem {
	public $x;
	public $mod;

	private function __construct($x, $mod) {
		$this->x = $x;
		$this->mod = $mod;
	}

	public static function new($x, $m) {
		return new self($x, $m);
	}

	public static function fromStdin($i, $x) {
		$m = -$i;
		while($m < 0) $m += $x;
		return new self($x, $m);
	}
}

function read_input() {
	$arr = array_map(
		function($x) { return (int)$x; },
		array_filter(
			explode(
				",", 
				explode(
					"\n", 
					file_get_contents("/dev/stdin")
				)[1]
			), 
			function($x){ return $x !== 'x'; }
		)
	);

	$result = [];
	foreach($arr as $i => $x) {
		$result[] = Elem::fromStdin($i, $x);
	}

	return $result;
}

function arr2str($array) {
	return implode(", ", array_map(function($i) { return "(" . $i->x . ";" . $i->mod . ")"; }, $array));
}

function find_greatest_i($array) {
	$greatest_i = 0;
	$greatest = $array[0];
	for($i = 1; $i < count($array); ++$i) {
		if($array[$i]->x > $greatest->x) {
			$greatest = $array[$i];
			$greatest_i = $i;
		}
	}
	return $greatest_i;
}

function transform($array) {
	$greatest = array_splice($array, find_greatest_i($array), 1)[0];

	$new_array = array_map(
		function($item) use ($greatest) {
			for($i = 0; true; ++$i) {
				$now = $greatest->mod + ($i * $greatest->x);
				if(($now % $item->x) === $item->mod) return Elem::new($item->x, $i);
			}
		}, $array);

	return [$greatest, $new_array];
}

function loopty($array) {
	echo "--> ", arr2str($array), "\n";

	if(count($array) == 1) {
		return $array[0]->mod;
	}

	[$elem, $array] = transform($array);
	$sub = loopty($array);

	return ($sub * $elem->x) + $elem->mod;
}

$a = read_input();
echo loopty($a), "\n";
