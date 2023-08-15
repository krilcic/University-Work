package hr.fer.labos.lab3;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Lab3 {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Input a natural number n: ");
        int n = scanner.nextInt();
        System.out.print("Input a natural number a: ");
        int a = scanner.nextInt();
        System.out.print("Input a natural number b: ");
        int b = scanner.nextInt();
        System.out.print("Input a natural number c: ");
        int c = scanner.nextInt();

        NodeList nodes = new NodeList(n);
        nodes.fillNeighbours(a,b,c);

        List<Node> start = new ArrayList<>();
        start.add(nodes.getNodes().get(0));

        if(nodes.connected(new ArrayList<>(), start) && n <= 2) {
            System.out.print("\nGraph G is a connected graph, but Prufers code can not be made.\n");
        } else if(nodes.connected(new ArrayList<>(), start) && n > 2) {
            System.out.print("\nGraph G is a connected Graph\n");
            Tree tree = new Tree(nodes.getNodeStart(),nodes.getNodeEnd(), n);
            System.out.print("Prufer code of minimum spanning tree: ");
            System.out.println(tree.pruferCode());
        } else {
            System.out.println("\n\n Graph G is not connected");
        }
    }
}
