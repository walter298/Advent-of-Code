def make_matrix():
    with open('day4_input.txt', mode='r') as file:
        return [line.strip() for line in file]

def in_bounds(matrix: list, x : int, y : int) -> bool:
    return 0 <= x and x < len(matrix) and 0 <= y and y < len(matrix[0])

def search_from_x_in_direction(matrix : list, x : int, y : int, hdir : int, vdir) -> int:
    ox = x
    oy = y

    xmas_str = 'XMAS'
    xmas_str_idx = 0

    while xmas_str_idx < len(xmas_str):
        if not in_bounds(matrix, x, y):
            return 0
        #print(x, y)
        if matrix[x][y] != xmas_str[xmas_str_idx]:
            return 0
        x += hdir
        y += vdir
        xmas_str_idx += 1
    #print('(' + str(ox) + ', ' + str(oy) + ') ---> (' + str(x) + ', ' + str(y) + ')')
    return 1

LEFT = -1
RIGHT = 1
UP = -1
DOWN = 1
NO_DIR = 0

def search_from_x_in_all_directions(matrix : list, x : int, y : int) -> int:
    return search_from_x_in_direction(matrix, x, y, LEFT, NO_DIR) +  \
           search_from_x_in_direction(matrix, x, y, LEFT, UP) +      \
           search_from_x_in_direction(matrix, x, y, LEFT, DOWN) +    \
           search_from_x_in_direction(matrix, x, y, RIGHT, NO_DIR) + \
           search_from_x_in_direction(matrix, x, y, RIGHT, UP) +     \
           search_from_x_in_direction(matrix, x, y, RIGHT, DOWN) +   \
           search_from_x_in_direction(matrix, x, y, NO_DIR, UP) +    \
           search_from_x_in_direction(matrix, x, y, NO_DIR, DOWN)

def search_for_xmas_for_each_x(matrix : list) -> int:
    total = 0
    for x in range(0, len(matrix)):
        for y in range(0, len(matrix[0])):
            if matrix[x][y] == 'X':
                total += search_from_x_in_all_directions(matrix, x, y)
    return total 

def has_mas_diagonal(matrix : list, x1 : int, y1 : int, x2 : int, y2 : int) -> bool:
    if not (in_bounds(matrix, x1, y1) and in_bounds(matrix, x2, y2)):
        return False
    
    return (matrix[x1][y1] == 'M' and matrix[x2][y2] == 'S') or \
           (matrix[x1][y1] == 'S' and matrix[x2][y2] == 'M')

def has_mas(matrix, x : int, y : int) -> bool:
    return has_mas_diagonal(matrix, x + LEFT, y + UP, x + RIGHT, y + DOWN) and \
           has_mas_diagonal(matrix, x + LEFT, y + DOWN, x + RIGHT, y + UP)

def search_for_mas_from_each_a(matrix : list) -> int:
    total = 0
    for x in range(0, len(matrix)):
        for y in range(0, len(matrix[0])):
            if matrix[x][y] == 'A' and has_mas(matrix, x, y):
                total += 1
    return total

if __name__=="__main__":
    matrix = make_matrix()
    print(search_for_xmas_for_each_x(matrix))
    print(search_for_mas_from_each_a(matrix))