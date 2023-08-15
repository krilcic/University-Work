package hr.fer.labos.lab3;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Tree {

    private final int size;
    private final List<Node> nodes = new ArrayList<>();
    private final List<Node> visited = new ArrayList<>();

    public Tree(Node node1, Node node2, int size) {
        for (int i = 0; i < size; i++) {
            this.nodes.add(new Node(i + 1));
        }

        this.connect(node1,node2);

        this.visited.add(node1);
        this.visited.add(node2);
        this.size = size;
    }

    private void connect(Node node1, Node node2) {
        this.nodes.get(node1.getIndex() - 1).getNeighbours().add(node2);
        this.nodes.get(node2.getIndex() - 1).getNeighbours().add(node1);
    }

    private int getWeight(Node node1, Node node2) {
        return node1.getWeightedNeigbours().get(node2.getIndex());
    }

    private void add(Node node, Node parent) {
        this.connect(node,parent);
        this.visited.add(node);
    }

    public String pruferCode() {
        if(this.visited.size() == this.size){
            return this.pruferCode(1);
        }

        int minWeight = Integer.MAX_VALUE;
        Node nextNode = null;
        Node parentNode = null;

        for(int i = 0; i < this.visited.size(); i++){
            Node nodeI = this.visited.get(i);

            for(int j = 0; j < nodeI.getNeighbours().size(); j++){
                Node nodeIJ = nodeI.getNeighbours().get(j);
                if(this.visited.contains(nodeIJ)) continue;

                int weight = getWeight(nodeI, nodeIJ);
                if(weight < minWeight){
                    minWeight = weight;
                    nextNode = nodeIJ;
                    parentNode = nodeI;
                }
            }
        }

        this.add(nextNode, parentNode);
        return this.pruferCode();
    }

    private String pruferCode(int counter){
        int minIndex = Integer.MAX_VALUE;
        Node minLeaf = null;

        for (Node node : this.nodes) {
            if (node.getNeighbours().size() == 1 && node.getIndex() < minIndex) {
                minIndex = node.getIndex();
                minLeaf = node;
            }
        }

        if(minLeaf == null) {
            return null;
        }

        for (Node node : this.nodes) {
            Iterator<Node> nodeIterator = node.getNeighbours().iterator();

            while(nodeIterator.hasNext()) {
                Node neighbourNode = nodeIterator.next();

                if(neighbourNode.getIndex() == minLeaf.getIndex()) {
                    nodeIterator.remove();
                }
            }
        }

        this.nodes.remove(minLeaf);

        if (counter == 1) {
            System.out.print("(");
        }

        int ind = minLeaf.getNeighbours().get(0).getIndex();

        if(this.nodes.size() == 2) {
            return ind + ")";
        }

        return ind + "," + this.pruferCode(counter + 1);
    }
}