#pragma once

#include "GL/glew.h"

#include "sourcesLoader/sourcesLoader.h"

class Texture {
private:
    GLuint texture_id{};

public:
    Texture() = default;
    explicit Texture(const char *buf);
    ~Texture();

    void bind(int texture) const;
};