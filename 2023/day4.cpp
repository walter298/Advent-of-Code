#include "day4.h"

constexpr size_t WINNING_NUMS = 11;

using RowWindow = std::array<int, WINNING_NUMS>;

void score(std::string& line, RowWindow& win, size_t& idx) {
    auto& row = line;

    auto start = std::next(std::ranges::find(line, ':'));
    auto breakp = std::ranges::find(line, '|');

    std::array<std::string, WINNING_NUMS> winningNums;
    size_t winningIdx = 0;

    advent::getWords<true>(start, breakp,
        [&](auto& word) { 
            winningNums[winningIdx] = std::move(word);
            winningIdx++;
        }
    );

    int sharedNumsC = 0;
    advent::getWords<true>(breakp, line.end(),
        [&](auto& word) {
            if (std::ranges::contains(winningNums, word)) {
                sharedNumsC++;
            }
        }
    );
    for (size_t i = 0; i < sharedNumsC; i++) {
        const auto currIdx = (idx + 1 + i) % WINNING_NUMS;
        win[currIdx] += win[idx];
    }
}

void runDay4() {
    int scratchBoardCount = 0;
    RowWindow currentRows;
    std::ranges::fill(currentRows, 1);
    size_t rowIdx = 0;

    advent::doWhileReading(
        "2023/inputs/day4.txt"s,
        [&](auto& line) {
            score(line, currentRows, rowIdx);
            line.clear();
            scratchBoardCount += currentRows[rowIdx];
            currentRows[rowIdx] = 1;
            rowIdx = (rowIdx + 1) % WINNING_NUMS;
        }
    );
    
    std::cout << "Total Scratchboard Count: " << scratchBoardCount << std::endl;
}