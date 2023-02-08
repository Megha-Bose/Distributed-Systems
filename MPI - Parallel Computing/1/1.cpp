#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

// grid boundary
double x_min = -1.5, x_max = 1;
double y_min = -1, y_max = 1;

// z_{i+1} = z_{i} ^ 2 + c
// Returns 0 if x + iy escapes the Mandelbrot set
// before k iterations, else returns 1.
int mandelbrot(double x, double y, int k)
{
    int i = 0;
    double z_r = 0, z_i = 0;
    double znext_r, znext_i;
    while (i < k && z_r * z_r + z_i * z_i <= 4.0)
    {
        znext_r = (z_r * z_r) - (z_i * z_i) + x;
        znext_i = (2.0 * z_r * z_i) + y;
        z_r = znext_r;
        z_i = znext_i;
        i++;
    }
    return i < k ? 0 : 1;
}

int main(int argc, char **argv)
{
    time_t start, end;
    MPI_Init(&argc, &argv);

    int n, m, k, rank, size;

    // Get rank of the process.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Get the number of processes in the communicator.
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // If rank 0 process, then take input.
    if (rank == 0)
    {
        cin >> n >> m >> k;
        time(&start);
    }

    // Broadcasting input values n, m, k from rank 0 process to others.
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Computing the cell size for x and y coordinates.
    double cell_width = (x_max - x_min) / (m - 1);
    double cell_height = (y_max - y_min) / (n - 1);

    // Assigning horizontal strip of grid n * m for each process to compute.

    int num_process_rows = n;
    if (size > 1) {
        num_process_rows = n / (size - 1);
    }

    int start_row = num_process_rows * rank;
    int end_row = num_process_rows * (rank + 1);
    if (rank == size - 1)
        end_row = n; // last process

    if (size > n) {
        start_row = rank;
        end_row = rank + 1;
        num_process_rows = 1;
        if (rank >= n) {
            end_row = start_row;
        }
    }
    
    // Data computed by each process.
    int process_buf[num_process_rows * m] = {0};

    // Processing horizontal strip assigned to process.
    int index = 0;
    for (int row = start_row; row < end_row; row++)
        for (int col = 0; col < m; col++)
            process_buf[index++] = mandelbrot(x_min + col * cell_width, y_min + row * cell_height, k);

    // if (rank == 0) {
    //     time(&end);
    //     double time_taken = double(end - start);
    //     cout << "Time taken by program is : " << fixed << time_taken << setprecision(6);
    //     cout << " sec " << endl;
    // }

    int total_buf[n * m];
    MPI_Gather(process_buf, num_process_rows * m, MPI_INT, total_buf, num_process_rows * m, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // cout << size << " " << num_process_rows << endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cout << total_buf[i * m + j] << " ";
            }
            cout << endl;
        }
    }

    

    MPI_Finalize();
    return 0;
}

/*
Input
16 16 1000

Output
0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0
0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0
0 0 0 0 0 0 1 1 1 1 1 0 0 0 0 0
0 0 0 0 0 1 1 1 1 1 1 1 0 0 0 0
0 0 0 1 0 1 1 1 1 1 1 1 0 0 0 0
0 0 1 1 1 1 1 1 1 1 1 1 0 0 0 0
0 0 1 1 1 1 1 1 1 1 1 1 0 0 0 0
0 0 0 1 0 1 1 1 1 1 1 1 0 0 0 0
0 0 0 0 0 1 1 1 1 1 1 1 0 0 0 0
0 0 0 0 0 0 1 1 1 1 1 0 0 0 0 0
0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0

*/