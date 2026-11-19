
#include <iostream>
#include <vector>
#include <omp.h>
#include <cmath>

using namespace std;

#include <random>

void generateMatrix(vector<int>& arr, int N)
{
    int total = N * N;

#pragma omp parallel
    {
        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<> distrib(0, 1000);

#pragma omp for
        for (int i = 0; i < total; i++)
        {
            arr[i] = distrib(gen);
        }
    }
}


// Parallel Bitonic Sort
void bitonicSort(vector<int>& arr, int n)
{
   
    for (int k = 2; k <= n; k *= 2)
    {
        for (int j = k / 2; j > 0; j /= 2)
        {
#pragma omp parallel for
            for (int i = 0; i < n; i++)
            {
                int ixj = i ^ j;

                if (ixj > i)
                {
                    if ((i & k) == 0)
                    {
                        if (arr[i] > arr[ixj])
                            swap(arr[i], arr[ixj]);
                    }
                    else
                    {
                        if (arr[i] < arr[ixj])
                            swap(arr[i], arr[ixj]);
                    }
                }
            }
        }
    }
}



void sequentialBitonicSort(vector<int>& arr, int n)
{
    for (int k = 2; k <= n; k *= 2)
    {
        for (int j = k / 2; j > 0; j /= 2)
        {
            for (int i = 0; i < n; i++)
            {
                int ixj = i ^ j;

                if (ixj > i)
                {
                    if ((i & k) == 0)
                    {
                        if (arr[i] > arr[ixj])
                            swap(arr[i], arr[ixj]);
                    }
                    else
                    {
                        if (arr[i] < arr[ixj])
                            swap(arr[i], arr[ixj]);
                    }
                }
            }
        }
    }
}

int main()
{

    omp_set_num_threads(20);

    int N;

    std::cout << "Available processors: "
        << omp_get_num_procs() << std::endl;

    std::cout << "Maximum OpenMP threads: "
        << omp_get_max_threads() << std::endl;

    cout << "Enter matrix dimension N: ";
    cin >> N;

    int total = N * N;

    if (total <= 0 || (total & (total - 1)) != 0)
    {
        cout << "N*N must be a power of 2!\n";
        return 1;
    }

    vector<int> arr(total);

    //cout << "Enter matrix elements:\n";

    //for (int i = 0; i < total; i++)
    //    cin >> arr[i];

    generateMatrix(arr, N);


    // sorting
    double start = omp_get_wtime();

    bitonicSort(arr, total);
    //sequentialBitonicSort(arr, total);


    double end = omp_get_wtime();


    //cout << "\nSorted matrix:\n";

    //for (int i = 0; i < total; i++)
    //{
    //    cout << arr[i] << "\t";

    //    if ((i + 1) % N == 0)
    //        cout << "\n";
    //}

    cout << "\nExecution time: "
        << end - start << " seconds\n";

    return 0;
}
