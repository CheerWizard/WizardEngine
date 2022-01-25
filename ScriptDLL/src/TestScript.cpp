//
// Created by mecha on 13.01.2022.
//

#include <scripting/Scriptable.h>

class DLL_API Test : public Scriptable {

public:
    Test() = default;
    ~Test() override = default;

public:
    void onCreate() override {
        RUNTIME_INFO("Test: onCreate()");
        set<TagComponent>("Test");
//        add<Transform3dComponent>(transform3d());
//        set<MeshComponent>(GET_MESH("human.obj"));
    }

    void onDestroy() override {
        RUNTIME_INFO("Test: onDestroy()");
    }

    void onUpdate(Time dt) override {
        auto tag = get<TagComponent>().tag;
//        auto transform = get<Transform3dComponent>();
//        auto* mesh = entity.getPtr<MeshComponent>();
        RUNTIME_INFO("Test: onUpdate({0}ms)", dt.getMilliseconds());
        RUNTIME_INFO("Test: entity={0}", tag);
//        RUNTIME_INFO("Test: meshCount={0}", meshCount);
    }

};

DLL_EXPORT void* create() {
    return new Test();
}