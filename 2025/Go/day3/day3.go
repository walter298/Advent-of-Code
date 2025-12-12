package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
)

type DigitSearchData struct {
	digit int
	index int
}

func findLargestDigit(bank string, begin int, digitsLeft int) DigitSearchData {
	end := len(bank) - (digitsLeft - 1)
	searchableBatteries := bank[begin:end]
	
	var ret DigitSearchData

	for i, digitChr := range searchableBatteries {
		digit := int(digitChr - '0')
		if digit > ret.digit {
			ret.digit = digit
			ret.index = i
		}
	}
	ret.index += begin
	return ret
}

func getMaxBankJoltage(bank string, batteryCount int) int {
	digitsLeft := batteryCount

	if (digitsLeft > len(bank)) {
		panic("Insufficient batteries")
	}

	ret := 0
	
	begin := 0

	for digitsLeft > 0 {
		digitData := findLargestDigit(bank, begin, digitsLeft)
		digitsLeft--
		ret += int(math.Pow10(digitsLeft)) * digitData.digit
		begin = digitData.index + 1
	}

	return ret
}

func main() {
	sum := 0

	file, ok := os.Open("input.txt")
	if ok != nil {
		fmt.Println(ok.Error())
		return
	}

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		sum += getMaxBankJoltage(line, 12)
	}
	fmt.Println(sum)
}