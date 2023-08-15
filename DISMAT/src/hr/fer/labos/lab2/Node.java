package hr.fer.labos.lab2;

import java.util.ArrayList;
import java.util.List;

public class Node {

    private final List<Node> neighbours = new ArrayList<>();

    public List<Node> getNeighbours() {
        return this.neighbours;
    }
}
