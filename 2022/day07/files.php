#!/usr/bin/php
<?php

function calc_dir_size($curdir, $path, &$sizes) {
	if($path === '/') {
		$prefix = '/';
	} else {
		$prefix = $path . '/';
	}

	$cursize = 0;
	foreach($curdir as $name => $data) {
		if(!is_array($data)) {
			$cursize += $data;
		} else {
			$cursize += calc_dir_size($data, $prefix . $name, $sizes);
		}
	}
	$sizes[$path] = $cursize;
	return $cursize;
}

$root = [];
$stack = [];
$cwd = &$root;

$in_listing = false;
while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === '') continue;

	if($line[0] === '$') {
		$command = substr($line, 2, 2);
		if($command === 'cd') {
			$dest = substr($line, 5);
			if($dest === '/') {
				$stack = [];
				$cwd = &$root;
			} else if($dest === '..') {
				if(empty($stack)) {
					fprintf(STDERR, "Error: Found \"cd ..\", but we're already in root directory\n");
					die(1);
				}
				array_pop($stack);
				$cwd = &$root;
				for($i = 0; $i < count($stack); ++$i) $cwd = &$cwd[$stack[$i]];
			} else {
				if(!array_key_exists($dest, $cwd)) {
					fprintf(STDERR, "Error: found \"cd %s\", but no such directory exists\n", $dest);
					exit(1);
				}
				$stack[] = $dest;
				$cwd = &$cwd[$dest];
			}
			$in_listing = false;
		} else if($command === 'ls') {
			$in_listing = true;
		} else {
			fprintf(STDERR, "Error: Unknown command \"%s\"\n", $command);
			die(1);
		}
	} else if($in_listing) {
		[$size, $name] = explode(' ', $line, 2);
		if(array_key_exists($name, $cwd)) {
			fprintf(STDERR, "Error: duplicate file /%s%s\n", implode('/', $stack), $name);
			exit(1);
		}
		if($size === 'dir') {
			$cwd[$name] = [];
		} else {
			$size = (int)$size;
			$cwd[$name] = $size;
		}
	} else {
		fprintf(STDERR, "Error: Found a listing entry, but we're not in ls mode\n");
		die(1);
	}
}

$sizes = [];
calc_dir_size($root, '/', $sizes);

// -- Part 1

$THRESHOLD = 100000;
$p1_sum = 0;
$p1_list = [];
foreach($sizes as $path => $size) {
	if($size <= $THRESHOLD) {
		$p1_list[] = $path;
		$p1_sum += $size;
	}
}
echo "Part 1: ", $p1_sum, " (\"", implode("\" + \"", $p1_list), "\")\n";

// -- Part 2

$TOTAL  = 70000000;
$NEEDED = 30000000;

$free = $TOTAL - $sizes['/'];
$want = $NEEDED - $free;

$smol_path = '';
$smol_size = PHP_INT_MAX;
foreach($sizes as $path => $size) {
	if(($size >= $want) && ($size < $smol_size)) {
		$smol_path = $path;
		$smol_size = $size;
	}
}
echo "Part 2: ", $smol_size, " (\"", $smol_path, "\")\n";
