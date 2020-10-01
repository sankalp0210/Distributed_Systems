#include "stdio.h"
#include "stdlib.h"
#include "vector"
#include "mpi.h"

using namespace std;
typedef long long ll;

void merge(ll* final_arr, ll* arr1, ll* arr2, ll n1, ll n2){

    ll i=0, j=0, k=0;

    while(i < n1 and j < n2)
    {
        if(arr1[i] < arr2[j])
        {
            final_arr[k++] = arr1[i++];
        }
        else
        {
            final_arr[k++] = arr2[j++];
        }
    }
    while(i < n1)
    {
        final_arr[k++] = arr1[i++];
    }
    while(j < n2)
    {
        final_arr[k++] = arr2[j++];
    }
}

ll partition(ll *arr, ll low, ll high){
    ll pivot = arr[high];
    ll i = low - 1;
    for(ll j=low;j<=high;j++)
    {
        if(arr[j] < pivot)
        {
            i++;
            ll temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    i++;
    ll temp = arr[i];
    arr[i] = arr[high];
    arr[high] = temp;
    return i;
}

void quicksort(ll *arr, ll low, ll high)
{
    if (low < high)
    {
        ll pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot+1, high);
    }
}

int main(int argc, char **argv) {

    int rank, num_procs;
    vector<ll> arr;

    ll num = 1;
    ll temp = 0;

    FILE* file;
    file = fopen(argv[1], "r+");
    while(num > 0){
        num = fscanf(file, "%lld", &temp);
        if(num > 0)
            arr.push_back(temp);
    }
    fclose(file);
    ll num_elements = arr.size();
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    MPI_Bcast(&num_elements, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    ll elements_per_proc = num_elements / num_procs;
    ll rem_elements = num_elements % num_procs;
	ll *sub_arr  = (ll*)malloc(sizeof(ll)*elements_per_proc);
    MPI_Barrier(MPI_COMM_WORLD);

	MPI_Scatter(&arr[rem_elements], elements_per_proc, MPI_LONG_LONG, sub_arr, elements_per_proc, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    quicksort(sub_arr, 0, elements_per_proc-1);
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0)
    {

    	ll *temp_arr = (ll*)malloc(sizeof(ll)*elements_per_proc);
        ll ct = rem_elements + elements_per_proc;
        ll *final_arr = (ll*)malloc(sizeof(ll)*ct);
        for(ll i=0;i<rem_elements;i++)
        {
            temp_arr[i] = arr[i];
        }
        quicksort(temp_arr, 0, rem_elements-1);
        merge(final_arr, temp_arr, sub_arr, rem_elements, elements_per_proc);
        sub_arr = final_arr;

        for(ll i=1;i<num_procs;i++)
        {
            ct += elements_per_proc;
            final_arr = (ll*)malloc(sizeof(ll)*ct);
            MPI_Recv(temp_arr, elements_per_proc, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            merge(final_arr, sub_arr, temp_arr, ct-elements_per_proc, elements_per_proc);
            sub_arr = final_arr;
        }
    }

    else
    {
        MPI_Send(sub_arr, elements_per_proc, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

    if(rank == 0){
        printf( "Total time (s): %f\n", maxTime );
        FILE* file;
        file = fopen(argv[2], "w+");
        for(ll i=0;i<num_elements;i++)
        {
            fprintf(file, "%lld ", sub_arr[i]);
        }
        fprintf(file, "\n");
        fclose(file);
    }
    MPI_Finalize();

    return 0;
}