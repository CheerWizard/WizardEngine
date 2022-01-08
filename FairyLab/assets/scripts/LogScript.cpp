#include "WizardEngine.h"

class LogScript : public Entity {

public:
    void onCreate() {
        EDITOR_INFO("LogScript: onCreate()");
    }

    void onDestroy() {
        EDITOR_INFO("LogScript: onDestroy()");
    }

    void onUpdate(Time dt) {
        auto tag = get<TagComponent>().tag;
        auto meshCount = get<MeshComponent>().meshCount;
        EDITOR_INFO("LogScript: onUpdate({0}ms)", dt.getMilliseconds());
        EDITOR_INFO("LogScript: entity={0}", tag);
        EDITOR_INFO("LogScript: meshCount={0}", meshCount);
    }

};