#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define N 100

volatile int n = 0;
volatile unsigned long long k = 0;

void mask() {
    sigset_t x;
    sigemptyset(&x);
    sigaddset(&x, SIGALRM);
    sigprocmask(SIG_BLOCK, &x, NULL);
}

void unmask() {
    sigset_t x;
    sigemptyset(&x);
    sigaddset(&x, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &x, NULL);
}

void hello(int signo) {
    if(!(n % 1000))
        printf("Hello\n");
    n++;
    k--;
}

int main(int argc, char** argv) {
    struct sigaction sa;

    sa.sa_handler = hello;
    sa.sa_flags = 0;

    sigaction(SIGALRM, &sa, 0);
    ualarm(1000, 1000);

    unsigned long long ref = 0;

    while(n < N) {
        ref++;
        mask();
        k++;
        unmask();
    }

    printf("diff: %lld %d\n", ref - k, N);

    return EXIT_SUCCESS;
}