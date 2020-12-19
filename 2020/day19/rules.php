#!/usr/bin/php
<?php

class Rule {
	public $processed;
	public $value;

	public function __construct($v) {
		$this->processed = false;
		$this->value = $v;
	}
}

function process_rule_string($string) {
	global $rules;

	$alternatives = explode(" | ", $string);
	if(count($alternatives) > 1) {
		$altValues = [];
		foreach($alternatives as $alt) {
			$altValues[] = process_rule_string($alt);
		}
		$newValue = '(?>' . implode('|', $altValues) . ')';
	} else if($string[0] != '"') {
		$newValue = '';
		foreach(explode(" ", $string) as $part) {
			$newValue .= process_rule($part);
		}
	} else {
		// very lazily extract character from "\"x\""
		$newValue = $string[1];	
	}
	return $newValue;
}

function process_rule($index) {
	global $rules;
	if(!$rules[$index]->processed) {
		$rules[$index]->value = process_rule_string($rules[$index]->value);
		$rules[$index]->processed = true;
	}

	return $rules[$index]->value;
}


list($lines, $messages) = explode("\n\n", file_get_contents("/dev/stdin"));

// ===== Part 1 =====

$rules = [];
foreach(explode("\n", $lines) as $line) {
	list($number, $value) = explode(": ", $line);
	$rules[$number] = new Rule($value);
}

process_rule(0);

$regex = '/^' . $rules[0]->value  . '$/';
echo $regex, "\n";

$matches = 0;
foreach(explode("\n", $messages) as $msg) {
	if(preg_match($regex, $msg) === 1) ++$matches;
}

echo "Part1: ", $matches, "\n";

// ===== Part 2 =====

if(!isset($rules[8]) || !isset($rules[11]) || !isset($rules[31]) || !isset($rules[42])) exit;

$rules = [];
foreach(explode("\n", $lines) as $line) {
	list($number, $value) = explode(": ", $line);
	$rules[$number] = new Rule($value);
}

// special handling for rule 8 and 11

process_rule(42);
process_rule(31);

$rules[8]->value = '(' . $rules[42]->value . '){1,}?';
$rules[8]->processed = true;

$rules[11]->value = '((' . $rules[42]->value . ')' . '(|(?2))' . '(' . $rules[31]->value . '))';
$rules[11]->processed = true;

// do the rest as before

process_rule(0);

$regex = '/^' . $rules[0]->value  . '$/';
echo $regex, "\n";

$matches = 0;
foreach(explode("\n", $messages) as $msg) {
	if(preg_match($regex, $msg) === 1) ++$matches;
}

echo "Part2: ", $matches, "\n";
