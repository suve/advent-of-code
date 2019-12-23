#!/usr/bin/php
<?php

function read_file($path) {
	$data = file_get_contents($path);
	if($data === FALSE) {
		fprintf(STDERR, "Failed to read file \"%s\"\n", $data);
		exit(1);
	}
	return $data;
}

function read_json_file($path) {
	$data = read_file($path);
	$data = json_decode($data, true);
	if(json_last_error() != JSON_ERROR_NONE) {
		fprintf(STDERR, "Failed to read JSON from file \"%s\": %s\n", json_last_error_msg());
		exit(1);
	}
	return $data;
}


chdir(__DIR__);

$languages = read_json_file('languages.json');
foreach($languages as $ext => $lang) {
	if(!file_exists("$ext.png")) {
		fprintf(STDERR, "Language \"$lang\" (.$ext) is missing its icon (file \"$ext.png\" not found)\n");
		exit(1);
	}
}

$solutions = read_json_file('solutions.json');
$header = read_file('header.md');

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
	
	echo "| **$daySymbol** |";
	
	foreach($solutions as $year => $files) {
		if($i >= count($files)) {
			echo ' |';
			continue;
		}

		$file = $files[$i];
		if($file === NULL) {
			echo ' |';
			continue;
		}
		
		$ext = pathinfo($file, PATHINFO_EXTENSION);
		if(!isset($languages[$ext])) {
			fprintf(STDERR, "Unknown file extension \"%s\" (file: \"%s\")\n", $ext, $file);
			exit(1);
		}

		$lang = $languages[$ext];
		$path = $year . '/day' . ($dayNo < 10 ? '0' : '') . $dayNo . '/' . $file;
		
		echo " [![$lang](https://github.com/suve/advent-of-code/raw/master/.readme/$ext.png)]($path) |";
	}
	echo "\n";
}
