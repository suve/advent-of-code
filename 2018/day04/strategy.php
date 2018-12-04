#!/usr/bin/php
<?php

date_default_timezone_set('UTC');

define('STARTSHIFT', 1);
define('SLEEP', 2);
define('WAKEUP', 3);

class Event {
	public $line;

	public $type;
	public $guardID;
	public $dt;

	public function __construct($line) {
		$brak = strpos($line, ']');
		$dt = substr(substr($line, 0, $brak), 1);
		$this->dt = new DateTime($dt);

		$this->line = $line;
	}
}

function str_starts_with($string, $prefix) {
	$len = strlen($prefix);
	return substr($string, 0, $len) === $prefix;
}

function find_max_ID(&$array) {
	$maxID = -1;
	$maxValue = -1;
	foreach($array as $ID => $value) {
		if($value > $maxValue) {
			$maxValue = $value;
			$maxID = $ID;
		}
	}

	return $maxID;
}

function find_most_sleepy_minute_for_guard($gID) {
	global $events;
	global $count;

	$minutes = [];
	for($m = 0; $m < 60; ++$m) $minutes[$m] = 0;
	
	for($i = 0; $i < $count; ++$i) {
		$e = $events[$i];
		if($e->type != WAKEUP) continue;
		if($e->guardID != $gID) continue;

		$startMinute = (int)$events[$i-1]->dt->format('i');
		$endMinute = (int)$e->dt->format('i');

		for($m = $startMinute; $m < $endMinute; ++$m) $minutes[$m] += 1;
	}

	$maxID = find_max_id($minutes);
	return ['id' => $maxID, 'val' => $minutes[$maxID]];
}


$events = [];
while(!feof(STDIN)) {
	$line = trim(fgets(STDIN));
	if(empty($line)) continue;

	$events[] = new Event($line);
}
$count = count($events);

usort($events, function($a,$b){ return $a->dt <=> $b->dt; });

$currentGuardID = -1;
$allGuardIDs = [];
for($i = 0; $i < $count; ++$i) {
	$e = $events[$i];

	$brak = strpos($e->line, ']');
	$line = substr($e->line, $brak+2);

	if(str_starts_with($line, 'Guard')) {
		$currentGuardID = preg_replace('/[^0-9]/', '', $line);
		$currentGuardID = (int)$currentGuardID;
		$e->type = STARTSHIFT;
	} else if(str_starts_with($line, 'falls')) {
		$e->type = SLEEP;
	} else if(str_starts_with($line, 'wakes')) {
		$e->type = WAKEUP;
	} else {
		echo "OMG LOL WTF: $line\n";
		die;
	}

	$e->guardID = $currentGuardID;
	$allGuardIDs[$currentGuardID] = 1;
}
$allGuardIDs = array_keys($allGuardIDs);
sort($allGuardIDs);

$sleepByGuard = [];
for($i = 0; $i < $count; ++$i) {
	$e = $events[$i];
	if($e->type != WAKEUP) continue;

	$sleepTime = $events[$i-1]->dt->diff($e->dt);
	$sleepMinutes = $sleepTime->i;
	
	if(!isset($sleepByGuard[$e->guardID])) $sleepByGuard[$e->guardID] = 0;
	$sleepByGuard[$e->guardID] += $sleepMinutes;	
}

$mostSleepyGuard = find_max_ID($sleepByGuard);
echo("-> most sleepy guard: #$mostSleepyGuard\n");


$mostSleepyTime = find_most_sleepy_minute_for_guard($mostSleepyGuard)['id'];
echo("-> most sleepy minute: $mostSleepyTime\n");
echo("---> part1 answer: " . ($mostSleepyGuard * $mostSleepyTime) . "\n");


$bestID = $bestMin = $bestVal = -1;

$id_count = count($allGuardIDs);
for($i = 0; $i < $id_count; ++$i) {
	$gID = $allGuardIDs[$i];
	$minute = find_most_sleepy_minute_for_guard($gID);

	echo("-> guard #$gID sleeps " . $minute['val'] . " times on minute " . $minute['id'] . "\n");

	if($minute['val'] > $bestVal) {
		$bestVal = $minute['val'];
		$bestMin = $minute['id'];
		$bestID = $gID;
	}
}
echo("---> part2: guard #$bestID sleeps $bestVal times on minute $bestMin\n");
echo("---> part2 answer: " . ($bestID * $bestMin) . "\n");

