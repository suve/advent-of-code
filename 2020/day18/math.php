#!/usr/bin/php
<?php

const PART_1 = 1;
const PART_2 = 2;

class Expression {
	public $left;
	public $right;
	public $op;

	public function __construct($l, $r = NULL, $o = NULL) {
		$this->left = $l;
		$this->right = $r;
		$this->op = $o;
	}

	public function calc() {
		if($this->op === NULL) {
			return $this->left;
		}
		
		if($this->op === '+') {
			return $this->left->calc() + $this->right->calc();
		}
		if($this->op === '*') {
			return $this->left->calc() * $this->right->calc();
		}

		echo "Expession with \"", $this->op, "\" - this shouldn't be possible\n";
		exit(1);
	}
}

function parse_node($node, $mode) {
	if(gettype($node) === 'integer') {
		return new Expression($node);
	}
	
	if($node->kind !== \ast\AST_BINARY_OP) {
		echo "Unexpected AST node kind: ", ast\get_kind_name($node->kind), "\n";
		exit(1);
	}
	
	if($node->flags === \ast\flags\BINARY_ADD) {
		$op = ($mode === PART_2) ? '*' : '+';
	} elseif($node->flags === \ast\flags\BINARY_MUL) {
		$op = ($mode === PART_2) ? '+' : '*';
	} elseif($node->flags === \ast\flags\BINARY_SUB) {
		$op = '*';
	} else {
		echo "Unexpected AST binary op: ", $node->flags, "\n";
		exit(1);
	}
	
	$left = parse_node($node->children['left'], $mode);
	$right= parse_node($node->children['right'], $mode);
	return new Expression($left, $right, $op);
}

function parse_ast(\ast\Node $ast, $mode) {
	if($ast->kind !== \ast\AST_STMT_LIST) {
		echo "Unexpected AST node kind: ", ast\get_kind_name($ast->kind), "\n";
		exit(1);
	}

	$kids = count($ast->children);
	if($kids !== 1) {
		echo "Expected 1 child one, but found ", $kids, "\n";
		exit(1);
	}

	return parse_node($ast->children[0], $mode);
}

function parse_line($line, $mode) {
	if($mode === PART_1) {
		$line = str_replace('*', '-', $line);
	} else {
		$line = str_replace(['*', '+', '-'], ['-', '*', '+'], $line);
	}

	return parse_ast(\ast\parse_code("<?php {$line};", 70), $mode);
}

// read input

$lines = explode("\n", file_get_contents("/dev/stdin"));
array_pop($lines); // remove empty line at end of array

// part 1

$sum = 0;
foreach($lines as $line) {
	$result = parse_line($line, PART_1)->calc();
	$sum += $result;
	
	fprintf(STDERR, "--> %d\n", $result);
}
echo "Part1: ", $sum, "\n";

// part 2

$sum = 0;
foreach($lines as $line) {
	$result = parse_line($line, PART_2)->calc();
	$sum += $result;
	
	fprintf(STDERR, "--> %d\n", $result);
}
echo "Part2: ", $sum, "\n";
