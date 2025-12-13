package main

type Dynamic2DArray struct {
	buff []int
	rowCount int
	columnCount int
}

func NewArray(rowCount int, columnCount int) Dynamic2DArray {
	return Dynamic2DArray{ 
		buff: make([]int, rowCount * columnCount),
		rowCount: rowCount,
		columnCount: columnCount,
	}
}

func (a *Dynamic2DArray) get(row int, column int) *int {
	idx := (row * a.columnCount) + column
	return &a.buff[idx]
}