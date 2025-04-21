#include <math.h>
#include <stdio.h>
#include <pthread.h>

typedef struct quadArg {
    int a, b, c;
} quadArg;

typedef struct exArg {
    int x, n;
} exArg;

void quadratic(quadArg arg1) {
    int a = arg1.a;
    int b = arg1.b;
    int c = arg1.c;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        printf("x is Imaginary");
        return;
    }
    double x1 = (-b + sqrt(discriminant)) / (2.0 * a);
    double x2 = (-b - sqrt(discriminant)) / (2.0 * a);
    printf("x = %lf, %lf\n", x1, x2);
}

void exponent(exArg arg2) {
    int x = arg2.x;
    int n = arg2.n;

    int lastX = 1;
    int lastFact = 1;
    double ex = 0;
    for (int i = 1; i <= n; i++) {
        ex += (lastX / lastFact);
        lastX *= x;
        lastFact *= i;
    }
    printf("e^x = %lf", ex);
}

int main() {

    quadArg arg1;
    exArg arg2;

    printf("Enter the values of a, b, c: ");
    scanf("%d %d %d", &arg1.a, &arg1.b, &arg1.c);

    int x, n;
    printf("Enter the values of n, x for e^x: ");
    scanf("%d %d", &arg2.n, &arg2.x);

    pthread_t quadThread, exThread;
    pthread_create(&quadThread, NULL, (void *)quadratic, &arg1);
    pthread_join(quadThread, NULL);
    pthread_create(&exThread, NULL, (void *)exponent, &arg2);
    pthread_join(exThread, NULL);
    
    return 0;
}
