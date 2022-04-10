//
// Created by mecha on 22.10.2021.
//

#pragma once

#include <core/String.h>

#define OBJ_EXT ".obj"
#define PNG_EXT ".png"
#define JPG_EXT ".jpg"
#define GLSL_EXT ".glsl"
#define TTF_EXT ".ttf"
#define CPP_EXT ".cpp"
#define IS_TEXTURE(x) SAME(x, JPG_EXT) || SAME(x, PNG_EXT)
#define IS_SCRIPT(x) SAME(x, CPP_EXT) || SAME(x, GLSL_EXT)
#define IS_MODEL(x) SAME(x, OBJ_EXT)
#define IS_SHADER(x) SAME(x, GLSL_EXT)