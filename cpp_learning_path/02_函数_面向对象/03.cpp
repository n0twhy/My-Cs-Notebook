#include <iostream>
using namespace std;
class Base {
public:    
    virtual void Draw() {
        cout << "Base" << endl;
    }
};

class other : public Base{
public:
    void Draw () override {
        cout << "other" << endl;
    }
};

class Square : public Base {
public:
    void Draw() override {
        cout << "Square" << endl;
    }
};

void renderShape(Base* base) {
    base->Draw();
}

int main() {
    Base* base1 = new other;
    Base* base2 = new Square;

    renderShape(base1);
    renderShape(base2);

    delete base1;
    delete base2;
}