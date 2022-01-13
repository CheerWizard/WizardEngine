//
// Created by mecha on 13.01.2022.
//

#include "string"

class Test {

    public:
        Test() = default;
        ~Test() = default;

    public:
        const char* test_cstring() {
            return "Hello world!";
        }

        std::string test_string() {
            return "Hello world!";
        }
};