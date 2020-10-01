import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;;

public class Server {
    public static void main(String args[]) {
        try {
            if(args.length!=1){
                System.out.println("Error: Specify Port. Usage: java Server <port>");
                System.exit(0);
            }
            Integer port = Integer.parseInt(args[0]);

            LocateRegistry.createRegistry(port);
            GraphImpl obj = new GraphImpl();
            Naming.rebind("rmi://0.0.0.0:"+ port.toString() + "/graph", obj);
        }
        catch(Exception exc) {
            System.out.println(exc);
        } 
    }
}
