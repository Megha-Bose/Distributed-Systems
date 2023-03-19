import sys

flag = 0
row = 0
pos = 0
m = None
n = None
n2 = None
p = None
key = None

for line in sys.stdin:
    if pos == 0:
        m, n = line.split()
        m = int(m)
        n = int(n)
        key = "a"
    elif m != None and pos == m + 1 and flag == 0:
        n2, p = line.split()
        n2 = int(n2)
        p = int(p)
        row = 0
        flag = 1
        key = "b"
    else:
        row_values = line.strip().split()
        col = 0
        for cell_value in row_values:
            value = ","
            value += str(row)
            value += ","
            value += str(col)
            value += ","
            value += str(cell_value)
            print(key+value)
            col += 1
        row += 1
    pos += 1