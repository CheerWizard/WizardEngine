//
// Created by mecha on 03.04.2022.
//

#include <ecs/ecs_test.h>

int find_recurse(int* arr, int x, int low, int high) {
    if (low > high)
        return -1;

    int mid = (low + high) / 2;

    if (x == arr[mid])
        return mid;
    else if (x > arr[mid])
        return find_recurse(arr, x, mid + 1, high);
    else
        return find_recurse(arr, x, low, mid - 1);
}

int find_loop(int* arr, int x, int low, int high) {
    while (low != high) {
        int mid = (low + high) / 2;
        if (x == arr[mid])
            return mid;
        else if (x > arr[mid])
            low = mid + 1;
        else
            high = mid - 1;
    }
}

int main() {
    INIT_ENGINE_LOG("Engine");
    INIT_RUNTIME_LOG("Test");

    RUNTIME_INFO("Running test suite...");
    test::ecs::test_suite();
    RUNTIME_INFO("Test suite finished!");

    int arr[5] = { 10, 12, 24, 35, 46 };
    int result1 = find_recurse(arr, 24, 0, 5);
    int result2 = find_loop(arr, 46, 0, 5);

    RUNTIME_INFO("Result1: {0}", result1);
    RUNTIME_INFO("Result2: {0}", result2);

    return 0;
}