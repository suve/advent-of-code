#!/usr/bin/php
<?php

function read_node(array &$input) {
	$child_count = array_shift($input);
	$meta_count = array_shift($input);
	
	$children = [];
	for($i = 0; $i < $child_count; ++$i) {
		$children[] = read_node($input);
	}
	
	$meta = [];
	for($i = 0; $i < $meta_count; ++$i) {
		$meta[] = array_shift($input);
	}
	
	return ['child' => $children, 'meta' => $meta];
}

function sum_meta($node) {
	$sum = 0;
	for($i = 0; $i < count($node['child']); ++$i) $sum += sum_meta($node['child'][$i]);
	for($i = 0; $i < count($node['meta']); ++$i) $sum += $node['meta'][$i];
	
	return $sum;
}

function node_value($node) {
	$child_count = count($node['child']);
	if($child_count == 0) {
		$sum = 0;
		for($i = 0; $i < count($node['meta']); ++$i) $sum += $node['meta'][$i];
		return $sum;
	}
	
	$value = 0;
	for($i = 0; $i < count($node['meta']); ++$i) {
		$meta = $node['meta'][$i] - 1;
		if($meta < 0 || $meta >= $child_count) continue;
		$value += node_value($node['child'][$meta]);
	}
	return $value;
}

$input = explode(' ', trim(fgets(STDIN)));
$root = read_node($input);
echo "part1: ", sum_meta($root), "\n";
echo "part2: ", node_value($root), "\n";
