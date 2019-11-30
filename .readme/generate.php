#!/usr/bin/php
<?php

chdir(__DIR__);

$languages = json_decode(file_get_contents('languages.json'), true);
$solutions = json_decode(file_get_contents('solutions.json'), true);

$header = file_get_contents('header.md');

echo $header;

# ---

echo '| Day |';
foreach($solutions as $year => $files) echo " $year |";

echo "\n";

echo '| :--- |';
foreach($solutions as $year => $files) echo " :---: |";

echo "\n";

# ---

for($i = 0; $i < 49; ++$i) {
	$dayNo = floor(($i / 2) + 1);
	$daySymbol = $dayNo . (($i % 2) ? '½' : '');
	
	echo "| $daySymbol |";
	
	foreach($solutions as $year => $files) {
		$file = $files[$i];
		if($file === NULL) {
			echo ' |';
			continue;
		}
		
		$ext = pathinfo($file, PATHINFO_EXTENSION);
		$lang = $languages[$ext];
		
		$path = $year . '/day' . ($dayNo < 10 ? '0' : '') . $dayNo . '/' . $file;
		
		echo " [![$lang](https://github.com/suve/advent-of-code/raw/master/.readme/$ext.png)]($path) |";
	}
	echo "\n";
}
