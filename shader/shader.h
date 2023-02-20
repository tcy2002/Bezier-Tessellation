#pragma once

#include <iostream>
#include "GL/glew.h"

#include "config/config.h"

class Shader {
private:
    int program_id;

    static int createShader(const char *buf, int type);
    static void compileShaderAndCheck(int shader);
    static void linkProgramAndCheck(int program);

public:
    Shader(const char *vertPath, const char *fragPath, const char *tcsPath = nullptr, const char *tesPath = nullptr, const char *gsPath = nullptr);
    ~Shader();

    void use() const;

    void setBool(const char *name, bool value) const;
    void setInt(const char *name, int value) const;
    void setFloat(const char *name, float value) const;
    void setVec2(const char *name, Vector2f value) const;
    void setVec3(const char *name, Vector3f value) const;
    void setVec4(const char *name, Vector4f value) const;
    void setMat4(const char *name, Matrix4f value) const;
};