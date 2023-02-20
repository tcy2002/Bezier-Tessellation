#include "shader.h"

Shader::Shader(const char *vertPath, const char *fragPath, const char *tcsPath, const char *tesPath, const char *gsPath) {
    program_id = glCreateProgram();

    int vertID = createShader(vertPath, GL_VERTEX_SHADER);
    glAttachShader(program_id, vertID);
    glDeleteShader(vertID);

    int fragID = createShader(fragPath, GL_FRAGMENT_SHADER);
    glAttachShader(program_id, fragID);
    glDeleteShader(fragID);

    if (tcsPath != nullptr) {
        int tcsID = createShader(tcsPath, GL_TESS_CONTROL_SHADER);
        glAttachShader(program_id, tcsID);
        glDeleteShader(tcsID);
    }

    if (tesPath != nullptr) {
        int tesID = createShader(tesPath, GL_TESS_EVALUATION_SHADER);
        glAttachShader(program_id, tesID);
        glDeleteShader(tesID);
    }

    if (gsPath != nullptr) {
        int gsID = createShader(gsPath, GL_GEOMETRY_SHADER);
        glAttachShader(program_id, gsID);
        glDeleteShader(gsID);
    }

    linkProgramAndCheck(program_id);
}

Shader::~Shader() {
    glDeleteProgram(program_id);
}

int Shader::createShader(const char *buf, int type) {
    int shader = glCreateShader(type);
    glShaderSource(shader, 1, &buf, nullptr);
    compileShaderAndCheck(shader);
    return shader;
}

void Shader::compileShaderAndCheck(int shader) {
    int success;
    char infoLog[1024];
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cout << "Shader Compile Error: \n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Shader::linkProgramAndCheck(int program) {
    int success;
    char infoLog[1024];
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cout << "Program Compile Error: \n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Shader::use() const {
    glUseProgram(program_id);
}

void Shader::setBool(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(program_id, name), (int)value);
}

void Shader::setInt(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(program_id, name), value);
}

void Shader::setFloat(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(program_id, name), value);
}

void Shader::setVec2(const char *name, Vector2f value) const {
    glUniform2f(glGetUniformLocation(program_id, name), value.x, value.y);
}

void Shader::setVec3(const char *name, Vector3f value) const {
    glUniform3f(glGetUniformLocation(program_id, name), value.x, value.y, value.z);
}

void Shader::setVec4(const char *name, Vector4f value) const {
    glUniform4f(glGetUniformLocation(program_id, name), value.x, value.y, value.z, value.a);
}

void Shader::setMat4(const char *name, Matrix4f value) const {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name), 1, GL_FALSE, value[0]);
}
