# Map-Reduce framework using Hadoop streaming


## Multiplying matrices A and B

Input

2 2
1 3
2 4
2 2
4 2
3 1

Converted to (multiplying matrices a and b) Matrix, row, col, value:

a,0,0,1
a,0,1,3
a,1,0,2
a,1,1,4
b,0,0,4
b,0,1,2
b,1,0,3
b,1,1,1

In this example, the mapper generates key-value pairs as follows:

a    a,0,0,1
a    a,0,1,3
a    a,1,0,2
a    a,1,1,4
b    b,0,0,4
b    b,0,1,2
b    b,1,0,3
b    b,1,1,1

Reducer fetches values by key and uses value to calculated necessary products for final form of a * b.


## Estimating value of e

| No. of simulations | Exp. Value (averaged over 10 runs each) |
| --- | --- |
| 100 | 2.73 |
| 1000 | 2.735 |
| 10000 | 2.7142 |
| 100000 | 2.71908 |

Actual value of e is 2.7183. The deviations of values calculated for 100, 1000, 10000 and 100000 simulations from actual value of e are 0.43%, 0.61%, 0.15% and 0.03% respectively. As we increase the number of simulations, we get more accurate value estimation of e, as expected.

The number of times n occurs for which X1+X2+...Xn value first exceeds 1 is calclated by mapping to 1 count for each occurence and then using reduce to count total number of occurences of each value of n. Then, the expected value of n is calculated to give the final estimate.




---

Sample commands:

Run `make build` in docker-hadoop-python. Start docker containers in docker-hadoop-python. Put 2019111021 directory in namenode:

```
sudo docker cp  2019111021 namenode:/tmp/
sudo docker exec -it namenode /bin/bash
```

In interactive shell mode in namenode:

```
cd 2019111021/2019111021_2
chmod 777 runner_script.sh
./runner_script.sh /opt/hadoop-3.2.4/share/hadoop/tools/lib/hadoop-streaming-3.2.4.jar data1.txt /input /output .
```
