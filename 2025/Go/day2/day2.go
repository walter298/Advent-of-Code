package main

import (
	"bufio"
	"fmt"
	"math"
	"math/rand/v2"
	"os"
	"slices"
	"strconv"
	"strings"
)

type DigitRecurrenceTable = [10]int64

type NumberData struct {
	digitRecurenceTable DigitRecurrenceTable
	digits []int
}

func calcDigitData(x int64) NumberData {
	var numData NumberData
	for x > 0 {
		currDigit := int(x % 10)
		numData.digitRecurenceTable[currDigit]++
		numData.digits = append(numData.digits, currDigit)
		x /= 10
	}
	slices.Reverse(numData.digits)
	return numData
}

func calcGCD(a int64, b int64) int64 {
	r := a % b
	if r == 0 {
		return b
	} else {
		return calcGCD(b, r)
	}
}

type DigitArrayData struct {
	onlyOneDigit     bool
	totalDigitCount  int64
	gcdOfDigitCounts int64
}

func calcDigitRecurenceData(array DigitRecurrenceTable) DigitArrayData { //at least one element != 0
	ret := DigitArrayData{}

	containedDigits := [10]bool{}
	uniqueDigitCount := 0

	for digit, digitCount := range array {
		if digitCount == 0 {
			continue
		}

		if !containedDigits[digit] {
			uniqueDigitCount++
			containedDigits[digit] = true
		}
		if uniqueDigitCount == 1 {
			ret.gcdOfDigitCounts = digitCount
		} else if uniqueDigitCount > 1 {
			ret.gcdOfDigitCounts = calcGCD(ret.gcdOfDigitCounts, digitCount)
		}
		ret.totalDigitCount += digitCount
	}
	if uniqueDigitCount == 0 {
		panic("Error: invalid digit table")
	}

	ret.onlyOneDigit = uniqueDigitCount == 1
	return ret
}

func containsRepeatingSequenceN(seqLen int64, numberData NumberData, tableData DigitArrayData) bool {
	indexTable := make([]int, seqLen) 
	
	for digitIndex := int64(0); digitIndex < tableData.totalDigitCount; digitIndex++ {
		currDigit := numberData.digits[digitIndex]
		relativeSeqIndex := digitIndex % seqLen
		
		if digitIndex < seqLen { //we have not yet initialized expected digit
			indexTable[relativeSeqIndex] = currDigit
		} else { //we are expecting a repeated value to be here
			expectedDigit := indexTable[relativeSeqIndex]
			if expectedDigit != currDigit {
				return false
			}
		}
	}
	return true
}

func containsRepeatingSequence(x int64, numberData NumberData, tableData DigitArrayData) bool {
	if tableData.onlyOneDigit && tableData.totalDigitCount > 1 {
		return true
	} 
	if tableData.gcdOfDigitCounts == 1 { //digits have to fit evenly into at least two groups
		return false
	}

	for seqCount := int64(2); seqCount <= tableData.gcdOfDigitCounts; seqCount++ {
		if tableData.gcdOfDigitCounts % seqCount != 0 {
			continue
		}
		seqLen := tableData.totalDigitCount / seqCount
		if containsRepeatingSequenceN(seqLen, numberData, tableData) {
			return true
		}
	}

	return false
}

func invalid(x int64) bool {
	if x <= 0 {
		panic(fmt.Sprintf("Unexpected input: %v", x))
	}
	digitTable := calcDigitData(x)
	tableData := calcDigitRecurenceData(digitTable.digitRecurenceTable)
	
	return containsRepeatingSequence(x, digitTable, tableData)
}

type Interval struct {
	first int64 
	last int64
}

func parseIntervals() []Interval {
	file, ok := os.Open("input.txt")
	if ok != nil {
		panic(ok.Error())
	}
	scanner := bufio.NewScanner(file)
	if !scanner.Scan() {
		panic("Error: no lines to scan in the file!")
	}

	input := scanner.Text()
	intervalStrings := strings.Split(input, ",")

	ret := make([]Interval, 0, len(intervalStrings))
	for _, intervalStr := range intervalStrings {
		firstLastStrings := strings.Split(intervalStr, "-")
		first, _ := strconv.ParseInt(firstLastStrings[0], 10, 64)
		last, _ := strconv.ParseInt(firstLastStrings[1], 10, 64)
		interval := Interval{
			first: first,
			last: last,
		}
		ret = append(ret, interval)
	}
	return ret
}

func makeTestNum() int64 {
	digitCount := rand.IntN(4) + 1 //add 1 to prevent 0, get [1, 5]
	seqCount := rand.IntN(2) + 2 //sequence repeats 2-4 times
	
	var sequence int64
	for d := range digitCount {
		digit := rand.Int64N(9)
		if digit == 0 {
			digit++
		}
		sequence += digit * int64(math.Pow10(d))
	}

	ret := int64(0)
	for i := range seqCount {
		ret += sequence * int64(math.Pow10(digitCount * i))
	}
	
	return ret
}

func tryToFindBug() {
	for {
		x := makeTestNum()
		if !invalid(x) {
			fmt.Printf("Aha! %v slipped through the cracks\n", x)
			os.Exit(1)
		}
	}
}

func main() {
	intervals := parseIntervals()

	invalidIDSum := int64(0)
	for _, interval := range intervals {
		for x := interval.first; x <= interval.last; x++ {
			if invalid(x) {
				invalidIDSum += x
			} else {
			}
		}
	}
	fmt.Println("Invalid ID sum: ", invalidIDSum)
}
