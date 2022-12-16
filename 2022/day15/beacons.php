#!/usr/bin/php
<?php

class Coordinates {
	public int $x;
	public int $y;

	public function __construct(int $x, int $y) {
		$this->x = $x;
		$this->y = $y;
	}

	public function distance(Coordinates $other): int {
		return abs($this->x - $other->x) + abs($this->y - $other->y);
	}

	public function equal(Coordinates $other): bool {
		return ($this->x === $other->x) && ($this->y === $other->y);
	}
}

class Sensor {
	public Coordinates $pos;
	public Coordinates $beacon;
	public int $distance;
}

function split_string(string $source, string $split): array {
	$parts = explode($split, $source);
	if(count($parts) !== 2) {
		fprintf(STDERR, "Error: invalid input; string \"%s\" not found\n", $split);
		die(1);
	}
	return $parts;
}

function parse_coords(string $str): Coordinates {
	[$x, $y] = split_string($str, ", ");
	if(!str_starts_with($x, "x=")) {
		fprintf(STDERR, "Error: invalid input; expected x= declaration\n");
		die(1);
	}
	$x = intval(substr($x, 2));

	if(!str_starts_with($y, "y=")) {
		fprintf(STDERR, "Error: invalid input; expected y= declaration\n");
		die(1);
	}
	$y = intval(substr($y, 2));

	return new Coordinates($x, $y);
}

function read_input() {
	$sensors = [];
	while(($line = fgets(STDIN)) !== false) {
		$line = trim($line);
		if($line === "") continue;

		$parts = split_string($line, "Sensor at ");
		$line = $parts[1];

		[$spos, $bpos] = split_string($line, ": closest beacon is at ");
		$s = new Sensor();
		$s->pos = parse_coords($spos);
		$s->beacon = parse_coords($bpos);
		$s->distance = $s->pos->distance($s->beacon);
		$sensors[] = $s;
	}
	return $sensors;
}

# -- read args and input

$sensors = read_input();

$yPos = 2000000;
if(($argc > 1)) $yPos = intval($argv[1]);

# -- script proper

$minX = PHP_INT_MAX;
$maxX = PHP_INT_MIN;
$maxDist = PHP_INT_MIN;
foreach($sensors as $sens) {
	if($sens->pos->x < $minX) $minX = $sens->pos->x;
	if($sens->pos->x > $maxX) $maxX = $sens->pos->x;
	if($sens->distance > $maxDist) $maxDist = $sens->distance;
}

$p1 = 0;

# This is O(n*m), so quite terrible, but eh
for($x = $minX - $maxDist; $x <= $maxX + $maxDist; ++$x) {
	$crd = new Coordinates($x, $yPos);
	foreach($sensors as $sens) {
		if($crd->equal($sens->beacon)) continue 2;
	}
	foreach($sensors as $sens) {
		if($crd->distance($sens->pos) <= $sens->distance) {
			++$p1;
			break;
		}
	}
}
echo "Part 1: ", $p1, "\n";

