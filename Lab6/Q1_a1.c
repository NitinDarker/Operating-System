#include <math.h>
#include <pthread.h>
#include <stdio.h>

typedef struct quadArg {
    int a, b, c;
} quadArg;

typedef struct exArg {
    int x, n;
} exArg;

void *quadratic(void *arg) {
    quadArg *arg1 = (quadArg *)arg;
    int a = arg1->a;
    int b = arg1->b;
    int c = arg1->c;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        printf("The roots are Imaginary\n");
        pthread_exit(NULL);
    }
    double x1 = (-b + sqrt(discriminant)) / (2.0 * a);
    double x2 = (-b - sqrt(discriminant)) / (2.0 * a);
    printf("x = %f, %f\n", x1, x2);
    pthread_exit(NULL);
}

void *exponent(void *arg) {
    exArg *arg2 = (exArg *)arg;
    int x = arg2->x;
    int n = arg2->n;

    double ex = 1.0;   // start with x^0 / 0! = 1
    double term = 1.0; // 0th term

    for (int i = 1; i <= n; i++) {
        term *= (double)x / i;
        ex += term;
    }

    printf("e^%d = %f\n", x, ex);
    pthread_exit(NULL);
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
    pthread_create(&quadThread, NULL, quadratic, &arg1);
    pthread_create(&exThread, NULL, exponent, &arg2);
    pthread_join(quadThread, NULL);
    pthread_join(exThread, NULL);

    return 0;
}
