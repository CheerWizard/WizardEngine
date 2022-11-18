#include <WizardEngine.h>

class Template : public Scriptable {
public:
    void onCreate() override {
    }

    void onUpdate(Time dt) override {
    }

    void onDestroy() override {
    }

    void serialize(YAML::Emitter &out) override {

    }

    void deserialize(const YAML::Node &parent) override {

    }
};

INIT_SCRIPT(Template)