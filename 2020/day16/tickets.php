#!/usr/bin/php
<?php

class Range {
	public $min;
	public $max;

	public function __construct($min, $max) {
		$this->min = (int)$min;
		$this->max = (int)$max;
	}

	public function test($value) {
		if($value < $this->min) return false;
		if($value > $this->max) return false;
		return true;
	}
}

class Field {
	public $index;
	public $name;
	public $ranges;

	public function __construct($name, $ranges) {
		$this->index = null;
		$this->name = $name;
		$this->ranges = $ranges;
	}

	public function test($value) {
		$value = (int)$value;
		
		foreach($this->ranges as $r) {
			if($r->test($value)) return true;
		}
		return false;
	}
}

$lines = explode("\n", file_get_contents("/dev/stdin"));

// -- ranges

$fields = [];

$l = 0;
for(; $lines[$l] !== ""; ++$l) {
	list($name, $ranges) = explode(": ", $lines[$l]);
	$ranges = array_map(
		function($x) {
			$x = explode("-", $x);
			return new Range($x[0], $x[1]);
		},
		explode(" or ", $ranges)
	);
	$fields[] = new Field($name, $ranges);
}

$field_count = count($fields);

// -- our ticket

$l += 2;
$our_ticket = array_map('intval', explode(",", $lines[$l]));
$l += 3;

// -- nearby tickets

$tickets = [$our_ticket];
$error_rate = 0;

$count = count($lines) - 1;
for(; $l < $count; ++$l) {
	$ticket_data = array_map('intval', explode(",", $lines[$l]));
	
	$ticket_valid = true;
	foreach($ticket_data as $td) {
		foreach($fields as $f) {
			if($f->test($td)) {
				continue 2;
			}
		}
		$ticket_valid = false;
		$error_rate += $td;
	}
	
	if($ticket_valid) {
		$tickets[] = $ticket_data;
	} else {
	}
}

echo "Part1: ", $error_rate, "\n";

// -- part 2

$unknown_fields = [];
foreach($fields as $index => $f) $unknown_fields[] = $index;

$known_columns = [];

while(!empty($unknown_fields)) {
	foreach($unknown_fields as $f) {
		$possible = [];
		for($col = 0; $col < $field_count; ++$col) {
			if(isset($known_columns[$col])) continue;

			foreach($tickets as $t) {
				if(!$fields[$f]->test($t[$col])) {
					continue 2;
				}
			}

			$possible[] = $col;
		}
		
		if(count($possible) === 1) {
			$col = $possible[0];
			echo "--> Column ", $col, " is field \"", $fields[$f]->name, "\"\n";

			$fields[$f]->index = $col;
			unset($unknown_fields[$f]);

			$known_columns[$col] = true;
			continue 2;
		}
	}

	echo "Unable to match any more fields with columns\n";
	exit(1);
}

echo "--> Our ticket:\n";
foreach($fields as $f) {
	echo "--> * ", $f->name, ": ", $our_ticket[$f->index], "\n";
}

$our_score = 1;
foreach($fields as $f) {
	if(substr($f->name, 0, 9) === "departure") {
		$our_score *= $our_ticket[$f->index];
	}
}

echo "Part2: ", $our_score, "\n";
