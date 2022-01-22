//
// Created by mecha on 14.01.2022.
//

#include "Scriptable.h"

void Scriptable::onCreate() {
    RUNTIME_INFO("onCreate()");
}

void Scriptable::onUpdate(Time dt) {
    RUNTIME_INFO("onUpdate({0} ms)", dt.getMilliseconds());
}

void Scriptable::onDestroy() {
    RUNTIME_INFO("onDestroy()");
}