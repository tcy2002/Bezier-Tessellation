#pragma once

#include <cstring>
#include <cmath>
#include <vector>
#include "GL/glew.h"
#include <fstream>

#include "view/view.h"
#include "texture/texture.h"
#include "shader/shader.h"
#include "shader/shaderBuf.h"
#include "sourcesLoader/sourcesLoader.h"

struct VO_MODEL {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

class Model {
protected:
    Vertex *vertices;
    GLuint *indices;

    int index_size;
    int sample_size;

public:
    Model();
    ~Model();

    void bindBuffer(const VO_MODEL &vo);

    void download(const char *path);
    void upload(const char *path);

    GLsizei getIndexSize() const {return index_size;}
    GLsizei getSampleSize() const {return sample_size;}
};

class RotationModel : public Model {
private:
    int sample_num;
    int loop_num;

public:
    RotationModel(const Vector3f *curve, GLsizei size);
};

class ModelManager {
private:
    int num = 0;
    std::vector<Texture *> texes{};
    std::vector<Model *> models{};
    std::vector<VO_MODEL> vos{};

    Shader shader;
    View view;
    bool mode = false;
    int tex_id = -1;

    GLfloat dist = -4.0f;
    GLfloat rot_ver = 0.0f;
    GLfloat rot_hor = 0.0f;

public:
    ModelManager();
    ~ModelManager();

    void loadTexture();
    void switchTexture();

    void upgrade(GLfloat aspect) {view.updateProjInfo(aspect);}
    void changeMode() {mode = !mode;}

    void increase();
    void decrease();
    void rotateVertical(int a);
    void rotateHorizontal(int a);
    void rotate();

    void addRotationModel(Vector3f *curve, GLsizei size);
    bool loadRotationModel();
    bool downloadLastModel();

    void render();
};
