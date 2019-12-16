package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// golang doesn't support arrays as consts, so a global var will have to do
var BasePattern = [...]int8{0, 1, 0, -1}

func CalcDigit(digits []uint8, position int) uint8 {
	repeats := position + 1
	var sum int64 = 0

	first := true

	i := 0
	digit_count := len(digits)
	for {
		for bp := 0; bp < len(BasePattern); bp += 1 {
			var start int
			if first {
				start = 1
				first = false
			} else {
				start = 0
			}

			for r := start; r < repeats; r += 1 {
				// fmt.Printf("%d*%d ", digits[i], BasePattern[bp])
				sum += int64(int8(digits[i]) * BasePattern[bp])

				i += 1
				if i == digit_count {
					// fmt.Printf("= %d\n", sum)
					if sum < 0 { sum = -sum }
					return uint8(sum % 10)
				}
			}
		}
	}
}

func CalcPhase(digits []uint8) []uint8 {
	digit_count := len(digits)

	result := make([]uint8, digit_count, digit_count)
	for d := 0; d < digit_count; d += 1 {
		result[d] = CalcDigit(digits, d)
	}

	return result
}

func PrintPhase(digits []uint8) {
	digit_count := len(digits)
	for d := 0; d < digit_count; d += 1 {
		fmt.Printf("%c", digits[d] + 48) // '0' is ASCII 48
	}
	fmt.Print("\n")
}

func ParseArgs(phases, verbose *int) {
	var err error

	argc := len(os.Args)
	if argc == 1 {
		fmt.Fprint(os.Stderr, "Usage: fft [--verbose] --phases NUMBER\n")
		os.Exit(1)
	}

	*phases = 0
	*verbose = 0
	for i := 1; i < argc; {
		if os.Args[i] == "--verbose" {
			*verbose = 1
			i += 1
		} else if os.Args[i] == "--phases" {
			*phases, err = strconv.Atoi(os.Args[i+1])
			if err != nil {
				fmt.Fprintf(os.Stderr, "Invalid number of phases: %s\n", err.Error())
				os.Exit(1)
			}
			i += 2
		} else {
			fmt.Fprintf(os.Stderr, "Unknown option \"%s\"\n", os.Args[i])
			os.Exit(1)
		}
	}
}

func main() {
	var phases, verbose int
	ParseArgs(&phases, &verbose)

	reader := bufio.NewReader(os.Stdin)
	for {
		line, err := reader.ReadString('\n')
		if err != nil {
			break
		}

		line = strings.TrimSpace(line)
		line_len := len(line)
		digits := make([]uint8, line_len, line_len)
		for i := 0; i < len(line); i += 1 {
			digits[i] = line[i] - 48 // '0' is ASCII 48
		}

		if verbose != 0 {
			fmt.Printf("Input signal: ")
			PrintPhase(digits)
		}

		for p := 0; p < phases; p += 1 {
			digits = CalcPhase(digits)

			if verbose != 0 {
				fmt.Printf("After phase %d: ", p+1)
				PrintPhase(digits)
			}
		}

		if verbose == 0 {
			fmt.Printf("After phase %d: ", phases)
			PrintPhase(digits)
		}
	}
}