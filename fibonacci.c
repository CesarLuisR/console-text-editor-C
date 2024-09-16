#include <stdio.h>

void fibonacci(int before, int actual, int counter) {
    int new = actual + before;
    counter++;
    
    printf("%d\n", actual);
    if (counter == 10) return;
    
    fibonacci(actual, new, counter);
}

void fibo() {
    int a = 0;
    int b = 1;
    int counter = 0;
    
    while (1) {
        int c = b;
        b = a + b;
        a = c;
        printf("%d\n", b);
        counter++;
        
        if (counter == 10) break;
    }
}

int main() {
    fibonacci(0, 1, 0);
    fibo();
    
    return 0;
}