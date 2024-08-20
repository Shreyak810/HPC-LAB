#include <stdio.h>
#include <omp.h>

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }

    int x, y;
    #pragma omp task shared(x)
    x = fibonacci(n - 1);

    #pragma omp task shared(y)
    y = fibonacci(n - 2);

    #pragma omp taskwait
    return x + y;
}

int main() {
    int n;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    int fib;
    #pragma omp parallel
    {
        #pragma omp single
        fib = fibonacci(n);
    }

    printf("Fibonacci(%d) = %d\n", n, fib);
    return 0;
}
