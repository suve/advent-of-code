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

function count_lines_in_file($path) {
	return count(explode("\n", file_get_contents($path)));
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

# -- Data read - begin output

echo $header;

# -- Document header has been printed - output the solution table

echo "\n\n## Solutions\n\n";
echo '| Day |';
foreach($solutions as $year => $files) echo " $year |";

echo "\n";

echo '| :--- |';
foreach($solutions as $year => $files) echo " :---: |";

echo "\n";

# -- Table header printed - output the solution list

$solutionsByLanguage = [];
$linesByLanguage = [];
$linesByFile = [];

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

		$path = $year . "/day" . str_pad($dayNo, 2, '0', STR_PAD_LEFT) . "/" . $file;
		if(!file_exists("../" . $path)) {
			fprintf(STDERR, "File \"%s\" not found\n", $path);
			exit(1);
		}

		$lang = $languages[$ext];
		$path = $year . '/day' . ($dayNo < 10 ? '0' : '') . $dayNo . '/' . $file;
		
		echo " [![$lang](https://github.com/suve/advent-of-code/raw/master/.readme/$ext.png)]($path) |";

		// Collect stats for later printing
		if(!isset($solutionsByLanguage[$lang])) {
			$solutionsByLanguage[$lang] = 0;
			$linesByLanguage[$lang] = 0;
		}
		$solutionsByLanguage[$lang] += 1;

		if(!isset($linesByFile[$path])) {
			$linesByFile[$path] = count_lines_in_file("../" . $path);
			$linesByLanguage[$lang] += $linesByFile[$path];
		}
	}
	echo "\n";
}

# -- Solution list done - output stats

echo "\n\n## Stats\n\n";

echo '| Language | Solutions | Percentage | Lines of code | Percentage |';
echo "\n";

echo '| :---: | ---: | ---: | ---: | ---: |';
echo "\n";

$statLangs = array_keys($solutionsByLanguage);
usort($statLangs, function($a, $b) use ($solutionsByLanguage, $linesByLanguage) {
	if($solutionsByLanguage[$a] > $solutionsByLanguage[$b]) return -1;
	if($solutionsByLanguage[$a] < $solutionsByLanguage[$b]) return +1;

	// If number of solutions is the same, sort by lines of code
	if($linesByLanguage[$a] > $linesByLanguage[$b]) return -1;
	if($linesByLanguage[$a] < $linesByLanguage[$b]) return +1;

	// If LoC count is identical, sort alphabetically
	if($a < $b) return -1;
	if($a > $b) return +1;

	return 0;
});

$solutionsTotal = array_sum($solutionsByLanguage);
$linesTotal = array_sum($linesByLanguage);

foreach($statLangs as $lang) {
	$solutions = $solutionsByLanguage[$lang];
	$solPerc = number_format($solutions * 100 / $solutionsTotal, 1) . '%';

	$lines = $linesByLanguage[$lang];
	$linePerc = number_format($lines * 100 / $linesTotal, 1) . '%';
	if($lines >= 1000) {
		$lines = number_format($lines / 1000, 1) . 'k';
	}

	echo "| $lang | $solutions | $solPerc | $lines | $linePerc |\n";
}

