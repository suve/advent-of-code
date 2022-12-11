#!/usr/bin/php
<?php

interface Operation {
	function calc(int $old): int;
}

class SquareOp implements Operation {
	function calc(int $old): int {
		return $old * $old;
	}
}

class MultiplyOp implements Operation {
	private $value;

	public function __construct($value) {
		$this->value = (int)$value;
	}

	function calc(int $old): int {
		return $old * $this->value;
	}
}

class AddOp implements Operation {
	private $value;

	public function __construct($value) {
		$this->value = (int)$value;
	}

	function calc(int $old): int {
		return $old + $this->value;
	}
}

class Monke {
	public $items;
	public $op;
	public $div_by;
	public $if_true;
	public $if_false;
	public $inspection_count;

	public function __construct($items, $op, $div_by, $if_true, $if_false) {
		$this->items = $items;
		$this->op = $op;
		$this->div_by = (int)$div_by;
		$this->if_true = (int)$if_true;
		$this->if_false = (int)$if_false;
		$this->inspection_count = 0;
	}
}

function must_read_line() {
	$line = fgets(STDIN);
	if($line === false) {
		fprintf(STDERR, "Error: Unexpected end of input\n");
		die(1);
	}
	return rtrim($line);
}

function parse_single_monkey() {
	// Allow any number of empty lines before monkeys
	do {
		$line = fgets(STDIN);
		if($line === false) return null;

		$line = trim($line);
	} while($line === "");

	if(substr($line, 0, 7) !== "Monkey ") {
		fprintf(STDERR, "Error: Expected monkey identifier line, got something else\n");
		die(1);
	}
	// Ideally the monkey's ID should be parsed here,
	// but they're all ordered by ID in the input, so whatever

	$line = must_read_line();
	if(substr($line, 0, 18) !== "  Starting items: ") {
		fprintf(STDERR, "Error: Expected list of starting items, got something else\n");
		die(1);
	}
	$items = array_map('intval', explode(", ", substr($line, 18)));

	$line = must_read_line();
	if(substr($line, 0, 23) !== "  Operation: new = old ") {
		fprintf(STDERR, "Error: Expected operation specification, got something else\n");
		die(1);
	}

	$args = explode(' ', substr($line, 23));
	if($args[0] === '*') {
		if($args[1] === "old") {
			$op = new SquareOp();
		} else {
			$op = new MultiplyOp(intval($args[1]));
		}
	} else if($args[0] === '+') {
		$op = new AddOp(intval($args[1]));
	} else {
		fprintf(STDERR, "Error: Unrecognized operation \"%s\" (expected \"*\" or \"+\")\n", $args[0]);
		die(1);
	}

	$line = must_read_line();
	if(substr($line, 0, 21) !== "  Test: divisible by ") {
		fprintf(STDERR, "Error: Expected division test specification, got something else\n");
		die(1);
	}
	$div_by = intval(substr($line, 21));

	$line = must_read_line();
	if(substr($line, 0, 29) !== "    If true: throw to monkey ") {
		fprintf(STDERR, "Error: Expected true-action specification, got somthing else\n");
		die(1);
	}
	$if_true = intval(substr($line, 29));

	$line = must_read_line();
	if(substr($line, 0, 30) !== "    If false: throw to monkey ") {
		fprintf(STDERR, "Error: Expected false-action specification, got somthing else\n");
		die(1);
	}
	$if_false = intval(substr($line, 30));

	return new Monke($items, $op, $div_by, $if_true, $if_false);
}

function parse_input() {
	$monkeys = [];
	while(($monke = parse_single_monkey()) !== null) $monkeys[] = $monke;
	return $monkeys;
}

# -- start script proper

$monkeys = parse_input();
for($round = 1; $round <= 20; ++$round) {
	foreach($monkeys as $idx => $monke) {
		foreach($monke->items as $item) {
			$item = $monke->op->calc($item);
			$item = floor($item / 3);
			$dest = (($item % $monke->div_by) == 0) ? $monke->if_true : $monke->if_false;
			$monkeys[$dest]->items[] = $item;
		}
		$monkeys[$idx]->inspection_count += count($monke->items);
		$monkeys[$idx]->items = [];
	}
}

$ic = [];
foreach($monkeys as $idx => $monke) {
	echo "Monkey ", $idx, " inspected ", $monke->inspection_count, " items.\n";
	$ic[] = $monke->inspection_count;
}
rsort($ic, SORT_NUMERIC);
echo "Part 1: ", $ic[0] * $ic[1], "\n";
