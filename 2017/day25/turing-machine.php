#!/usr/bin/php
<?php

// Begin with state globals
$TAPE = [];
$CURRENT_POS = 0;
$CHECKSUM = NULL;
$STATE = [];
$CURRENT_STATE = NULL;


// Type definitions
class Action {
	public $value;
	public $movement;
	public $next_state;
	
	public function __construct(int $v, int $m, string $ns) {
		$this->value = $v;
		$this->movement = $m;
		$this->next_state = $ns;
	}
}

class State {
	public $letter;
	public $when_0;
	public $when_1;
	
	public function __construct(string $l, Action $a0, Action $a1) {
		$this->letter = $l;
		$this->when_0 = $a0;
		$this->when_1 = $a1;
	}
}

function str_starts_with($long, $short) {
	return substr($long, 0, strlen($short)) === $short;
}

function parse_state_input($name) {
	$act1 = $act2 = NULL;
	$cval = $a_val = $a_mov = $a_ns = NULL;
	while(1) {
		$line = fgets(STDIN);
		if($line === FALSE) {
			fprintf(STDERR, "Unexpected end of data when parsing state\n");
			die;
		}
		$line = trim($line);
		
		if(str_starts_with($line, 'If the current value is ')) {
			$cval = (int)substr($line, -2, 1);
		} else if(str_starts_with($line, '- Write the value ')) {
			$a_val = (int)substr($line, -2, 1);
		} else if(str_starts_with($line, '- Continue with state ')) {
			$a_ns = substr($line, -2, 1);
		} else if($line === '- Move one slot to the left.') {
			$a_mov = -1;
		} else if($line === '- Move one slot to the right.') {
			$a_mov = +1;
		} else {
			fprintf(STDERR, "parse_state_input(): Unrecognized input: \"%s\"\n", $line);
			die;
		}
		
		if(($cval !== NULL) && ($a_val !== NULL) && ($a_mov !== NULL) && ($a_ns !== NULL)) {
			if($cval === 0)
				$act0 = new Action($a_val, $a_mov, $a_ns);
			else
				$act1 = new Action($a_val, $a_mov, $a_ns);
			
			if(($act0 !== NULL) && ($act1 !== NULL)) {
				return new State($name, $act0, $act1);
			}
			
			$cval = $a_val = $a_mov = $a_ns = NULL;
		}
	}
}

function parse_input() {
	global $STATE;
	global $CURRENT_STATE;
	global $CHECKSUM;
	
	while(1) {
		$line = fgets(STDIN);
		if($line === FALSE) return;
		
		$line = trim($line);
		if(str_starts_with($line, 'Begin in state ')) {
			$CURRENT_STATE = substr($line, -2, 1);
		} else if(str_starts_with($line, 'Perform a diagnostic checksum after ')) {
			$CHECKSUM = (int)preg_replace('/[^0-9]/', '', $line);
		} else if(str_starts_with($line, 'In state ')) {
			$statename = substr($line, -2, 1);
			$STATE[$statename] = parse_state_input($statename);
		} else if($line === '') {
			// nothing to do
		} else {
			fprintf(STDERR, "parse_input(): Unrecognized input: \"%s\"\n", $line);
			die;
		}
	}
}

// Time for program code proper
parse_input();
while($CHECKSUM --> 0) {
	$st = $STATE[$CURRENT_STATE];
	$act;
	
	$cur_val = $TAPE[$CURRENT_POS] ?? 0;
	if($cur_val === 0) {
		$act = $st->when_0;
	} else {
		$act = $st->when_1;
	}
	
	$TAPE[$CURRENT_POS] = $act->value;
	$CURRENT_POS += $act->movement;
	
	$CURRENT_STATE = $act->next_state;
}
echo array_sum($TAPE);
echo "\n";
