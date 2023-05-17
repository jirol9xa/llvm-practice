void foo() {
    volatile int a;
    a = 10;
    return;
}

int main() {
    for (int i = 0; i < 10; ++i)
        foo();

    return 0;
}
