#include "Graphics/Screenshot.h"

#include "Physics/Physics.h"
#include "Core/Common.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Graphics/stb_image_write.h"

#include <GL/glew.h>
#include <regex>

void Screenshot::capturepng(std::string format, unsigned width, unsigned height) {
    format = std::regex_replace(format, std::regex(R"(\$frame)"), cat(Physics::frames));
    format = std::regex_replace(format, std::regex(R"(\$time)"), std::regex_replace(currentTime(), std::regex(R"(:)"), "_"));
    format = std::string("Screenshots/") + format;

    unsigned char* pixels = new unsigned char[3 * width * height];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    stbi_flip_vertically_on_write(true);
    stbi_write_png(format.c_str(), width, height, 3, pixels, width * 3);
    delete [] pixels;
}