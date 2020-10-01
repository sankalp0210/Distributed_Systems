#include "stdio.h"
#include "stdlib.h"
#include "vector"
#include "limits.h"
#include "mpi.h"

using namespace std;
typedef long long ll;

typedef struct Edge {
    ll u, v, w;
} edge;

int main(int argc, char **argv) {

    int rank, num_procs, num_nodes, num_edges, source;

    FILE* file;
    file = fopen(argv[1], "r+");
    fscanf(file, "%d %d", &num_nodes, &num_edges);

    edge* edges = (edge*)malloc(sizeof(edge)*num_edges*2);

    for(int i=0;i<num_edges;i++)
    {
        ll u, v, w;
        fscanf(file, "%lld %lld %lld", &u, &v, &w);
        edges[2*i].u = u;edges[2*i].v = v;edges[2*i].w = w;
        edges[2*i+1].u = v;edges[2*i+1].v = u;edges[2*i+1].w = w;
    }
    num_edges *= 2;
    fscanf(file, "%d", &source);
    fclose(file);

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    MPI_Datatype MPI_EDGE;
    MPI_Type_contiguous(3, MPI_LONG_LONG, &MPI_EDGE);
    MPI_Type_commit(&MPI_EDGE);

    int elements_per_proc = num_edges / num_procs;
    int rem_elements = num_edges % num_procs;

    edge* sub_edges = (edge*)malloc(sizeof(edge)*elements_per_proc);
    ll dist[num_nodes + 1];
    ll dist2[num_nodes + 1];

    for(int i=0;i<=num_nodes;i++)
    {
        dist[i] = dist2[i] = LLONG_MAX;
    }
    dist[source] = 0;

    MPI_Bcast(&dist, num_nodes + 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

	MPI_Scatter(&edges[rem_elements], elements_per_proc, MPI_EDGE, sub_edges, elements_per_proc, MPI_EDGE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    for(int i=0;i<num_nodes;i++)
    {
        for(int i=0;i<elements_per_proc;i++)
        {
            ll u = sub_edges[i].u;
            ll v = sub_edges[i].v;
            ll w = sub_edges[i].w;
            if (dist[u] != LLONG_MAX && (dist[u] + w) < dist[v])
            {
                dist[v] = dist[u] + w;
            }
        }

        if(rank == 0)
        {   
            for(int i=0;i<rem_elements;i++)
            {
                ll u = edges[i].u, v = edges[i].v, w = edges[i].w;
                if (dist[u] != LLONG_MAX && (dist[u] + w) < dist[v])
                {
                    dist[v] = dist[u] + w;
                }
            }
        }

        for(int i=0;i<=num_nodes;i++)
        {
            dist2[i] = dist[i];
        }

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(dist2, dist, num_nodes + 1, MPI_LONG_LONG, MPI_MIN, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );

    if(rank == 0)
    {
        printf( "Total time (s): %f\n", maxTime );
        FILE* file;
        file = fopen(argv[2], "w+");
        for(int i=1;i<=num_nodes;i++)
        {
            if(dist[i] == LLONG_MAX) dist[i] = -1;
            fprintf(file, "%d %lld\n", i, dist[i]);
        }
        fclose(file);
    }

    MPI_Finalize();

    return 0;
}