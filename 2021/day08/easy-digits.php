#!/usr/bin/php
<?php

$ones = 0;
$fours = 0;
$sevens = 0;
$eights = 0;
while(($line = fgets(STDIN)) !== false) {
	[$input, $output] = explode(' | ', trim($line));
	$output = explode(' ', $output);
	
	foreach($output as $o) {
		switch(strlen($o)) {
			case 2:
				++$ones;
				break;
			case 3:
				++$sevens;
				break;
			case 4:
				++$fours;
				break;
			case 7:
				++$eights;
				break;
		}
	}
}

echo "{$ones} x 1, {$fours} x 4, {$sevens} x 7, {$eights} x 8\n";
echo "Total: ", ($ones + $fours + $sevens + $eights), "\n";

