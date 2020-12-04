#!/bin/python3

from sys import stdin

all_ok = 0
ok_without_cid = 0

def validate(passport):
	global all_ok
	global ok_without_cid	

	required_fields = [
		"byr", "iyr", "eyr",
		"hgt", "hcl", "ecl",
		"pid",
	];

	for field in required_fields:
		if not field in passport:
			return
	
	all_ok += 1
	if "cid" in passport:
		ok_without_cid += 1


def main():
	current = {}
	for line in stdin:
		line = line.strip()
		if line == "":
			validate(current)
			current = {}
			continue

		for field in line.split(" "):
			fieldname = field.split(":")[0]
			current[fieldname] = True

	if len(current) > 0:
		validate(current)

	print("valid: {}; of those, without cid: {}".format(all_ok, ok_without_cid))


if __name__ == "__main__":
	main()
