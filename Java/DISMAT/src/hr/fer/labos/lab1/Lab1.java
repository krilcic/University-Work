package hr.fer.labos.lab1;

import java.util.Scanner;

public class Lab1 {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Input a non-negative number: ");
        int n = scanner.nextInt();
        System.out.print("Input number b_0: ");
        double b0 = scanner.nextDouble();
        System.out.print("Input number b_1: ");
        double b1 = scanner.nextDouble();
        System.out.print("Input number b_2: ");
        double b2 = scanner.nextDouble();
        System.out.print("Input number c_0: ");
        double c0 = scanner.nextDouble();
        System.out.print("Input number c_1: ");
        double c1 = scanner.nextDouble();
        System.out.print("Input number c_2: ");
        double c2 = scanner.nextDouble();

        double bn = 0;
        if (n == 0) {
            System.out.printf("Value of number b_n: %f", b0);
        } else if (n < 0){
            System.out.println("Incorrect value of n");
        } else if (n == 1) {
            System.out.printf("Value of number b_n: %f", b1);
        } else if (n == 2) {
            System.out.printf("Value of number b_n: %f", b2);
        } else {
            double lambda1 = (c0 * b2 - b0 * c2) / (b1 * c0 - b0 * c1);
            double lambda2 = (b1 * c2 - c1 * b2) / (b1 * c0 - b0 * c1);

            double first = b2;
            double second = b1;

            for (int i = 0; i < n - 2; i++) {
                bn = lambda1 * first + lambda2 * second;
                second = first;
                first = bn;
            }

            System.out.printf("Value of number b_n: %f", bn);
        }
    }
}
