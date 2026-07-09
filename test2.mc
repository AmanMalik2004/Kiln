int factorial(int n) {
    if (n < 2) {
        return 1;
    }
    int prev = factorial(n - 1);
    int result = n * prev;
    return result;
}

int x = factorial(5);
print(x);

int y = factorial(3);
print(y);


int add(int a, int b) {
    int result = a + b;
    return result;
}

int multiply(int a, int b) {
    int result = a * b;
    return result;
}

int x = add(5, 3);
print(x);

int y = multiply(4, 6);
print(y);

int z = add(x, y);
print(z);