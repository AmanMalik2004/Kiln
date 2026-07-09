print("Hello World");
print("MiniC Compiler");

int sum = 0;
for (int i = 0; i < 5; i = i + 1) {
    sum = sum + i;
}
print(sum);

int factorial(int n) {
    if (n < 2) {
        return 1;
    }
    int prev = factorial(n - 1);
    int result = n * prev;
    return result;
}

int f = factorial(5);
print(f);