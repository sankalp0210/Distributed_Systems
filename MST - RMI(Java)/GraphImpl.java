import java.rmi.*;
import java.rmi.server.*;
// import java.rmi.registry.*;
import java.util.*;
	
public class GraphImpl extends UnicastRemoteObject implements Graph {

	private Map<String, ArrayList<Integer[]>> graph = new HashMap<>();
	private Map<String, Integer> GraphName = new HashMap<>();
	private Map<String, Integer[]> par = new HashMap<>();

	GraphImpl() throws RemoteException{
		super();
	}

	public void add_graph(String graphId, int numNodes) {
		try{
			GraphName.put(graphId,numNodes);
			ArrayList<Integer[]> x  = new ArrayList<Integer[]>();
			Integer[] temp = new Integer[numNodes+1];
			graph.put(graphId, x);
			par.put(graphId, temp);
		}
		catch(Exception exc) {
			System.out.println(exc);
		}
	}

	public void add_edge(String graphId, int node1, int node2, int weight) {
		try{
			Integer[] temp = new Integer[3];
			temp[0] = weight;temp[1] = node1;temp[2] = node2;
			// Vector<Integer> temp = new Vector<Integer>();
			// temp.add(weight);temp.add(node1);temp.add(node2);
			graph.get(graphId).add(temp);
		}
		catch(Exception exc) {
			System.out.println(exc);
		}
	}
	private int find(String graphId, int x){
		if(par.get(graphId)[x] < 0)
			return x;
		else
			return find(graphId, par.get(graphId)[x]);
	}
	private int merge(String graphId, int x, int y){
		int px = find(graphId, x);
		int py = find(graphId, y);
		if(px == py)
			return 0;
		par.get(graphId)[px] = py;
		return 1;
	}
	public int get_mst(String graphId){
		try{
			ArrayList<Integer[]> temp = graph.get(graphId);
			Collections.sort(temp, new Comparator<Integer[]>() {
				@Override
				public int compare(Integer[] a1, Integer[] a2){
					return  a1[0].compareTo(a2[0]);
				}
			});
			int numNodes = GraphName.get(graphId);
			for(int i=0;i<=numNodes;i++){
				par.get(graphId)[i] = -1;
			}
			int ans = 0, edges = 0;
			for(int i=0;i<temp.size();i++){
				if(merge(graphId, temp.get(i)[1], temp.get(i)[2]) == 1){
					ans += temp.get(i)[0];
					edges++;
				}
				if(edges == (numNodes - 1)){
					break;
				}
			}
			if(edges != numNodes - 1)
				return -1;
			return ans;
		}
		catch(Exception exc) {
			System.out.println(exc);
			return -1;
		}
		// return ans;
	}
}
