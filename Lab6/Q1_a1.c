#include <math.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    double a, b;
} pair;

void quadratic() {
    int a, b, c;
    printf("Enter the values of a, b, c: ");
    scanf("%d %d %d", &a, &b, &c);

    pair p;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        printf("x is Imaginary");
        return;
    }
    p.a = (-b + sqrt(discriminant)) / (2.0 * a);
    p.b = (-b - sqrt(discriminant)) / (2.0 * a);
    printf("x = %lf, %lf\n", p.a, p.b);
}

void exponent() {
    int x, n;
    printf("Enter the values of n, x: ");
    scanf("%d %d", &n , &x);

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
    pthread_t quadThread;
    pthread_t exThread;
    pthread_create(&quadThread, NULL, (void *)quadratic, (void *)0);
    pthread_join(quadThread, NULL);
    pthread_create(&exThread, NULL, (void *)exponent, (void *)0);
    pthread_join(exThread, NULL);

    // pair x = Quadratic(a, b, c);
    // printf("x = %lf, %lf\n", x.a, x.b);
    exponent();
    return 0;
}
