def bitmask2intervals(bitmask):
    lens = []
    cur_len = 0
    prev_bit = 0
    for bit in bitmask:
        if bit == prev_bit:
            cur_len += 1
        else:
            lens.append(cur_len)
            cur_len = 1
        prev_bit = bit
    if prev_bit:
        lens.append(cur_len)
        cur_len = 0
    lens.append(cur_len)

    lens[0] += 1
    lens[-1] += 1

    def try_fill(mobility, size):
        if size < 1:
            return None
        if size == 1:
            return []
        if size == 2:
            return None
        if mobility == 0:
            return None
        if size % 2:
            return [1] * ((size - 1) // 2)
        if mobility == 1:
            return None
        return [1] * ((size - 4) // 2) + [2]

    for mobility in range(4):
        result = []
        for i, cur_len in enumerate(lens):
            if i % 2:
                result.append(cur_len + mobility)
            else:
                pltfs = try_fill(mobility, cur_len - mobility)
                if pltfs is None:
                    break
                for pltf in pltfs:
                    result.append(pltf)
        else:
            return result
    return None

intervals = bitmask2intervals([c == '1' for c in input('Input bitmask: ')])
print('Intervals =', intervals)
