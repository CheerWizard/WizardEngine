#version 400 core

#include include/v_scene_attributes.glsl
#include include/v_scene.glsl

void main() {
    updateObject(gl_InstanceID);
}