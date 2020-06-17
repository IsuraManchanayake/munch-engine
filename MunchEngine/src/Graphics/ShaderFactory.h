#pragma once

#include "Graphics/Shader.h"

#pragma message("TODO: Make all shaders here and make the Shader constructor private")
struct ShaderFactory {
    static Shader* defaultShader();
    static Shader* dirShadowShader();
    static Shader* omniDirShadowShader();
    static Shader* pbrShader();
    static Shader* terrainShader();
    static Shader* hdriShader();
    static Shader* irradianceShader();
private:
    ShaderFactory();
};