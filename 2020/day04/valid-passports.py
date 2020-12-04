#!/bin/python3

from sys import stdin
import re

all_ok = 0
ok_without_cid = 0

def validate_byr(byr):
	byr = int(byr)
	return (byr >= 1920) and (byr <= 2002)

def validate_iyr(iyr):
	iyr = int(iyr)
	return (iyr >= 2010) and (iyr <= 2020)

def validate_eyr(eyr):
	eyr = int(eyr)
	return (eyr >= 2020) and (eyr <= 2030)

def validate_hgt(hgt):
	regex = "^(1([5-8][0-9]|9[0-3])cm|(59|6[0-9]|7[0-6])in)$"
	return re.match(regex, hgt) != None

def validate_hcl(hcl):
	regex = "^#[0-9a-f]{6}$"
	return re.match(regex, hcl) != None

def validate_ecl(ecl):
	valid_colours = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]
	return ecl in valid_colours

def validate_pid(pid):
	regex = "^[0-9]{9}$"
	return re.match(regex, pid) != None

def validate(passport):
	global all_ok
	global ok_without_cid	

	required_fields = {
		"byr": validate_byr,
		"iyr": validate_iyr,
		"eyr": validate_eyr,
		"hgt": validate_hgt,
		"hcl": validate_hcl,
		"ecl": validate_ecl,
		"pid": validate_pid,
	}

	for field, validator in required_fields.items():
		if not field in passport:
			return
		if validator(passport[field]) != True:
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
			fieldname, value = field.split(":")
			current[fieldname] = value

	if len(current) > 0:
		validate(current)

	print("valid: {}; of those, without cid: {}".format(all_ok, ok_without_cid))


if __name__ == "__main__":
	main()
