#pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include "GL/glew.h"

#include "shader/shader.h"
#include "shader/shaderBuf.h"

struct VO_BEZIER {
    GLuint pVAO, pVBO;
    GLuint cVAO, cVBO;
};

enum STATE {START, DOING, DONE, ALL_DONE};

class Bezier {
private:
    STATE state = START;
    GLsizei control_num = 0;
    GLsizei sample_num = 0;
    double total_length = 0;
    Vector2f *control_vertices = new Vector2f[5]{};
    Vector2f *curve_vertices = nullptr;

    static GLfloat getParam(int n, int x, double t);
    static void calcLastPoint(const Vector2f &src, const Vector2f &ctl, Vector2f &dst);
    void updateSamples();

public:
    Bezier() = default;
    ~Bezier();

    STATE getState() const {return state;}
    int getVertexNum() const {return control_num;}
    int getSampleNum() const {return sample_num + 1;}
    Vector2f getSample(int i) const {return curve_vertices[i];}
    Vector2f getVertex(int i) const {return control_vertices[i];}
    Vector2f getEndVertex() const {return control_vertices[control_num - 1];};

    void end() {state = DONE;}

    void adjustVertex(int n, const Vector2f &pos);
    void addVertex(const Vector2f &vert);

    void bindBuffer(const VO_BEZIER &vo) const;
};

class bezierManager {
public:
    using selected_points = std::vector<std::pair<int, int>>;

private:
    STATE state = START;
    int num = 0;

    //分割线
    GLuint VAO{};
    GLuint VBO{};
    GLfloat vs[4]{0.0f, -1000.0f, 0.0f, 1000.0f};

    std::vector<VO_BEZIER> vos{};
    std::vector<Bezier *> curves{};
    std::pair<int, int> select = {-1, -1};

    GLfloat aspect = 1.0f;
    Shader shader;

    void addCurve();

public:
    bezierManager();
    ~bezierManager();

    void endCurve();
    void end();

    STATE getState() {return state;}
    void upgrade(GLfloat asp) {aspect = asp;}

    void addVertex(const Vector2f &vert);
    selected_points *findVertex(const Vector2f &vert);
    void adjustVertex(const std::pair<int, int> &vert_id, const Vector2f &pos);

    void render();
    void to3dCurve(Vector3f *&curve, GLsizei &size);
};
