#!/usr/bin/php
<?php

interface Instruction {
	function getCycleCount(): int;
	function onFinished(&$register): void;
}

class NoopInstruction implements Instruction {
	public function getCycleCount(): int {
		return 1;
	}

	public function onFinished(&$register): void {
		// do nothing
	}
}

class AddxInstruction implements Instruction {
	private $value;

	public function __construct($value) {
		$this->value = $value;
	}

	public function getCycleCount(): int {
		return 2;
	}

	public function onFinished(&$register): void {
		$register += $this->value;
	}
}

$program = [];
while(($line = fgets(STDIN)) !== false) {
	$line = trim($line);
	if($line === "") continue;

	$cmd = substr($line, 0, 4);
	$arg = substr($line, 5);

	if($cmd === "noop") {
		// could have just one instance of noop and re-use it, but whatever
		$program[] = new NoopInstruction();
	} else if($cmd === "addx") {
		$program[] = new AddxInstruction((int)$arg);
	}
}

$X = 1;
$p1 = 0;
$p2 = '';
$p2a = [];

$instr = null;
$instrNo = 0;
$instrCycles = 0;
for($cycleNo = 1; $cycleNo <= 240; ++$cycleNo) {
	if($instr === null) {
		$instr = $program[$instrNo];
		$instrNo = ($instrNo + 1) % count($program);
		$instrCycles = 0;
	}

	if(($cycleNo % 40) === 20) {
		$p1 += $cycleNo * $X;
	}

	$pix = ($cycleNo - 1) % 40;
	$p2 .= (abs($pix - $X) <= 1) ? '#' : '.';
	if($pix == 39) {
		$p2a[] = $p2;
		$p2 = '';
	}

	$instrCycles += 1;
	if($instrCycles === $instr->getCycleCount()) {
		$instr->onFinished($X);
		$instr = null;
	}
}

echo "Part 1: ", $p1, "\n";
echo "Part 2:\n";
foreach($p2a as $row) echo $row, "\n";
