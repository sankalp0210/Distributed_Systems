import java.rmi.Remote; 
import java.rmi.RemoteException;  

public interface Graph extends Remote {  
	void add_graph(String graphId, int numNodes) throws RemoteException;
	void add_edge(String graphId, int node1, int node2, int weight) throws RemoteException;
	int get_mst(String graphId) throws RemoteException;
}
