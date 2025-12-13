package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func getLines() []string {
	file, _ := os.Open("input.txt")
	scanner := bufio.NewScanner(file)
	
	lines := make([]string, 0)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	return lines
}

func add(a int, b int) int {
	return a + b
}
func mul(a int, b int) int {
	return a * b
}

type ProblemData struct {
	nums Dynamic2DArray
	operators []func(int, int) int
}

func parseProblemData(lines []string) ProblemData {
	var ret ProblemData

	rowLen := len(lines) - 1
	columnLen := len(strings.Fields(lines[0]))
	ret.nums = NewArray(rowLen, columnLen)
	
	for row, line := range lines[:rowLen] {
		numberStrings := strings.Fields(line)
		for column, numStr := range numberStrings {
			n, _ := strconv.Atoi(numStr)
			*ret.nums.get(row, column) = n
		}
	}

	//insert operators
	signs := strings.Fields(lines[rowLen])
	for _, sign := range signs {
		if sign == "+" {
			ret.operators = append(ret.operators, add)
		} else {
			ret.operators = append(ret.operators, mul)
		}
	}
	return ret
}

func calcColumnNum(problemData ProblemData, columnIndex int) int {
	operator := problemData.operators[columnIndex]
	columnNum := *problemData.nums.get(0, columnIndex)
	for row := 1; row < problemData.nums.rowCount; row++ {
		columnNum = operator(columnNum, *problemData.nums.get(row, columnIndex))
	}
	return columnNum
}

func cephalapod(problemData ProblemData) int {
	sum := 0
	for column := range problemData.nums.columnCount {
		sum += calcColumnNum(problemData, column)
	}
	return sum
}

func main() {
	lines := getLines()
	problemData := parseProblemData(lines)
	fmt.Println(cephalapod(problemData))
}