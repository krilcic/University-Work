package hr.fer.labos.lab2;

import java.util.*;

public class MinDka {
    public static void main(String[] args) {
        List<String> lines = new ArrayList<>();
        Scanner scanner = new Scanner(System.in);
        String line;
        while (scanner.hasNextLine()) {
            line = scanner.nextLine();
            lines.add(line);
        }

        String[] alphabet = lines.get(1).split(",");
        String[] acceptableStates = lines.get(2).split(",");
        String start = lines.get(3);

        int size = lines.size();
        Map<List<String>, String> transition = new HashMap<>();
        for (int i = 4; i < size; i++) {
            line = lines.get(i);
            String[] temp = line.split("->");
            String[] inputs = temp[0].split(",");
            String exit = temp[1];

            List<String> stateInput = new ArrayList<>();
            stateInput.add(inputs[0]);
            stateInput.add(inputs[1]);

            transition.put(stateInput,exit);
        }

        List<String> possibleStates = new ArrayList<>();
        possibleStates.add(start);

        boolean change = true;
        while (change){
            change = false;
            for (int i = 0;i < possibleStates.size(); i++) {
                for (String symbol : alphabet) {
                    List<String> temp = new ArrayList<>();
                    temp.add(possibleStates.get(i));
                    temp.add(symbol);

                    String nextState = transition.get(temp);
                    if(!possibleStates.contains(nextState)) {
                        possibleStates.add(nextState);
                        change=true;
                    }
                }
            }
        }

        Collections.sort(possibleStates);

        List<String> finalStates = new ArrayList<>();
        for (String acceptableState : acceptableStates) {
            if (possibleStates.contains(acceptableState)) {
                finalStates.add(acceptableState);
            }
        }

       List<String> nonFinalStates = new ArrayList<>();
        for (String possibleState : possibleStates) {
            if (!finalStates.contains(possibleState)) {
                nonFinalStates.add(possibleState);
            }
        }

        Collections.sort(finalStates);
        Collections.sort(nonFinalStates);

        List<List<String>> groups = new ArrayList<>();

        groups.add(finalStates);
        groups.add(nonFinalStates);

        List<List<String>> minimized = minimize(groups,transition,alphabet);

        List<String> endStates = new ArrayList<>();

        for(List<String> mini : minimized){
            if(mini.size() == 1){
                endStates.add(mini.get(0));
            } else if (mini.size() > 1) {
                Collections.sort(mini);
                endStates.add(mini.get(0));
            }
        }

        Collections.sort(endStates);

        List<String> endAcceptableStates = new ArrayList<>();

        for(String s : acceptableStates){
            if (endStates.contains(s)){
                endAcceptableStates.add(s);
            }
        }

        Collections.sort(endAcceptableStates);

        //first row
        customPrint(endStates);

        System.out.print("\n");

        //second row
        for (int i = 0; i < alphabet.length; i++) {
            if (i == alphabet.length - 1){
                System.out.printf("%s", alphabet[i]);
            } else {
                System.out.printf("%s,", alphabet[i]);
            }
        }

        System.out.print("\n");

        //third row
        customPrint(endAcceptableStates);

        System.out.print("\n");


        //fourth row

        for(List<String> b : minimized){
            Collections.sort(b);

            if (b.contains(start)){
                System.out.printf("%s", b.get(0));
            }
        }

        System.out.print("\n");

        //row >=5
        for(String a : endStates){
            for (String s : alphabet) {
                List<String> temp = new ArrayList<>();
                temp.add(a);
                temp.add(s);

                System.out.printf("%s,%s->", a, s);

                if (!endStates.contains(transition.get(temp))) {
                    for (List<String> b : minimized) {
                        Collections.sort(b);

                        if (b.contains(transition.get(temp))) {
                            System.out.printf("%s", b.get(0));
                        }
                    }
                } else {
                    System.out.printf("%s", transition.get(temp));
                }
                System.out.print("\n");
            }
        }
    }

    private static void customPrint(List<String> endStates) {
        for (int i = 0; i < endStates.size(); i++) {
            if (i == endStates.size() - 1){
                System.out.printf("%s", endStates.get(i));
            } else {
                System.out.printf("%s,", endStates.get(i));
            }
        }
    }

    public static List<List<String>> minimize(List<List<String>> groups,Map<List<String>, String> transition, String[] alphabet){
        List<List<String>> result = new ArrayList<>();
        List<List<String>> newTemp = new ArrayList<>();

        for(List<String> g : groups) {
            if(g.size() <= 1) {
                continue;
            }

            List<String> newGroup = new ArrayList<>();

            for (int i = 0; i < g.size(); i++) {
                if(newGroup.contains(g.get(i))){
                    continue;
                }

                for (int j = i + 1; j < g.size(); j++) {
                    if(newGroup.contains(g.get(j))){
                        continue;
                    }

                    for (String s : alphabet) {
                        List<String> temp = new ArrayList<>();
                        temp.add(g.get(i));
                        temp.add(s);

                        List<String> temp2 = new ArrayList<>();
                        temp2.add(g.get(j));
                        temp2.add(s);

                        String state1 = transition.get(temp);
                        String state2 = transition.get(temp2);
                        boolean cont = false;

                        for (List<String> gg : groups) {
                            if (gg.contains(state1) && gg.contains(state2)) {
                                cont = true;
                                break;
                            }
                        }

                        if (!cont) {
                            newGroup.add(g.get(j));
                            break;
                        }
                    }

                }


            }
            if(newGroup.size() > 0){
                result.add(newGroup);
                newTemp.add(newGroup);
            } else {
                result.add(g);
            }
        }
        for(List<String> ggg : groups){
            for(List<String> s : newTemp){
                for (String ss : s){
                    ggg.remove(ss);
                }
            }

            if(!result.contains(ggg)) {
                result.add(ggg);
            }
        }

        if(groups.size() == result.size()){
            return result;
        } else {
            return minimize(result,transition,alphabet);
        }
    }
}