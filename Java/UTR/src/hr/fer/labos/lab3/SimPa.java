package hr.fer.labos.lab3;

import java.util.*;

public class SimPa {
    public static void main(String[] args) {
        List<String> lines = new ArrayList<>();

        Scanner scanner = new Scanner(System.in);
        String line;
        while (scanner.hasNextLine()) {
            line = scanner.nextLine();
            lines.add(line);
        }

        String[] entry = lines.get(0).split("\\|");
        String[] acceptableStates = lines.get(4).split(",");
        List<String> acceptableStatess = new ArrayList<>();
        Collections.addAll(acceptableStatess, acceptableStates);
        String startState = lines.get(5);
        String startStack = lines.get(6);

        int size = lines.size();
        Map<List<String>, List<String>> transition = new HashMap<>();
        for (int i = 7; i < size; i++) {
            line = lines.get(i);
            String[] temp = line.split("->");
            String[] inputs = temp[0].split(",");
            String[] exits = temp[1].split(",");

            List<String> stateInput = new ArrayList<>();
            stateInput.add(inputs[0]);
            stateInput.add(inputs[1]);
            stateInput.add(inputs[2]);

            int length = exits.length;
            List<String> exitss = new ArrayList<>(Arrays.asList(exits).subList(0, length));

            transition.put(stateInput,exitss);
        }

        for (String s : entry) {
            String currentState = startState;
            List<String> stack = new ArrayList<>();
            stack.add(startStack);
            int failed = 0;

            System.out.printf("%s#%s|", currentState, startStack);

            String[] letterEntry = s.split(",");

            for (int j = 0; j < letterEntry.length; j++) {
                List<String> temp = new ArrayList<>();
                temp.add(currentState);
                temp.add(letterEntry[j]);
                temp.add(stack.get(stack.size() - 1));

                List<String> right = transition.get(temp);

                if (right != null) {
                    stack.remove(stack.size() - 1);
                    currentState = right.get(0);
                    customPrint(currentState, stack, right);
                } else {
                    List<String> temp2 = new ArrayList<>();
                    temp2.add(currentState);
                    temp2.add(String.valueOf(('$')));
                    temp2.add(stack.get(stack.size() - 1));

                    right = transition.get(temp2);

                    if (right != null) {
                        stack.remove(stack.size() - 1);
                        currentState = right.get(0);
                        customPrint(currentState, stack, right);
                        j--;
                    } else {
                        System.out.print("fail|0\n");
                        failed = 1;
                        break;
                    }
                }
            }

            if (failed == 0) {
                if (acceptableStatess.contains(currentState)) {
                    System.out.print("1\n");
                } else {
                    while (!acceptableStatess.contains(currentState)) {
                        List<String> temp = new ArrayList<>();
                        temp.add(currentState);
                        temp.add(String.valueOf('$'));
                        temp.add(stack.get(stack.size() - 1));

                        List<String> right;
                        right = transition.get(temp);
                        if (right != null) {
                            stack.remove(stack.size() - 1);
                            currentState = right.get(0);
                            customPrint(currentState, stack, right);
                        } else {
                            break;
                        }
                    }

                    if (acceptableStatess.contains(currentState)) {
                        System.out.print("1\n");
                    } else {
                        System.out.print("0\n");
                    }
                }
            }
        }
    }

    private static void customPrint(String currentState, List<String> stack, List<String> right) {
        String rightGet = right.get(1);
        if (!rightGet.equals("$") || stack.size() == 0) {
            for (int k = rightGet.length() - 1; k >= 0; k--) {
                stack.add(String.valueOf(rightGet.charAt(k)));
            }
        }
        if (stack.size() > 0) {
            System.out.printf("%s#", currentState);
            for (int z = stack.size() - 1; z >= 0; z--) {
                System.out.printf("%s", stack.get(z));
            }
            System.out.print("|");
        } else {
            System.out.printf("%s#$|", currentState);
        }
    }
}

