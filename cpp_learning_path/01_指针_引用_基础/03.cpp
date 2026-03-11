#include <iostream>

void foo() {
    int x = 0;
    x++;
    std::cout << x << std::endl;
    //* 函数结束，x被销毁
}

void foo_static() {
    static int x = 0;
    x++;
    std::cout << x << std:: endl;
}

class Base {
public:
    Base() {std::cout << "Base 构造\n";}
    virtual ~Base() {std::cout << "Base 析构\n";}
};

class Derived : public Base {
public:
    int* data;
    Derived() {
        data = new int[100];
        std::cout << "Derived 构造\n";
    }
    ~Derived() {
        delete [] data;
        std::cout << "Derived 析构\n";
    }
};

void test_virtual_destructor() {
    Base* p = new Derived;
    delete p;
}
int main() {
    foo();
    foo();
    foo();
    foo_static();
    foo_static();
    foo_static();

    test_virtual_destructor();
}



