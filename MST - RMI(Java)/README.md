# Distributed System Assignment - 2 
Sankalp Agarwal (20171161)

--- 
# Architecture
The java application uses RMI to invoke methods on an object running in another JVM.
The whole project comprises of 4 files:-
- Server
- Client
- Graph Implementation and 
- a graph interface file.

# MST Algorithm
Here, we are using kruskal's algorithm(using Disjoint Set Union) to find the MST of a given graph.
We calculate MST on the fly whenever a client sends a request for MST
# Results and Observations
The server can handle request from multiple client. Any client can work with any graph.  