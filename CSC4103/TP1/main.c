#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <process.h>


bool is_prime(int number) {
    if (number == 2 || number == 3)
        return true;
    if (number <= 1 || number % 2 == 0 || number % 3 == 0)
        return false;

    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0)
            return false;
    }
    return true;
}

long facto(int n) {
    if (n == 1 || n == 0) {
        return 1;
    }
    return n * facto(n-1);
}

double power(double x, int n) {
    return pow(x, n);
}

double expon(double x, int n) {
    double exp = 1;
    for (int i = 1; i <= n; i++) {
        exp += power(x, i) / facto(i);
    }
    return exp;
}

void int_to_bits(int n) {
    unsigned mask = 1 << 31;

    for (int i=0; i <= 32; i++) {
        unsigned tmp = mask & n;
        if (tmp != 0) {
            printf("1");
        } else
            printf("0");

        mask = mask >> 1;
    }
}

int main() {
    int v1 = 0;
    printf("Hello, World!\n Let's enter a value : ");
    scanf_s("%d", &v1);
    printf("Our value is : %d, let's check if it's prime ! \n", v1);
    if (is_prime(v1))
        printf("It's a Prime !");
    else
        printf("Nope you're not prime");

    printf("let's play a game !");

    srand(_getpid());
    int value_to_find = rand() % 100;

    int user_value = 0;
    int count = 0;
    printf("Try to guess the value !\n");
    while (user_value != value_to_find && count >= 1) {
        count--;
        printf("You have %d, remaining test \n", count);
        scanf_s("%d", &user_value);
        printf("\n");
    }
    if (user_value == value_to_find) {
        printf("You win ! \n");
    }
    else
        printf("You loose... \n");

    printf("Let's compute the exponential of : ");
    double user_x = 0;
    scanf_s("%lf", &user_x);
    printf("On obtient alors : %lf", expon(user_x, 20));

    printf("Now let's see how to represent value in binary \n");
    printf("What's your number ? ");
    int user_nb = 0;
    scanf_s("%d", &user_nb);
    int_to_bits(user_nb);

    return 0;
}
