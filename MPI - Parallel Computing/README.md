# Assignment 1: MPI Programming

Let, number of processes be p.

## Q1: Mandelbrot Set
Program using MPI that computes the Mandelbrot set after K iterations for N ∗ M points spaced uniformly in the region of the complex plane bounded by -1.5 - i, -1.5 + i, 1 + i, 1 - i in a distributed manner.

- The total time complexity of the approach is O(((n * m) / p) * k) for each process because n * m points are distributed among p processes and computations are performed independently on each of the processes for k iterations each. Process with rank 0 gathers n * m values in the end though.

- The total message complexity of the approach is O(n * m) as all the processes send values to the process with rank 0 for a total of n * m points.

- The space requirements of the solution O(n * m / p) on each process because n * m points are distributed among p processes and hence we need to store values only for the points assigned to a particular process.

- The performance scaling as you went from 1 to 12 processes (use a large enough test case for this)

	Took case of n = 100, m = 100 and k = 100 but all came 0s. So, went beyond constraints and took n = 1000, m = 1000 and k = 1000.

	We can see that as the number of processes increases, the time taken decreases.

	| No. of processes | Time taken (s)  | 
	| :---:   | :---: | 
	| 1 | 2 |
	| 2 | 2 |
	| 3 | 1 |
	| 4 | 1 |
	| 5 | 1 |
	| 6 | 1 |
	| 7 | 1 |
	| 8 | 1 |
	| 9 | 1 |
	| 10 | 1 |
	| 11 | 1 |
	| 12 | 0 |



## Q2: Particle Simulation
Given a 2D Grid of size N ∗ M and the initial positions of the Pym Particles and their directions of movement find the final positions of the Pym particles after T time steps.

- The total time complexity of the approach is O(k * t / p) for each process because the k particles are distributed among the processes, there are p processes and we rum the program for t timesteps. Hence, each process needs to make order of k/p operations for t times each. 

- The total message complexity of the approach is O(2 * N * t * p) or O(2 * M * t * p) based on which kind of split is made because for each timestep, each process sends max of N (or M) particles to processes before and after it. It can also be O(k) if it is of smaller order than the previous ones because a maximum of total number of particles can be passed amongst the processes.

- The space requirements of the solution is O(4 * N * M / p) for each process as we store the current particles under a process and the N * M grid is distributed amongst p processes equally. At a time, there can not be more than 4 particles at one lattice point (U, D, L, R). Using similar logic as earlier, it can also be O(k) because there won't be more than k particles on a process at a given time.

- The performance scaling as you went from 1 to 12 processes (use a large enough test case for this)
	The testcase used here is mentioned below:
	| No. of processes | Time taken (s)  | 
	| :---:   | :---: | 
	| 1 | 2 |
	| 2 | 2 |
	| 3 | 2 |
	| 4 | 1 |
	| 5 | 1 |
	| 6 | 1 |
	| 7 | 1 |
	| 8 | 1 |
	| 9 | 1 |
	| 10 | 1 |
	| 11 | 0 |
	| 12 | 0 |



## Q3: Optimal BST
Given a set of N pairs of key and its search frequency count, Jar has to do the following things, implement merge sort to order the set based on the key. Use the sorted set to create an optimal binary search tree based on the frequency count such that the total average cost of all searches is minimized. All the keys are unique.

- The total time complexity of the approach is O(n^3 / p) because the independent computations needed for each diagonal are distributed among p processes and there are in total n^3 order computations.

- The total message complexity of the approach is O(n^2 * p * n) because DP of size n^2 is sent by each process to the ther processes.

- The space requirements of the solution is O(n^2 * p) because we are keeping nxn DP for each of the p processes.

- The performance scaling as you went from 1 to 12 processes:
	Testcase used:
	8
	5 7
	3 6
	7 5
	6 17
	1 16
	4 8
	2 13
	8 28


	| No. of processes | Time taken (s)  | 
	| :---:   | :---: | 
	| 1 | 0 |
	| 2 | 0 |
	| 3 | 0 |
	| 4 | 0 |
	| 5 | 0 |
	| 6 | 0 |
	| 7 | 0 |
	| 8 | 0 |
	| 9 | 0 |
	| 10 | 0 |
	| 11 | 0 |
	| 12 | 0 |




---

Large Testcase Used for Q2 Performance Scaling:

300 500 1000 1000
108 215 D
46 234 R
231 37 D
92 141 R
297 350 R
217 193 L
139 497 R
9 112 D
241 447 L
46 224 R
140 169 L
255 96 R
128 89 R
45 106 D
272 354 U
292 363 U
272 277 D
19 326 U
259 376 L
155 413 D
248 94 U
30 177 D
269 442 D
220 427 R
295 94 L
294 303 U
228 219 R
35 427 L
67 55 R
108 411 R
67 247 U
63 194 U
151 487 L
252 4 L
181 461 R
234 54 L
40 469 U
37 212 U
233 308 D
272 209 R
209 58 R
174 22 R
142 456 D
242 34 U
220 274 L
60 455 R
62 441 U
180 267 D
164 420 L
253 197 R
25 430 U
24 135 D
281 484 D
170 426 U
239 225 D
165 56 U
253 479 U
163 378 U
198 100 R
156 196 U
145 420 D
241 102 R
219 350 L
228 78 D
242 284 L
143 352 R
175 256 U
89 464 R
220 111 L
203 83 L
23 427 D
186 366 U
269 178 L
126 399 R
7 66 U
133 288 R
275 107 D
195 379 U
82 376 R
101 99 R
215 1 R
160 95 U
249 353 R
175 424 D
118 493 L
8 445 U
19 97 L
116 87 U
163 135 U
58 1 U
275 139 U
44 52 L
223 180 D
144 454 L
26 213 U
138 398 D
18 125 R
158 146 U
51 428 L
233 423 U
3 213 U
287 145 R
100 85 R
285 210 U
93 341 R
128 45 L
233 444 L
86 152 U
190 44 R
30 496 U
4 32 U
48 66 U
24 195 U
179 193 U
101 66 U
206 145 U
213 457 U
149 440 D
131 182 U
33 393 D
219 417 U
69 390 D
224 365 U
30 441 U
218 414 D
227 157 R
203 407 R
175 212 D
2 134 U
42 104 L
229 46 U
277 431 L
175 68 L
31 295 D
129 487 L
275 325 U
296 351 R
138 14 U
193 379 R
55 495 D
51 277 U
206 336 L
268 158 R
270 276 U
83 46 U
125 290 U
294 316 U
205 339 D
27 381 U
178 45 U
105 141 L
36 170 R
38 441 U
38 243 D
84 433 R
96 12 L
290 51 R
4 379 D
298 211 D
298 18 R
26 65 L
18 156 D
264 383 U
32 156 U
268 347 L
205 286 D
117 297 D
3 313 D
18 186 D
48 295 D
155 58 D
83 169 L
57 48 L
45 190 L
9 371 U
51 139 L
287 201 L
124 295 L
66 341 R
147 60 D
248 74 U
185 445 R
281 345 U
296 135 D
266 57 R
284 45 D
148 449 L
147 323 U
293 124 L
1 320 D
162 14 D
197 264 R
142 231 D
260 136 R
286 295 D
207 316 D
229 68 L
159 264 U
19 60 D
116 220 L
217 380 D
180 18 R
87 299 D
14 274 L
5 471 U
78 264 R
284 288 L
34 86 R
48 32 D
103 307 L
47 54 D
139 101 L
274 58 U
127 27 U
13 304 L
197 365 R
68 176 U
83 265 R
88 296 D
79 472 R
66 455 U
209 337 D
110 382 U
226 162 L
212 310 R
75 44 U
20 177 D
287 279 L
138 453 D
189 179 D
267 256 R
1 405 D
168 144 L
268 189 R
17 237 L
24 339 R
60 100 D
120 386 U
236 206 L
177 8 U
46 180 L
187 277 U
152 238 D
224 122 D
231 452 D
205 37 R
44 159 R
124 494 U
238 231 R
176 163 D
26 265 U
132 331 L
126 361 U
117 494 D
23 57 U
74 193 U
48 369 L
261 342 R
105 391 D
27 230 L
224 117 L
68 229 U
157 52 L
81 69 L
118 387 L
284 246 U
294 483 U
229 121 R
206 462 D
164 296 U
288 467 D
38 158 D
196 64 U
31 422 U
63 277 L
235 160 U
155 108 U
166 118 R
101 265 U
209 122 R
67 11 D
10 36 L
169 30 L
74 192 R
148 180 U
127 308 R
93 350 L
93 97 R
80 388 L
178 38 R
105 192 L
104 52 R
223 262 U
41 377 R
244 305 L
56 431 R
58 423 R
254 265 D
288 497 L
56 422 R
39 417 D
217 368 U
31 274 D
271 198 L
95 64 L
159 485 U
93 208 U
70 336 L
245 247 L
134 366 U
241 76 L
38 47 D
130 233 D
123 284 L
6 496 R
31 354 R
112 136 L
159 262 L
195 227 D
296 115 U
138 419 R
196 94 R
284 461 U
66 30 R
241 159 U
276 115 D
115 359 L
33 279 D
205 28 R
213 61 D
173 90 D
170 245 R
297 54 D
225 446 L
172 54 D
156 279 D
131 473 D
99 343 L
193 225 R
158 139 D
160 259 R
257 333 R
242 87 D
178 150 U
100 93 R
35 173 D
23 403 D
164 342 D
32 90 R
139 33 L
165 46 U
256 209 L
21 454 R
42 327 L
96 275 L
125 455 U
116 485 D
183 145 L
80 130 D
12 469 D
200 145 D
230 164 D
253 352 U
296 186 D
266 447 D
66 354 D
141 441 U
24 266 L
132 89 D
41 136 D
226 465 R
251 339 L
94 89 L
275 140 R
106 143 D
263 110 U
214 294 D
270 439 U
8 388 L
66 150 U
273 336 L
219 332 L
147 348 L
79 258 D
66 389 D
204 437 D
30 358 U
267 220 D
203 423 U
226 182 U
58 303 D
216 431 U
17 71 R
29 234 L
251 365 L
52 494 U
105 476 L
272 12 R
267 27 D
231 30 R
205 77 D
126 214 R
255 202 R
78 209 U
189 133 D
95 276 L
60 91 D
18 368 R
229 390 R
124 126 U
47 65 L
2 42 R
197 310 R
203 220 R
87 28 D
234 460 L
251 379 U
176 142 R
277 48 U
170 360 U
70 262 L
54 138 R
40 256 D
53 270 L
112 253 D
179 150 U
17 296 D
30 170 U
199 360 L
196 156 R
270 163 L
20 379 D
177 20 R
122 44 L
230 321 U
103 405 L
134 411 D
83 92 U
275 73 L
65 324 D
152 393 D
295 264 D
216 396 U
93 104 D
138 183 R
158 422 R
80 363 U
174 305 D
18 309 R
198 424 L
287 349 L
67 9 D
196 217 R
129 471 U
214 384 L
276 288 R
183 171 R
271 330 R
261 18 U
72 429 R
199 226 R
25 68 U
167 333 L
194 383 U
258 421 L
290 108 R
96 266 D
105 311 R
113 413 U
105 404 L
136 190 R
79 83 U
219 345 R
185 296 R
80 119 R
40 334 L
233 367 U
142 82 U
259 120 U
293 372 R
144 105 D
107 245 R
250 144 R
161 176 U
276 75 L
224 304 R
278 74 U
26 54 R
46 74 U
12 73 R
6 398 R
235 358 R
128 359 U
149 155 U
203 289 L
259 442 L
10 303 D
138 59 L
161 436 U
201 434 L
54 215 R
173 49 L
31 436 D
153 226 D
149 132 R
88 272 L
18 401 D
191 469 R
154 315 U
164 140 D
226 233 D
185 42 D
147 468 U
84 498 D
146 229 U
11 312 R
222 393 L
151 127 U
122 404 U
200 461 R
100 130 U
113 466 D
197 173 L
179 106 R
199 341 D
169 90 L
81 371 U
231 331 D
171 108 L
61 137 D
103 194 L
71 493 L
128 85 L
6 401 R
185 93 U
199 208 R
153 438 R
66 268 D
137 6 U
231 384 U
204 128 U
291 386 U
79 123 L
23 224 L
216 111 R
208 91 U
19 475 D
158 2 U
189 292 D
243 255 U
232 270 L
254 468 D
134 238 U
68 330 R
244 272 R
255 403 D
289 76 U
138 317 R
22 302 D
111 216 R
188 117 L
66 198 L
103 213 R
129 326 D
179 440 L
128 486 L
196 175 D
226 440 L
5 474 D
266 200 L
57 3 R
168 10 L
14 29 D
259 101 U
32 371 R
174 33 D
297 12 D
130 205 U
120 195 R
167 195 R
226 377 R
249 434 R
181 335 R
176 238 R
13 471 U
212 311 U
165 79 D
160 348 R
47 360 D
134 334 D
255 149 L
1 179 D
42 204 L
173 355 R
244 461 D
136 463 R
284 187 R
277 387 R
62 425 U
230 276 R
151 141 U
191 359 U
229 24 R
267 498 L
276 387 U
297 143 L
217 134 D
42 293 D
191 404 R
197 5 L
227 446 L
227 368 L
127 286 D
129 374 L
129 451 D
119 250 D
44 80 L
57 19 L
1 63 D
165 255 D
146 80 D
30 182 L
131 308 L
211 352 U
277 205 L
54 280 R
47 329 D
92 251 U
29 417 L
172 90 R
254 127 L
263 34 R
111 227 L
87 215 U
191 155 L
126 185 L
113 373 U
157 420 U
239 224 D
72 260 L
111 257 U
45 247 U
284 424 U
245 346 L
257 293 U
91 466 U
257 396 D
199 28 L
84 283 L
265 156 D
23 400 D
99 356 R
225 285 L
243 415 U
75 21 U
296 304 D
64 386 L
33 259 L
294 430 U
298 320 R
207 18 L
284 68 R
215 425 D
112 317 U
50 378 L
230 183 R
191 414 D
35 449 R
237 48 L
121 431 R
86 443 U
242 180 D
37 386 R
63 489 L
275 176 R
15 397 R
289 381 L
170 186 L
145 16 D
9 395 D
46 240 R
268 450 U
187 462 U
29 402 U
55 266 L
32 493 U
285 389 D
271 340 L
54 26 D
223 294 R
100 448 D
35 187 D
144 15 D
38 251 R
205 245 U
89 476 U
271 171 R
26 389 U
233 124 L
150 254 L
112 45 L
63 285 L
201 408 L
68 355 L
298 70 D
270 196 U
219 293 U
160 150 L
154 451 D
82 128 U
173 272 L
113 60 L
15 297 D
45 409 L
54 176 R
162 338 D
252 172 L
79 314 R
139 82 R
61 102 D
94 87 D
199 275 R
197 373 R
282 328 D
68 271 D
123 410 U
109 343 U
253 266 D
273 220 U
66 142 U
41 415 L
128 170 L
43 338 D
26 345 L
177 254 U
120 308 L
211 154 U
295 477 R
168 37 R
41 322 L
9 490 R
37 247 U
21 309 D
182 148 R
286 430 U
99 336 R
79 282 R
179 280 D
201 145 D
272 457 L
174 241 D
1 161 R
239 332 R
210 129 U
205 379 R
281 275 L
273 307 D
298 239 U
23 257 D
18 2 R
60 114 U
189 10 U
234 199 R
257 20 L
273 317 L
180 185 U
109 93 L
258 452 U
39 244 R
224 3 U
267 50 R
167 296 R
288 409 R
205 309 U
231 113 L
250 245 L
252 265 R
254 364 D
297 221 U
60 497 R
189 389 D
209 210 U
163 149 R
86 27 R
280 222 L
68 23 U
262 334 D
128 181 L
31 112 L
168 125 L
283 27 L
266 225 D
249 254 R
264 63 L
191 182 R
13 62 D
230 335 L
159 96 D
191 183 L
211 334 U
187 384 U
8 103 D
165 177 L
119 64 L
83 19 D
278 411 L
89 152 R
45 262 L
122 268 L
30 230 R
116 227 U
281 68 R
146 380 R
182 396 D
166 400 R
99 195 R
40 221 R
269 234 L
9 493 U
196 373 D
101 156 D
18 208 L
211 255 L
44 470 R
79 497 D
208 278 D
166 33 L
171 355 D
93 5 R
123 420 R
203 101 D
210 186 U
153 360 R
215 301 L
206 412 D
229 371 U
263 430 L
107 242 L
94 69 L
77 97 L
77 477 U
208 439 D
184 387 U
141 283 U
270 362 R
149 29 U
175 473 D
99 421 R
199 26 R
175 198 D
233 394 U
185 111 U
48 55 U
52 399 L
231 40 D
86 470 R
114 472 D
192 496 D
201 398 U
214 103 L
65 29 D
285 198 U
159 215 L
167 23 L
225 259 U
231 418 R
144 214 D
228 75 L
1 163 L
164 466 L
196 4 U
173 251 D
15 69 L
213 344 L
4 360 L
272 400 D
281 492 U
206 111 U
155 95 U
27 470 R
7 277 R
19 70 R
249 296 R
167 95 U
123 202 U
49 239 R
40 435 L
211 427 L
165 51 D
114 363 L
269 309 D
90 415 L
189 301 D
119 181 D
121 175 L
3 357 L
152 409 L
208 492 L
120 171 R
68 319 U
12 246 R
282 82 L
215 294 U
25 21 L
266 448 D
256 36 R
286 59 R
228 428 L
47 269 L
276 472 D
178 405 U
190 255 L
82 245 D
92 335 D
166 422 L
175 18 L
172 241 U
250 467 L
160 229 U
241 282 L
86 356 L
222 213 U
216 80 L
177 137 D
97 24 U
119 119 R
257 198 U
76 13 U
80 215 L
90 315 D
174 134 L
55 335 U
23 85 D
6 13 D
27 221 R
53 80 R
43 122 U
3 247 R
141 464 D
215 464 D
202 139 U
3 55 L
93 226 L
277 477 D
38 67 R
149 413 U
188 457 L
39 439 U
258 247 L
111 109 R
136 227 D
150 39 U
56 95 R
91 475 D
132 239 R
275 405 D
44 17 U
12 267 L
234 172 D
224 284 D
51 339 D
197 470 R
129 433 U
168 393 U
11 282 L
12 241 D
193 10 R
41 387 D
134 284 L
15 86 R
206 174 D
16 110 U
51 28 L
107 73 R
172 223 L
272 46 U
259 373 D
42 165 R
255 200 R
271 386 L
200 94 R
177 127 D
63 25 L
222 222 R
137 275 R
237 262 R
222 171 R
133 16 R
281 403 R
14 87 L
152 109 R
159 467 R
232 396 L
201 407 R
242 315 R
67 321 U
294 168 U
221 363 L
187 215 L
46 8 L