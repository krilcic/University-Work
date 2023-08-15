package hr.fer.labos.lab4;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Parser {

    private static String CURRENT;
    private static int INDEX;
    private static String ENTRY;
    private static boolean ACCEPTED;

    public static void main(String[] args) {
        List<String> lines = new ArrayList<>();

        Scanner scanner = new Scanner(System.in);
        String line;
        while (scanner.hasNextLine()) {
            line = scanner.nextLine();
            lines.add(line);
        }

        ACCEPTED = true;
        ENTRY = lines.get(0);
        INDEX = 0;
        CURRENT = String.valueOf(ENTRY.charAt(INDEX));

        s();

        if(!ACCEPTED) {
            System.out.print("\nNE");
        } else if(INDEX == ENTRY.length()) {
            System.out.print("\nDA");
        } else {
            System.out.print("\nNE");
        }
    }

    private static void s() {
        System.out.print("S");
        if(INDEX != ENTRY.length()) {
            CURRENT = String.valueOf(ENTRY.charAt(INDEX));
        } else {
            ACCEPTED = false;
            return;
        }

        if(CURRENT.equals("a")) {
            ++INDEX;

            a();

            if(!ACCEPTED) {
                return;
            }

            b();
        } else if(CURRENT.equals("b")) {
            ++INDEX;

            b();

            if(!ACCEPTED) {
                return;
            }
            a();
        } else{
            ACCEPTED = false;
        }
    }

    private static void a() {
        System.out.print("A");
        if(INDEX != ENTRY.length()) {
            CURRENT = String.valueOf(ENTRY.charAt(INDEX));
        } else {
            ACCEPTED = false;
            return;
        }

        if(CURRENT.equals("a")) {
            ++INDEX;
        } else if(CURRENT.equals("b")) {
            ++INDEX;
            c();
        } else{
            ACCEPTED = false;
        }
    }

    private static void b() {
        System.out.print("B");
        if(INDEX == ENTRY.length()) {
            return;
        }

        CURRENT = String.valueOf(ENTRY.charAt(INDEX));
        if(!CURRENT.equals("c")) {
            return;
        }

        ++INDEX;

        if(INDEX != ENTRY.length()) {
            CURRENT = String.valueOf(ENTRY.charAt(INDEX));

            if (CURRENT.equals("c")) {
                ++INDEX;
                s();

                if (!ACCEPTED) {
                    return;
                }
            } else {
                ACCEPTED = false;
                return;
            }

            if (INDEX != ENTRY.length()) {
                CURRENT = String.valueOf(ENTRY.charAt(INDEX));

                if (CURRENT.equals("b")) {
                    ++INDEX;
                } else {
                    ACCEPTED = false;
                    return;
                }
            } else {
                ACCEPTED = false;
                return;
            }

            if (INDEX != ENTRY.length()) {
                CURRENT = String.valueOf(ENTRY.charAt(INDEX));

                if (CURRENT.equals("c")) {
                    ++INDEX;
                } else {
                    ACCEPTED = false;
                }
            } else {
                ACCEPTED = false;
            }
        } else {
            ACCEPTED = false;
        }
    }

    private static void c() {
        System.out.print("C");
        a();

        if(!ACCEPTED){
            return;
        }

        a();
    }
}