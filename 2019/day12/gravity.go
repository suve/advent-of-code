package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type Vector struct {
	x, y, z int
}

type Moon struct {
	pos, vel Vector
}

func ReadInput(moon *[]Moon) {
	reader := bufio.NewReader(os.Stdin)

	for {
		line, err := reader.ReadString('\n')
		if err != nil { break }

		var x, y, z int
		fmt.Sscanf(line, "<x=%d, y=%d, z=%d>", &x, &y, &z)

		*moon = append(*moon, Moon{
			pos: Vector{
				x: x, y: y, z: z,
			},
			vel: Vector{
				x: 0, y: 0, z: 0,
			},
		})
	}
}

func ApplyGravity(posA, posB int, velA, velB *int) {
	if posA > posB {
		*velA -= 1
		*velB += 1
	} else if posA < posB {
		*velA += 1
		*velB -= 1
	}
}

func Step(moon *[]Moon) {
	length := len(*moon)
	for i := 0; i < length-1; i += 1 {
		mi := &(*moon)[i]
		for j := i+1; j < length; j += 1 {
			mj := &(*moon)[j]

			ApplyGravity(mi.pos.x, mj.pos.x, &mi.vel.x, &mj.vel.x)
			ApplyGravity(mi.pos.y, mj.pos.y, &mi.vel.y, &mj.vel.y)
			ApplyGravity(mi.pos.z, mj.pos.z, &mi.vel.z, &mj.vel.z)
		}
	}

	for i := 0; i < length; i += 1 {
		m := &(*moon)[i]
		m.pos.x += m.vel.x
		m.pos.y += m.vel.y
		m.pos.z += m.vel.z
	}
}

func Abs(x int) int {
	if x < 0 {
		return -x
	} else {
		return +x
	}
}

func CalculateEnergy(moon *[]Moon) int {
	sum := 0

	length := len(*moon)
	for i := 0; i < length; i += 1 {
		m := &(*moon)[i]

		pot := Abs(m.pos.x) + Abs(m.pos.y) + Abs(m.pos.z)
		kin := Abs(m.vel.x) + Abs(m.vel.y) + Abs(m.vel.z)
		total := pot * kin

		sum += total
	}

	return sum
}

func ParseArgs(steps *int) {
	var err error

	argc := len(os.Args)
	if argc < 3 {
		fmt.Fprint(os.Stderr, "You must provide the number of --steps\n")
		os.Exit(1)
	}

	if os.Args[1] == "--steps" {
		*steps, err = strconv.Atoi(os.Args[2])
		if err != nil {
			fmt.Fprintf(os.Stderr, "Bad number of steps: %s\n", err.Error())
			os.Exit(1)
		}
	}
}

func main() {
	var steps int
	ParseArgs(&steps)

	moon := []Moon{}
	ReadInput(&moon)

	for i := 0; i < steps; i += 1 {
		Step(&moon)
	}

	fmt.Printf("Total energy in the system: %d\n", CalculateEnergy(&moon))
}