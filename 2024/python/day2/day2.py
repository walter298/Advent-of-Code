def is_valid_dist(a : int, b : int) -> bool:
    dist = abs(a - b)
    return dist > 0 and dist < 4

def is_valid_order(is_asc : bool, a : int, b : int) -> bool:
    if a > b and is_asc:
        return False
    elif a < b and not is_asc:
        return False
    else:
        return True

def is_sort_order_asc(levels : list):
    first = levels[0]
    for i in range(1, len(levels)):
        level = levels[i]
        if level == first:
            continue
        else:
            return level > first
    return False

def is_interval_valid(is_asc : bool, a : int, b : int) -> bool:
    return is_valid_dist(a, b) and is_valid_order(is_asc, a, b)

def is_safe(levels : list) -> bool:
    is_asc = is_sort_order_asc(levels)
    
    second_elements = levels[1:]
    for a, b in zip(levels, second_elements):
        if not is_interval_valid(is_asc, a, b):
            return False

    return True

def is_safe_without_level(levels : list, excluded_level_idx : int) -> bool:
    sliced_levels = levels[:excluded_level_idx] + levels[excluded_level_idx + 1:]
    return is_safe(sliced_levels)

def is_safe_with_dampener(levels : list) -> bool:
    is_asc = is_sort_order_asc(levels)
    
    level_idx = 0

    second_elements = levels[1:]

    for a, b in zip(levels, second_elements):
        if not is_interval_valid(is_asc, a, b):
            return is_safe_without_level(levels, 0) or is_safe_without_level(levels, level_idx) or is_safe_without_level(levels, level_idx + 1)
        level_idx += 1
            
    return True
    
def get_safe_level_count(level_rows : list) -> int:
    total = 0
    for levels in level_rows:
        if is_safe(levels):
            total += 1
    return total

def get_safe_level_count_with_dampener(level_rows : list) -> int:
    total = 0

    for levels in level_rows:
        if is_safe_with_dampener(levels):
            total += 1
    return total

def make_level_rows():
    level_rows = list()

    with open('day2_input.txt', mode='r') as file:
        for line in file:
            level_rows.append(list(map(int, line.strip().split())))
    
    return level_rows

def __main__():
    level_rows = make_level_rows()
    print("Safe Level Count: " + str(get_safe_level_count(level_rows)))
    print("Safe Level Count With Dampener: " + str(get_safe_level_count_with_dampener(level_rows)))

if __name__ == "__main__":
    __main__()