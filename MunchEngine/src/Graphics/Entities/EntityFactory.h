#pragma once

#include "Graphics/Entities/Default/DefaultEntity.h"

struct EntityFactory {
    static DefaultEntity* sphere();
    static DefaultEntity* plane();
    static DefaultEntity* terrain();
};