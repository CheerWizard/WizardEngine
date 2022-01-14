//
// Created by mecha on 13.01.2022.
//

#include "../../../WizardEngine/src/Parent.h"

class Test : public Parent {
public:
    Test() = default;
    ~Test() override = default;

    void print(const char* msg) override {
    }
};

extern "C" __declspec(dllexport) Parent* create_object() {
    return new Test();
}