#pragma once

#include <cmath>
#include <cstring>
#include "GL/glew.h"

#include "config/config.h"

enum Direction {X, Y, Z};

struct ProjInfo {
    GLfloat FOV;
    GLfloat aspect;
    GLfloat zNear;
    GLfloat zFar;
};

class View {
private:
    Vector3f m_scaleInfo{};
    Vector3f m_transInfo{};
    Vector3f m_rotInfo{};
    ProjInfo m_projInfo{};

    Matrix4f scale_mat{};
    Matrix4f world_mat{};
    Matrix4f rot_mat{};
    Matrix4f proj_mat{};
    Matrix4f trans_mat{};

    void getWorldMat(Matrix4f &mat);
    void getScaleMat(Matrix4f &mat) const;
    void getRotMat(Matrix4f &mat) const;
    void getTransMat(Matrix4f &mat) const;
    void getProjMat(Matrix4f &mat) const;

    static void loadIdentity(Matrix4f &mat);
    static void loadRotateMat(Matrix4f &mat, GLfloat angle, Direction d);

public:
    View();
    ~View() = default;

    void setScale(Vector3f scale);
    void setTransInfo(Vector3f pos);
    void setRotInfo(Vector3f rot);
    void setProjInfo(ProjInfo proj);
    void updateProjInfo(GLfloat aspect);

    Matrix4f getProjMat() {return proj_mat;}
    Matrix4f getWorldMat() {return world_mat;}
    Matrix4f getRotMat() {return rot_mat;}
};
