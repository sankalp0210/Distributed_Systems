import java.io.*;
import java.util.*;
import java.rmi.Naming;

public class Client{
    public static void main(String args[]){
        try {
            if(args.length!=2){
                System.out.println("Error: Specify Port. Usage: java ShareFile <server-ip> <server-port>");
                System.exit(0);
            }

			String server_ip = args[0];  
            Integer server_port = Integer.valueOf(args[1]);
            Graph obj = (Graph)Naming.lookup("rmi://" + server_ip + ":" + server_port + "/graph");

            BufferedReader buffReader = new BufferedReader(new InputStreamReader(System.in));
            while(true){
                try{
                    String input = buffReader.readLine();
					if(input == null)
                        break;

                    String[] inp = new String[5];
                    StringTokenizer st = new StringTokenizer(input);
                    int len = 0;
                    while (st.hasMoreTokens()){
                        if(len == 5){
                            System.out.println("Invalid command");
                            break;
                        }
                        inp[len++] = st.nextToken();
                    }

                    if(inp[0].equals("add_graph")){
                        if(len != 3){
                            System.out.println("Usage : add_graph <graph identifier> n");
                        }
                        int n = Integer.parseInt(inp[2]);
                        obj.add_graph(inp[1], n);
                    }

                    else if(inp[0].equals("add_edge")){
                        if(len != 5){
                            System.out.println("Usage : add_edge <graph identifier> <u> <v> <w>");
                        }
                        int u = Integer.parseInt(inp[2]);
                        int v = Integer.parseInt(inp[3]);
                        int w = Integer.parseInt(inp[4]);
                        obj.add_edge(inp[1], u, v, w);
                    }

                    else if(inp[0].equals("get_mst")){
                        if(len != 2){
                            System.out.println("Usage : get_mst <graph identifier>");
                        }
                        System.out.println(obj.get_mst(inp[1]));
                    }

                }
                catch(Exception exc) {
                    System.out.println(exc);
                }
            }
        }
        catch(Exception exc){
            System.out.println(exc);
        }
    }
}