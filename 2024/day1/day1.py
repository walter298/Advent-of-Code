from collections import defaultdict

class ListOutput():
    def __make_lists(self, file):
        for line in file:
            first, second = map(int, line.strip().split())
            self.left_list.append(first)
            self.right_list.append(second)

    def __init__(self):
        self.left_list = list()
        self.right_list = list()

        with open('day1_input.txt', mode='r') as file:
            self.__make_lists(file)

def get_list_elem_diffs(lists : ListOutput) -> int:
    lists.left_list.sort()
    lists.right_list.sort()

    total = 0
    for first, second in zip(lists.left_list, lists.right_list):
        total += abs((second - first))
    
    return total

def make_occ_map(right_list : list):
    occ_map = defaultdict(int)
    for num in right_list:
        occ_map[num] += 1
    return occ_map

def get_similarity_score(lists : ListOutput) -> int:
    total = 0

    occ_map = make_occ_map(lists.right_list)

    for num in lists.left_list:
        total += (occ_map[num] * num)

    return total

def __main__():
    lists = ListOutput()
    
    print("List Diffs: " + str(get_list_elem_diffs(lists)))
    print("Similarity Score: " + str(get_similarity_score(lists)))

if __name__ == "__main__":
    __main__()