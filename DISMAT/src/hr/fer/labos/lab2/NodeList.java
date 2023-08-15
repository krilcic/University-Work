package hr.fer.labos.lab2;

import java.util.ArrayList;
import java.util.List;


public class NodeList {

    private final int size;
    private final List<Node> nodes = new ArrayList<>();

    public NodeList(int size) {
        for (int i = 0; i < size; i++) {
            this.nodes.add(new Node());
        }

        this.size = size;
    }

    public List<Node> getNodes() {
        return this.nodes;
    }

    public void fillNeighbours(int[] k_values){
        for (int i = 1; i < this.size + 1; i++) {
            for (int j = i; j < this.size + 1; j++) {
                for (int k_value : k_values) {
                    if (Math.abs(i - j) == k_value) {
                        Node iNode = this.nodes.get(i - 1);
                        Node jNode = this.nodes.get(j - 1);

                        iNode.getNeighbours().add(jNode);
                        jNode.getNeighbours().add(iNode);
                    }
                }
            }
        }
    }

    public boolean connected(List<Node> visited, List<Node> current){
        visited.addAll(current);
        if(visited.size() == this.size){
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

        return next.size() != 0 && this.connected(visited, next);
    }

    public boolean hamilton(List<Node> visited, Node current){
        visited.add(current);

        if(visited.size() == this.size && current.getNeighbours().contains(visited.get(0)) && this.size != 1 && this.size != 2){
            return true;
        }

        List<Node> neighbours = current.getNeighbours();

        for (Node neighbour : neighbours) {
            if (!visited.contains(neighbour)) {
                if (this.hamilton(visited, neighbour)) {
                    return true;
                }
            }
        }

        int size = visited.size();
        visited.remove(size - 1);
        return false;
    }
}
