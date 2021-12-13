#!/usr/bin/php
<?php

class Dot {
	public $x;
	public $y;

	public function __construct($x, $y) {
		$this->x = (int)$x;
		$this->y = (int)$y;
	}

	public function fold($axis, $position) {
		if($axis === 'x') {
			$axisPos =& $this->x;
		} else {
			$axisPos =& $this->y;
		}
		if($axisPos > $position) {
			$axisPos = $position - ($axisPos - $position);
		}
	}

	public function __toString() {
		return "{$this->x}:{$this->y}";
	}
}

$dots = [];
while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === "") break;

	$dots[] = new Dot(...explode(',', $line));
}

$folds = [];
while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if(substr($line, 0, 11) !== "fold along ") {
		fprintf(STDERR, "Invalid input\n");
		exit(1);
	}
	
	$line = substr($line, 11);
	$line = explode('=', $line);
	$folds[] = [
		strtolower($line[0]), 
		(int)$line[1]
	];
}

echo "Before folding: ", count($dots), " dots\n";
foreach($folds as [$axis, $value]) {
	foreach($dots as &$dot) {
		$dot->fold($axis, $value);
	}
	$dots = array_unique($dots, SORT_STRING);
	echo "After folding on {$axis}={$value}: ", count($dots), " dots\n";
}

$maxX = $maxY = 0;
$minX = $minY = PHP_INT_MAX;
foreach($dots as $dot) {
	if($dot->x < $minX) $minX = $dot->x;
	if($dot->x > $maxX) $maxX = $dot->x;
	if($dot->y < $minY) $minY = $dot->y;
	if($dot->y > $maxY) $maxY = $dot->y;
}
$width = ($maxX - $minX + 1);
$height = ($maxY - $minY + 1);

$image = [];
for($y = 0; $y < $height; ++$y) {
	$image[$y] = str_repeat('.', $width);
}
foreach($dots as $dot) {
	$x = $dot->x - $minX;
	$y = $dot->y - $minY;
	$image[$y][$x] = '#';
}

foreach($image as $row) {
	echo $row, "\n";
}
