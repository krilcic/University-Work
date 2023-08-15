package hr.fer.labos.lab3;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class Node {
    private final int index;
    private final List<Node> neighbours = new ArrayList<>();
    private final HashMap<Integer,Integer> weightedNeigbours = new HashMap<>();

    public Node(int index) {
        this.index = index;
    }

    public HashMap<Integer, Integer> getWeightedNeigbours() {
        return this.weightedNeigbours;
    }

    public int getIndex() {
        return this.index;
    }

    public List<Node> getNeighbours() {
        return this.neighbours;
    }
}