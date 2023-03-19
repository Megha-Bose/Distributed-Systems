#!/usr/bin/env python

import sys
import random

for line in sys.stdin:
    n_iterations = int(line.strip())
    for i in range(n_iterations):
        s = 0
        n = 0
        while s <= 1:
            n += 1
            s += random.uniform(0, 1)
        print(n, "\t", 1)
