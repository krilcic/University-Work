package hr.fer.labos.lab1;

import java.util.*;

public class SimEnka {

    public static void main(String[] args){
        List<String> lines = new ArrayList<>();
        Scanner scanner = new Scanner(System.in);
        String line;
        while (scanner.hasNextLine()) {
            line = scanner.nextLine();
            lines.add(line);
        }

        String[] entry = lines.get(0).split("\\|");
        String start = lines.get(4);
        int size = lines.size();
        Map<List<String>, List<String>> transition = new HashMap<>();

        for (int i = 5; i < size; i++) {
            line = lines.get(i);
            String[] temp = line.split("->");
            String[] inputs = temp[0].split(",");
            String[] exits = temp[1].split(",");

            List<String> stateInput = new ArrayList<>();
            stateInput.add(inputs[0]);
            stateInput.add(inputs[1]);

            int length = exits.length;
            List<String> exitss = new ArrayList<>(Arrays.asList(exits).subList(0, length));

            transition.put(stateInput,exitss);
        }

        for (String item : entry) {
            List<String> eStates = new ArrayList<>();
            eStates.add(start);

            boolean change = true;
            while (change) {
                change = false;

                Set<String> tempList = new HashSet<>();
                for (String state : eStates) {
                    List<String> temp = new ArrayList<>();
                    temp.add(state);
                    temp.add("$");
                    List<String> transitions = transition.get(temp);
                    if (transitions != null) {
                        for (String value : transitions) {
                            if (!eStates.contains(value)) {
                                tempList.add(value);
                                change = true;
                            }
                        }
                    }
                }

                if (!tempList.isEmpty()) {
                    eStates.addAll(tempList);
                }
            }

            String[] tempEntry = item.split((","));
            for (String s : tempEntry) {
                List<String> newStates = new ArrayList<>();

                if (eStates.size() != 1) {
                    eStates.remove("#");
                }

                Collections.sort(eStates);
                int eStateSize = eStates.size();
                for (int k = 0; k < eStateSize; k++) {
                    if (k == eStateSize - 1) {
                        System.out.print(eStates.get(k) + "|");
                    } else {
                        System.out.print(eStates.get(k) + ",");
                    }
                }

                for (String eState : eStates) {
                    List<String> temp = new ArrayList<>();
                    temp.add(eState);
                    temp.add(s);

                    List<String> transitions = transition.get(temp);
                    if (transitions == null) continue;

                    for (String transit : transitions) {
                        if(!newStates.contains(transit) && !transit.equals("#")){
                            newStates.add(transit);
                        }
                    }
                }

                change = true;
                while (change) {
                    change = false;

                    Set<String> tempList = new HashSet<>();

                    for (String state : newStates) {
                        List<String> temp = new ArrayList<>();
                        temp.add(state);
                        temp.add("$");

                        List<String> transition2 = transition.get(temp);
                        if (transition2 == null) continue;

                        for (String transit : transition2){
                            if(!newStates.contains(transit)){
                                if(!transit.equals("#")){
                                    tempList.add(transit);
                                    change = true;
                                }
                            }
                        }
                    }

                    if (!tempList.isEmpty()) {
                        newStates.addAll(tempList);
                    }
                }

                eStates.clear();
                if (newStates.size() == 0) {
                    eStates.add("#");
                } else {
                    eStates = newStates;
                }
            }
            if (eStates.size() != 1) {
                eStates.remove("#");
            }

            Collections.sort(eStates);
            int eStatesSize = eStates.size();

            for (int k = 0; k < eStatesSize; k++) {
                if (k == eStatesSize - 1) {
                    System.out.print(eStates.get(k));
                } else {
                    System.out.print(eStates.get(k) + ",");
                }
            }

            System.out.print("\n");
        }
    }
}