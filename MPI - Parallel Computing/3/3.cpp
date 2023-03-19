#include <bits/stdc++.h>
#include <mpi.h>

#define MAX 505
#define INF 1000000000

using namespace std;

int n, freq[MAX], keys[MAX], cost[MAX][MAX], root[MAX][MAX];
map<int, int> par;

void setPar(int l, int r, int parent, int n) {
  int node = root[l][r];
  if (par[node] != -1) return;
  par[node] = parent;
  if (l >= r || l < 0 || r >= n) return;
  setPar(l, node - 1, node, n);
  setPar(node + 1, r, node, n);
}

void merge(vector<int> &arr1, vector<int> &arr2, vector<int> &result)
{
    int i = 0, j = 0, k = 0;
    while (i < arr1.size() && j < arr2.size())
    {
        if (arr1[i] < arr2[j]) result[k++] = arr1[i++];
        else result[k++] = arr2[j++];
    }
    while (i < arr1.size()) result[k++] = arr1[i++];
    while (j < arr2.size()) result[k++] = arr2[j++];
}

void merge_sort(vector<int> &arr)
{
    if (arr.size() > 1)
    {
        int mid = arr.size() / 2;
        vector<int> left(arr.begin(), arr.begin() + mid);
        vector<int> right(arr.begin() + mid, arr.end());
        merge_sort(left);
        merge_sort(right);
        vector<int> result(arr.size());
        merge(left, right, result);
        arr = result;
    }
}

int calcCost(int i, int j, int rank, int t)
{
  int total = 0;
  for (int k = i; k <= j; k++)
    total += freq[k];

  for (int r = i; r <= j; r++)
  {
    int leftCost = 0, rightCost = 0;
    if (r > i) leftCost = cost[i][r - 1];
    if (r < j) rightCost = cost[r + 1][j];

    int currentCost = total + leftCost + rightCost;
    if (currentCost < cost[i][j])
    {
      cost[i][j] = currentCost;
      root[i][j] = r;
    }
  }
  return cost[i][j];
}

void fillCost(int i, int j, int rank, int t)
{
  if (i == j) {
    cost[i][j] = freq[i];
    root[i][j] = i;
  }
  else if (j > i)
    cost[i][j] = calcCost(i, j, rank, t);
}

int main(int argc, char *argv[])
{
  time_t st, en;
  int rank, size, n;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    time(&st);
    cin >> n;
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int chunk = n;
  if (size > 2) {
    chunk = n / (size - 2);
  }
  int start = (rank - 1) * chunk;
  int end = start + chunk;
  if (rank == size - 1) end = n;

  if (size > n) {
    start = rank;
    end = rank + 1;
    chunk = 1;
    if (rank >= n) {
        end = start;
    }
  }

  if (rank == 0) {
    vector<int> arr(n);
    map<int, int> mp;
    for (int i = 0; i < n; i++) {
      int f;
      cin >> arr[i] >> f;
      mp[arr[i]] = f;
    }

    if (size < 3 || size > n) {
      merge_sort(arr);
      for (int i = 0; i < n; i++) {
        keys[i] = arr[i];
        freq[i] = mp[keys[i]];
        // cout << keys[i] << "-" << freq[i] << endl;
      }
    }
    else {

      for (int i = 1; i < size; i++) {
        if (i == size - 1) {
          MPI_Send(arr.data() + chunk * (i - 1), n - (chunk * (i - 1)), MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        else {
          MPI_Send(arr.data() + chunk * (i - 1), chunk, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
      }

      vector<int> all_arr;
      for (int i = 1; i < size; i++) {
        vector<int> received(n);
        int sz = chunk;
        if (i == size - 1) {
          MPI_Recv(received.data(), n - (chunk * (i - 1)), MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          sz = n - (chunk * (i - 1));
        }
        else {
          MPI_Recv(received.data(), chunk, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int k = 0; k < sz; k++) {
          all_arr.push_back(received[k]);
        }
      }
      vector<int> result(2 * n, INF);
      int start = 0;
      for (int i = 1; i < size; i++)
      {
        vector<int> l_arr(all_arr.begin() + start, i == size - 1 ? all_arr.end() : all_arr.begin() + start + chunk);
        vector<int> m_result(result.begin(), result.begin() + n);
        merge(l_arr, m_result, result);
        start += chunk;
      }
      for (int i = 0; i < n; i++) {
        keys[i] = result[i];
        freq[i] = mp[keys[i]];
        // cout << keys[i] << "-" << freq[i] << endl;
      }
    }
  }
  else {
    if (size > 2 && size <= n) {
      vector<int> local_arr(end - start);
      MPI_Recv(local_arr.data(), end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      merge_sort(local_arr);
      MPI_Send(local_arr.data(), local_arr.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
  }

  // Broadcast arrays sorted according to keys.
  MPI_Bcast(keys, n, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(freq, n, MPI_INT, 0, MPI_COMM_WORLD);


  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cost[i][j] = INF;
      root[i][j] = INF;
    }
  }

  chunk = n;
  if (size > 1) {
    chunk = n / (size - 1);
  }
  start = rank * chunk;
  end = start + chunk;

  if (rank == size - 1) end = n;

  if (size > n) {
    start = rank;
    end = rank + 1;
    chunk = 1;
    if (rank >= n) {
        end = start;
    }
  }

  int ts = 0;
  for (int t = 0; t < n; t++)
  {
    for (int ind = start; ind < end; ind++)
    {
      if (ind + t >= n)
        break;
      fillCost(ind, ind + t, rank, t);
    }
    for (int i = 0; i < n; i++) {
       for( int j = 0; j < n; j++) {
        // MPI_Allreduce to let other processes know the updated values. 
        MPI_Allreduce(MPI_IN_PLACE, &cost[i][j], 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        MPI_Allreduce(MPI_IN_PLACE, &root[i][j], 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
       }
    }
  }

  if (rank == 0) {
    cout << cost[0][n - 1] << endl;
    for (int w = 0; w < n; w++) {
      par[w] = -1;
    }
    setPar(0, n - 1, 0, n);
    for (int w = 0; w < n; w++) {
      int parent = w == root[0][n - 1] ? par[w] : (par[w] + 1);
      cout << parent << " ";
    }
    cout << endl;
  }

  // if (rank == 0) {
  //   time(&en);
  //   double time_taken = double(en - st);
  //   cout << "Time taken by program is : " << fixed << time_taken << setprecision(6);
  //   cout << " sec " << endl;
  // }

  MPI_Finalize();
  return 0;
}

/*
Input
8
5 7
3 6
7 5
6 17
1 16
4 8
2 13
8 28

Output
238
2 6 4 2 4 0 8 6
*/