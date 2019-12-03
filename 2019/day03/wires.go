package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Point struct {
	x, y int
}

func (p Point) ToString() string {
	return fmt.Sprintf("Point{ x: %d, y: %d }(dist: %d)", p.x, p.y, p.Distance())
}

func (p Point) Print() {
	fmt.Printf("%s\n", p.ToString())
}

func (p Point) Distance() int {
	var x, y int

	if p.x > 0 {
		x = +p.x
	} else {
		x = -p.x
	}

	if p.y > 0 {
		y = +p.y
	} else {
		y = -p.y
	}

	return x + y
}

type Segment struct {
	start, end Point
}

func (s Segment) ToString() string {
	return fmt.Sprintf("{ %d, %d -> %d, %d }", s.start.x, s.start.y, s.end.x, s.end.y)
}

func ParseLine(line string, out *[]Segment) {
	oldPos := Point{ x: 0, y: 0 }

	fragments := strings.Split(line, ",")
	for i := 0; i < len(fragments); i += 1 {
		f := fragments[i]
		velocity, _ := strconv.Atoi(f[1:])

		xMovement, yMovement := 0, 0
		switch(f[0]) {
			case 'U': yMovement = -1
			case 'R': xMovement = +1
			case 'D': yMovement = +1
			case 'L': xMovement = -1
		}

		newPos := Point{
			x: oldPos.x + (xMovement * velocity),
			y: oldPos.y + (yMovement * velocity),
		}

		*out = append(*out, Segment{
			start: oldPos,
			end: newPos,
		})

		oldPos = newPos
	}
}

func Min(a, b int) int {
	if a < b {
		return a
	} else {
		return b
	}
}

func Max(a, b int) int {
	if a > b {
		return a
	} else {
		return b
	}
}

func FindIntersection(a, b Segment) *Point {
	if(a.start.x == a.end.x) {
		// "a" is a vertical segment
		if(b.start.x == b.end.x) {
			// "b" is a vertical segment
			// Two vertical segments could overlap, but let's ignore that for now
			return nil
		} else {
			// "b" is a horizontal segment
			minX := Min(b.start.x, b.end.x)
			maxX := Max(b.start.x, b.end.x)
			if(a.start.x < minX) || (a.start.x > maxX) {
				return nil
			}
			minY := Min(a.start.y, a.end.y)
			maxY := Max(a.start.y, a.end.y)
			if(b.start.y < minY) || (b.start.y > maxY) {
				return nil
			}
			return &Point{ x: a.start.x, y: b.start.y }
		}
	} else {
		// "a" is a horizontal segment
		if(b.start.x == b.end.x) {
			// "b" is a vertical segment
			minX := Min(a.start.x, a.end.x)
			maxX := Max(a.start.x, a.end.x)
			if(b.start.x < minX) || (b.start.x > maxX) {
				return nil
			}
			minY := Min(b.start.y, b.end.y)
			maxY := Max(b.start.y, b.end.y)
			if(a.start.y < minY) || (a.start.y > maxY) {
				return nil
			}
			return &Point{ x: b.start.x, y: a.start.y }
		} else {
			// "b" is a horizontal segment
			// Two horizontal segments could overlap, but let's ignore that for now
			return nil
		}
	}
}

func main() {
	reader := bufio.NewReader(os.Stdin)
	var line string

	wire1 := []Segment{}
	line, _ = reader.ReadString('\n')
	ParseLine(strings.Trim(line, "\n \t"), &wire1)

	wire2 := []Segment{}
	line, _ = reader.ReadString('\n')
	ParseLine(strings.Trim(line, "\n \t"), &wire2)

	intersections := []Point{}
	for i := 0; i < len(wire1); i += 1 {
		for j := 0; j < len(wire2); j += 1 {
			cross := FindIntersection(wire1[i], wire2[j])
			if cross == nil { continue }

			// Special case, shut up
			if (cross.x == 0) && (cross.y == 0) { continue }

			// fmt.Printf("Segments %s and %s cross at %s\n", wire1[i].ToString(), wire2[j].ToString(), cross.ToString())
			intersections = append(intersections, *cross)
		}
	}

	minIdx := 0
	minDist := intersections[minIdx].Distance()
	intersections[minIdx].Print()
	for idx := 1; idx < len(intersections); idx += 1 {
		intersections[idx].Print()
		
		dist := intersections[idx].Distance()
		if dist < minDist {
			minIdx = idx
			minDist = dist
		}
	}

	fmt.Printf("Closest intersection is #%d\n", minIdx)
	intersections[minIdx].Print()
}