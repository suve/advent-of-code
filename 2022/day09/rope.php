#!/usr/bin/php
<?php

class Position {
	public $x;
	public $y;

	public function __construct($x, $y) {
		$this->x = $x;
		$this->y = $y;
	}

	public function follow(Position $other) {
		$distX = abs($other->x - $this->x);
		$distY = abs($other->y - $this->y);
		if(($distX < 2) && ($distY < 2)) return;

		if($distX !== 0) {
			$this->x += ($other->x > $this->x) ? +1 : -1;
		}
		if($distY !== 0) {
			$this->y += ($other->y > $this->y) ? +1 : -1;
		}
	}
}

class History {
	private $points;
	private $count;

	public function __construct() {
		$this->points = [];
		$this->count = 0;
	}

	public function track(Position $p) {
		if(!array_key_exists($p->y, $this->points)) $this->points[$p->y] = [];

		if(!array_key_exists($p->x, $this->points[$p->y])) {
			$this->points[$p->y][$p->x] = 1;
			$this->count += 1;
		}
	}

	public function getCount(): int {
		return $this->count;
	}
}

define("SEGMENTS", 10);
$seg = [];
for($i = 1; $i <= SEGMENTS; ++$i) $seg[$i] = new Position(0, 0);

$p1 = new History();
$p1->track($seg[2]);
$p2 = new History();
$p2->track($seg[SEGMENTS]);

while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === "") continue;

	$moveX = 0;
	$moveY = 0;

	$dir = $line[0];
	$steps = (int)substr($line, 2);
	if($dir === "U") {
		$moveY = +1;
	} else if ($dir === "D") {
		$moveY = -1;
	}
	if($dir === "R") {
		$moveX = +1;
	} else if($dir === "L") {
		$moveX = -1;
	}

	for($i = 0; $i < $steps; ++$i) {
		$seg[1]->x += $moveX;
		$seg[1]->y += $moveY;

		for($s = 2; $s <= SEGMENTS; ++$s) {
			$seg[$s]->follow($seg[$s-1]);
		}
		$p1->track($seg[2]);
		$p2->track($seg[SEGMENTS]);
	}
}

echo "Part 1: ", $p1->getCount(), "\n";
echo "Part 2: ", $p2->getCount(), "\n";
