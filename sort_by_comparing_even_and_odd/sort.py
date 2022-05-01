class UserVar:
    def __init__(self, name, index, value=None):
        self.name = name
        self.index = index
        self.value = value

    def __gt__(self, other):
        print("?", self.name, other.name)
        return input() == ">"

    def __lt__(self, other):
        print("?", self.name, other.name)
        return input() == "<"

    def __repr__(self):
        return self.name + ":" + str(self.index) + "=" + repr(self.value)


def quicksort(groups, pivot):
    def find_group(l=0, r=(len(groups) - 1)):
        if r - l <= 1:
            return (l, r)
        c = (l + r) // 2
        return find_group(c, r) if groups[c][1][0] < pivot else find_group(l, c)
    l, r = find_group()
    pivot_pos, result = groups[l][0], []
    for g in range(l, r + 1):
        new_groups = [[], []]
        for v in groups[g][1]:
            up = v > pivot
            new_groups[up].append(v)
            pivot_pos += not up
        offset = groups[g][0]
        for new_group in new_groups:
            if new_group:
                result.append((offset, new_group))
                offset += len(new_group)
    return groups[:l] + result + groups[r + 1 :], pivot_pos

n = int(input("n = "))
even = [UserVar("even " + str(k + 1), k * 2) for k in range(n // 2)]
odd = [UserVar("odd " + str(k + 1), k * 2 + 1) for k in range((n + 1) // 2)]
groups = [(0, even)]
for pivot in odd:
    groups, i = quicksort(groups, pivot)
    pivot.value = i
for offset, group in groups:
    n = group[0].value = offset
print("even: ", " ".join(str(n.value * 2 + 2) for n in even))
print("odd: ", " ".join(str(n.value * 2 + 1) for n in odd))
