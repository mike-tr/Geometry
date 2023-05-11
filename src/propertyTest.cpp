#include "property.hpp"
#include <iostream>

using namespace std;

class AB {
public:
    int x;
    int y;
    AB(int x, int y) {
        this->x = x;
        this->y = y;
    }
    AB operator+(const AB other) const {
        // base = this->value + other;
        return AB(this->x + other.x, this->y + other.y);
    }
};

class BB {
private:
    AB _pos = AB(0, 0);

public:
    Property<BB, AB> pos = Property(*this, _pos, callback);
    // Property<BB, glm::vec3> pos = Property(*this, glm::vec3(0.0f, 0.0f, 0.0f), callback);
    void callback() {
        // cout << "x : " << ((glm::vec3)pos).x << "| y : " << ((glm::vec3)pos).y << endl;
        cout << "x : " << ((AB)pos).x << "| y : " << ((AB)pos).y << endl;
    }
    BB() {}
};

void ptest() {
    BB test;
    test.pos = AB(3, 3);

    AB ab = AB(1, 1);

    test.pos += AB(1, 2);
}