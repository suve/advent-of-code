#!/usr/bin/php
<?php

$MIN_INSERT = 0;
$MAX_INSERT = 2017;

// $STEP_SIZE = 3;
$STEP_SIZE = 394;

$buffer = [$MIN_INSERT];
$len = 1;
$pos = 0;

for($i = $MIN_INSERT+1; $i <= $MAX_INSERT; ++$i) {
	$pos = ($pos + $STEP_SIZE) % $len++;
	
	array_splice($buffer, ++$pos, 0, [$i]);
}

printf("%d\n", $buffer[($pos + 1) % count($buffer)]);
