package hr.fer.labos.lab2;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Lab2 {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Input a natural number n: ");
        int n = scanner.nextInt();
        System.out.print("Input a natural number k_1: ");
        int k1 = scanner.nextInt();
        System.out.print("Input a natural number k_2: ");
        int k2 = scanner.nextInt();
        System.out.print("Input a natural number k_3: ");
        int k3 = scanner.nextInt();
        System.out.print("Input a natural number k_4: ");
        int k4 = scanner.nextInt();

        int[] k_values = { k1, k2, k3, k4 };
        NodeList nodes = new NodeList(n);
        nodes.fillNeighbours(k_values);

        List<Node> start = new ArrayList<>();
        start.add(nodes.getNodes().get(0));

        if(nodes.connected(new ArrayList<>(), start)) {
            System.out.print("\n Graph G is a connected graph");
            if(nodes.hamilton(new ArrayList<>(), nodes.getNodes().get(0))) {
                System.out.print("\n\n Graph G is a Hamiltonian graph");
            } else {
                System.out.print("\n\n Graph G is not a Hamiltonian graph");
            }
        } else {
            System.out.println("\n\n Graph G is not a connected graph so it is not a Hamiltonian graph");
        }
    }
}