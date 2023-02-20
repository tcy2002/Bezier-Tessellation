#include "view.h"

View::View() {
    m_scaleInfo = {1.0f, 1.0f, 1.0f};
    m_transInfo = {0.0f, 0.0f, 0.0f};
    m_rotInfo = {0.0f, 0.0f, 0.0f};
    m_projInfo = {45.0f, 1.0f, 1.0f, 100.0f};
    getScaleMat(scale_mat);
    getTransMat(trans_mat);
    getRotMat(rot_mat);
    getProjMat(proj_mat);
    getWorldMat(world_mat);
}

void View::getWorldMat(Matrix4f &mat) {
    mat = rot_mat * scale_mat;
    mat = trans_mat * mat;
}

void View::getScaleMat(Matrix4f &mat) const {
    loadIdentity(mat);
    mat[0][0] = m_scaleInfo.x;
    mat[1][1] = m_scaleInfo.y;
    mat[2][2] = m_scaleInfo.z;
    mat[3][3] = 1.0f;
}

void View::getRotMat(Matrix4f &mat) const {
    Matrix4f rx{}, ry{}, rz{};

    GLfloat x = degToRad(m_rotInfo.x);
    GLfloat y = degToRad(m_rotInfo.y);
    GLfloat z = degToRad(m_rotInfo.z);

    loadRotateMat(rx, x, X);
    loadRotateMat(ry, y, Y);
    loadRotateMat(rz, z, Z);

    mat = rx * ry;
    mat = mat * rz;
}

void View::getTransMat(Matrix4f &mat) const {
    loadIdentity(mat);
    mat[0][3] = m_transInfo.x;
    mat[1][3] = m_transInfo.y;
    mat[2][3] = m_transInfo.z;
}

void View::getProjMat(Matrix4f &mat) const {
    float zRange = m_projInfo.zNear - m_projInfo.zFar;
    float tanHalfFOV = tanf(degToRad(m_projInfo.FOV / 2.0f));

    memset(&mat, 0, sizeof(Matrix4f));
    mat[0][0] = 1.0f / (tanHalfFOV * m_projInfo.aspect);
    mat[1][1] = 1.0f / tanHalfFOV;
    mat[2][2] = (m_projInfo.zNear + m_projInfo.zFar) / zRange;
    mat[3][2] = -1.0f;
    mat[2][3] = 2.0f * m_projInfo.zFar * m_projInfo.zNear / zRange;
}

void View::loadIdentity(Matrix4f &mat) {
    memset(&mat, 0, sizeof(Matrix4f));
    mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1.0f;
}

void View::loadRotateMat(Matrix4f &mat, GLfloat angle, Direction d) {
    loadIdentity(mat);
    auto sin_ang = (GLfloat)sin(angle);
    auto cos_ang = (GLfloat)cos(angle);
    switch (d) {
        case X:
            mat[1][1] = cos_ang;
            mat[1][2] = -sin_ang;
            mat[2][1] = sin_ang;
            mat[2][2] = cos_ang;
            break;
        case Y:
            mat[0][0] = cos_ang;
            mat[2][0] = -sin_ang;
            mat[0][2] = sin_ang;
            mat[2][2] = cos_ang;
            break;
        case Z:
            mat[0][0] = cos_ang;
            mat[0][1] = -sin_ang;
            mat[1][0] = sin_ang;
            mat[1][1] = cos_ang;
            break;
    }
}

void View::setScale(const Vector3f scale) {
    m_scaleInfo = scale;
    getScaleMat(scale_mat);
    getWorldMat(world_mat);
}

void View::setTransInfo(Vector3f pos) {
    m_transInfo = pos;
    getTransMat(trans_mat);
    getWorldMat(world_mat);
}

void View::setRotInfo(Vector3f rot) {
    m_rotInfo = rot;
    getRotMat(rot_mat);
    getWorldMat(world_mat);
}

void View::setProjInfo(ProjInfo proj) {
    m_projInfo = proj;
    getProjMat(proj_mat);
}

void View::updateProjInfo(GLfloat aspect) {
    m_projInfo.aspect = aspect;
    getProjMat(proj_mat);
}
