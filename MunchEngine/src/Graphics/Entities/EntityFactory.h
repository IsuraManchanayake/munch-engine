#pragma once

#include "Graphics/Entities/Default/DefaultEntity.h"
#include "Graphics/Entities/Terrain/TerrainEntity.h"

struct EntityFactory {
    static DefaultEntity* sphere();
    static DefaultEntity* plane();
    static TerrainEntity* terrain();
    static TerrainEntity* terrain(const std::string& heightMapPath);
};