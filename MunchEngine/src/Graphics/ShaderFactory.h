#pragma once

#include "Graphics/Shader.h"

struct ShaderFactory {
    static Shader* defaultShader();
    static Shader* dirShadowShader();
};