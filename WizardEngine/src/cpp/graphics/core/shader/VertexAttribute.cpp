#include <graphics/core/shader/VertexAttribute.h>

#include <glad/glad.h>

namespace engine::shader {

    u8 AttributeCategory::VERTEX = 0;
    u8 AttributeCategory::INSTANCE = 1;
    u8 AttributeBool::FALSE = GL_FALSE;
    u8 AttributeBool::TRUE = GL_TRUE;

}