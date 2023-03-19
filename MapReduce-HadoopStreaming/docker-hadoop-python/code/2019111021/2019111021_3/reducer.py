#!/usr/bin/env python

import sys

sum_n = 0
total = 0
for line in sys.stdin:
    n, c = line.strip().split("\t")
    sum_n += int(n) * int(c)
    total += int(c)
sum_n = float(sum_n)
e = sum_n / total
print(e)
