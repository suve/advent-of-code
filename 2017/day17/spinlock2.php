#!/usr/bin/php
<?php

$MIN_INSERT = 0;
$MAX_INSERT = 50000000;

$STEP_SIZE = 3;
$STEP_SIZE = 394;

$bufpos1 = NULL;
$len = 1;
$pos = 0;

for($i = $MIN_INSERT+1; $i <= $MAX_INSERT; ++$i) {
	$pos = ($pos + $STEP_SIZE) % $len++;
	
	if(++$pos == 1) $bufpos1 = $i;
}

printf("after 0: %d\n", $bufpos1);
