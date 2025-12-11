package main

import (
	"fmt"
	"bufio"
	"os"
	"strconv"
)

func abs(k int) int {
	if k < 0 {
		return -k
	} else {
		return k
	}
}

func add(key int, r int) (int, int) {
	rotated := key + r
	
	if (rotated == 0) {
		return rotated, 1
	} else if (rotated > 0 && rotated <= 99) {
		return rotated, 0
	} else if (rotated > 99) {
		return rotated % 100, rotated / 100
	} else {
		zero_count := 0
		for r < 0 {	
			key--
			r++
			if (key < 0) {
				key = 99
			}
			if key == 0 {
				zero_count++
			} 
		}
		return key, zero_count
	}
}

func main() {
	file, openErr := os.Open("input.txt")
	if openErr != nil {
		fmt.Println("Error: ", openErr.Error())
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	key := 50
	zero_count := 0

	for scanner.Scan() {
		line := scanner.Text()
		
		shift, parsedErr := strconv.Atoi(line[1:])
		if parsedErr != nil {
			fmt.Println("Error: could not parse ", parsedErr.Error())
			return
		}
		if line[0] == 'L' {
			shift *= -1
		}

		new_key, zero_count_addend := add(key, shift)
		
		zero_count += zero_count_addend
		key = new_key
	}
	fmt.Println("Zero count: ", zero_count)
}