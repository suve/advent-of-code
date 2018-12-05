#!/usr/bin/php
<?php

function remove(&$string, $pos, $length) {
	$string = substr($string, 0, $pos) . substr($string, $pos+$length);
}

function polymer_strength($input) {
	do {
		$changed = false;
		for($i = 0; $i < 26; ++$i) {
			$smallLetter = chr(97 + $i);
			$largeLetter = chr(65 + $i);

			$rem = $smallLetter.$largeLetter;
			$pos = strpos($input, $rem);
		
			if($pos === FALSE) {
				$rem = $largeLetter.$smallLetter;
				$pos = strpos($input, $rem);
			}

			if($pos !== FALSE) {
				remove($input, $pos, 2);
				$changed = TRUE;

				// echo "--> removed $rem at pos $pos\n";
				break;
			}
		}
	} while($changed);

	return strlen($input);
}

$input = trim(fgets(STDIN));
echo "part1: " . polymer_strength($input) . "\n";

$best = strlen($input);
for($i = 0; $i < 26; ++$i) {
	$smallLetter = chr(97 + $i);
	$largeLetter = chr(65 + $i);
	
	$new_input = preg_replace("/[$smallLetter$largeLetter]/",'',$input);
	$new_strength = polymer_strength($new_input);

	echo "--> part2: $smallLetter$largeLetter: $new_strength\n";

	if($new_strength < $best) $best = $new_strength;
}
echo "part2: $best\n";
