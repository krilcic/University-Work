package hr.fer.labos.lab3;

import java.util.ArrayList;
import java.util.List;

public class NodeList {

    private final int size;
    private final List<Node> nodes = new ArrayList<>();
    private int minWeight = Integer.MAX_VALUE;
    private Node nodeStart;
    private Node nodeEnd;

    public NodeList(int size) {
        for (int i = 0; i < size; i++) {
            this.nodes.add(new Node(i + 1));
        }

        this.size = size;
    }

    public List<Node> getNodes() {
        return this.nodes;
    }

    public Node getNodeStart() {
        return this.nodeStart;
    }

    public Node getNodeEnd() {
        return this.nodeEnd;
    }

    public void fillNeighbours(int a, int b, int c) {
        for (int i = 1; i < this.size + 1; i++) {
            for (int j = i + 1; j < this.size + 1; j++) {
                Node node1 = this.nodes.get(i - 1);
                Node node2 = this.nodes.get(j - 1);

                int weight = Math.abs(((a * i) - (b * j))) / c;

                node1.getWeightedNeigbours().put(j, weight);
                node2.getWeightedNeigbours().put(i, weight);

                if(weight != 0){
                    node1.getNeighbours().add(node2);
                    node2.getNeighbours().add(node1);

                    if(weight <= this.minWeight) {
                        this.nodeStart = node1;
                        this.nodeEnd = node2;
                        this.minWeight = weight;
                    }
                }
            }
        }
    }

    boolean connected(List<Node> visited, List<Node> current) {
        visited.addAll(current);

        if (visited.size() == this.size) {
            return true;
        }

        List<Node> next = new ArrayList<>();
        for (Node node : current) {
            List<Node> neighbours = node.getNeighbours();
            for (Node neighbour : neighbours) {
                if (!visited.contains(neighbour) && !next.contains(neighbour)) {
                    next.add(neighbour);
                }
            }
        }

        return next.size() == 0 ? false : this.connected(visited, next);
    }
}