
#!/usr/bin/env python
import sys

a={}

b={}

m = 0
n = 0
p = 0

for line in sys.stdin:
    key,value = line.strip().split("\t",1)
    v = value.split(",")
    row = int(v[1])
    col = int(v[2])
    val = int(v[3])
    if key=="a":
        a[(row,col)] = val
        m = max(row + 1, m)
        n = max(col + 1, n)
    elif key=="b":
        b[(row,col)] = val
        p = max(col + 1, p)
    
result=0
for i in range(m):  
    for j in range(p):  
        for k in range(n):  
            result += a[(i,k)]*b[(k,j)]  
        print(result, end=" ")
        result = 0
    print()  