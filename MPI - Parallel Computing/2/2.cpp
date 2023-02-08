#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

// U: 0, D: 1, L: 2, R: 3
int n, m , k , t;

// Initialization
void initParticles(vector<vector<int>> &particles, int k)
{
  for (int i = 0; i < k; i++)
  {
    vector<int> p;
    int x, y, direction;
    char dir;
    cin >> x >> y >> dir;
    if (dir == 'U')
      direction = 0;
    else if (dir == 'D')
      direction = 1;
    else if (dir == 'L')
      direction = 2;
    else if (dir == 'R')
      direction = 3;
    p.push_back(x);
    p.push_back(y);
    p.push_back(direction);
    particles.push_back(p);
  }
}

// Move particle in a specific direction.
void moveParticle(vector<int> &particle)
{
  switch (particle[2])
  {
    case 0:
      particle[1]++;
      break;
    case 1:
      particle[1]--;
      break;
    case 2:
      particle[0]--;
      break;
    case 3:
      particle[0]++;
      break;
  }
  // if (particle[0] < 0 || particle[0] >= n) cout << "xx" << endl;
  // if (particle[1] < 0 || particle[1] >= m) cout << "yy" << endl;
}

int main(int argc, char *argv[])
{
  time_t st, en;
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Map to store particles in a partcular position under current process.
  map<pair<int, int>, vector<int>> position_to_particles;
  vector<vector<int>> particles;
  // n: number of cols, m: number of rows, k: no. of particles:
  char split = 'y'; // split type

  if (rank == 0)
  {
    cin >> n >> m >> k >> t;
    time(&st);
    initParticles(particles, k);
    if (n > m)
    {
      split = 'x';
    }
  }

  // Broadcast grid size information to all processes
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&split, 1, MPI_CHAR, 0, MPI_COMM_WORLD);

  int chunk, start, end, ind;

  if (split == 'y')
  {
    chunk = m;
    if (size > 1) {
      chunk = m / (size - 1);
    }
    start = rank * chunk;
    end = start + chunk;
    ind = 1;
    if (rank == size - 1)
      end = m;
    if (size > m) {
      start = rank;
      end = rank + 1;
      chunk = 1;
      if (rank >= m) {
          end = start;
      }
    }
  }

  if (split == 'x')
  {
    chunk = n;
    if (size > 1) {
      chunk = n / (size - 1);
    }
    start = rank * chunk;
    end = start + chunk;
    ind = 0;
    if (rank == size - 1)
      end = n;
    if (size > n) {
      start = rank;
      end = rank + 1;
      chunk = 1;
      if (rank >= n) {
          end = start;
      }
    }
  }

  map<int, vector<int>> distribute;

  // Distribute particles based on position in the beginning.
  if (rank == 0) {
    for (int i = 0; i < particles.size(); i++) {
      int dest = particles[i][ind] / chunk;
      distribute[dest].push_back(i);
    }
    for (int d = 1; d < size; d++) {
      int out = 0;
      if (distribute.find(d) != distribute.end()) out = distribute[d].size();
      MPI_Send(&out, 1, MPI_INT, d, 0, MPI_COMM_WORLD);
      if (out) {
        for (auto par : distribute[d])
          MPI_Send(particles[par].data(), 3, MPI_INT, d, 0, MPI_COMM_WORLD);
      }
    }
    for (int i = 0; i < particles.size(); i++) {
      if (particles[i][ind] < start || particles[i][ind] >= end)
      {
        particles.erase(particles.begin() + i);
        i--;
      }
    }
  }
  else {
    int in;
    MPI_Recv(&in, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (in)
    {
      vector<int> in_par(3);
      for (int prt = 0; prt < in; prt++)
      {
        MPI_Recv(in_par.data(), 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        particles.push_back(in_par);
      }
    }
  }

  // Assumption: No collision in the initial state.
  for (int step = 0; step < t; step++)
  {
    // CALCULATE WHAT TO SEND TO RANK + 1 (next) and RANK - 1 (prev) as needed.
    // Each chunk: >= start, < end

    vector<vector<int>> next, prev;
    for (int i = 0; i < particles.size(); i++)
    {
      // If particle is outside process chunk, send particle information over to correct process.
      if (particles[i][ind] < start || particles[i][ind] >= end)
      {
        int dest = particles[i][ind] / chunk;
        if (dest == rank - 1)
          prev.push_back(particles[i]);
        else if (dest == rank + 1)
          next.push_back(particles[i]);
        else
          cout << "wtf " << rank << " " << dest << " " << particles[i][ind]
               << " " << start << " " << end << endl;
        particles.erase(particles.begin() + i);
        i--;
      }
    }
    // SEND TO RANK - 1, RECEIVE FROM RANK + 1, SEND TO RANK + 1, RECEIVE FROM RANK - 1.
    vector<int> incoming_particle(3);
    int num_incoming, num_outgoing;
    if (rank - 1 >= 0)
    {
      num_outgoing = prev.size();
      MPI_Send(&num_outgoing, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
      if (num_outgoing)
      {
        for (auto it : prev)
          MPI_Send(it.data(), 3, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
      }
    }
    if (rank + 1 < size)
    {
      MPI_Recv(&num_incoming, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (num_incoming)
      {
        for (int part = 0; part < num_incoming; part++)
        {
          MPI_Recv(incoming_particle.data(), 3, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          particles.push_back(incoming_particle);
        }
      }
      num_outgoing = next.size();
      MPI_Send(&num_outgoing, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
      if (num_outgoing)
      {
        for (auto it : next)
          MPI_Send(it.data(), 3, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
      }
    }
    if (rank - 1 >= 0)
    {
      MPI_Recv(&num_incoming, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (num_incoming)
      {
        for (int part = 0; part < num_incoming; part++)
        {
          MPI_Recv(incoming_particle.data(), 3, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          particles.push_back(incoming_particle);
        }
      }
    }

    next.clear();
    prev.clear();

    // COLLISION CHECK AND UPDATE PARTICLE POSITIONS AND DIRECTIONS
    position_to_particles.clear();
    // cout << position_to_particles.size() << endl;

    // Assumption: No collision at edges and corners.
    for (int i = 0; i < particles.size(); i++)
    {
      // For head-on two-only particle collisions.
      if (!(particles[i][0] == 0 || particles[i][0] == n - 1 || particles[i][1] == 0 || particles[i][1] == m - 1))
        position_to_particles[{particles[i][0], particles[i][1]}].push_back(i);

      // For wall collisions.
      if (particles[i][2] == 2 && particles[i][0] == 0)
        particles[i][2] = 3;
      else if (particles[i][2] == 3 && particles[i][0] == n - 1)
        particles[i][2] = 2;
      else if (particles[i][2] == 0 && particles[i][1] == m - 1)
        particles[i][2] = 1;
      else if (particles[i][2] == 1 && particles[i][1] == 0)
        particles[i][2] = 0;
    }

    for (auto entry : position_to_particles)
    {
      // Only two particles.
      if (entry.second.size() != 2)
        continue;
      int i1 = entry.second[0], i2 = entry.second[1];
      if (particles[i1][2] == 2 && particles[i2][2] == 3)
        particles[i1][2] = 0, particles[i2][2] = 1;
      else if (particles[i1][2] == 3 && particles[i2][2] == 2)
        particles[i1][2] = 1, particles[i2][2] = 0;
      else if (particles[i1][2] == 0 && particles[i2][2] == 1)
        particles[i1][2] = 3, particles[i2][2] = 2;
      else if (particles[i1][2] == 1 && particles[i2][2] == 0)
        particles[i1][2] = 2, particles[i2][2] = 3;
    }

    for (int i = 0; i < particles.size(); i++)
    {
      // Move particles assigned to this process.
      moveParticle(particles[i]);
    }
  }

  // Wait till all processes are finished.
  MPI_Barrier(MPI_COMM_WORLD);
  for (auto it : particles)
  {
    char dir;
    if (it[2] == 0)
      dir = 'U';
    if (it[2] == 1)
      dir = 'D';
    if (it[2] == 2)
      dir = 'L';
    if (it[2] == 3)
      dir = 'R';
    cout << it[0] << " " << it[1] << " " << dir << endl;
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
Input 1
4 6 4 15
0 1 R
2 1 L
3 3 U
1 0 D

Output 1
1 3 R
1 3 D
3 2 D
3 3 R

Input 2
10 12 16 100
0 1 R
2 1 L
3 3 U
9 9 L
6 8 D
4 8 L
7 3 R
5 6 U
0 1 U
2 1 D
3 3 R
9 9 U
6 8 L
4 8 D
7 3 U
5 6 R

Output 2
0 6 L
0 9 D
2 11 U
3 3 R
3 6 L
3 7 D
4 10 D
5 3 R
5 4 D
6 4 U
7 5 U
7 9 R
7 10 R
8 6 L
9 1 D
9 10 R
*/