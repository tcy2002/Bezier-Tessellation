#include "config.h"

GLfloat degToRad(GLfloat deg) {
    return PI * deg / 180.0f;
}

GLfloat radToDeg(GLfloat rad) {
    return rad / PI * 180.0f;
}

Vector2f operator+(const Vector2f &a, const Vector2f &b) {
    return {a.x + b.x, a.y + b.y};
}

Vector3f operator+(const Vector3f &a, const Vector3f &b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

void operator+=(Vector2f &dst, const Vector2f &src) {
    dst.x += src.x;
    dst.y += src.y;
}

void operator+=(Vector3f &dst, const Vector3f &src) {
    dst.x += src.x;
    dst.y += src.y;
    dst.z += src.z;
}

Vector2f operator-(const Vector2f &a, const Vector2f &b) {
    return {a.x - b.x, a.y - b.y};
}

Vector3f operator-(const Vector3f &a, const Vector3f &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

void operator-=(Vector2f &dst, const Vector2f &src) {
    dst.x -= src.x;
    dst.y -= src.y;
}

void operator-=(Vector3f &dst, const Vector3f &src) {
    dst.x -= src.x;
    dst.y -= src.y;
    dst.z -= src.z;
}

Vector2f operator*(const Vector2f &a, GLfloat b) {
    return {a.x * b, a.y * b};
}

Vector3f operator*(const Vector3f &a, GLfloat b) {
    return {a.x * b, a.y * b, a.z * b};
}

void operator*=(Vector2f &dst, GLfloat src) {
    dst.x *= src;
    dst.y *= src;
}

void operator*=(Vector3f &dst, GLfloat src) {
    dst.x *= src;
    dst.y *= src;
    dst.z *= src;
}

bool operator==(const Vector2f &a, const Vector2f b) {
    return (a.x == b.x && a.y == b.y);
}

double operator*(const Vector3f &a, const Vector3f &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3f operator%(const Vector3f &src, const Vector3f &dst) {
    return {src.y * dst.z - src.z * dst.y,
            src.z * dst.x - src.x * dst.z,
            src.x * dst.y - src.y * dst.x};
}

Vector2f operator&(const Vector2f &src, GLfloat r) {
    float len = std::sqrt(src.x * src.x + src.y * src.y);
    return src * (1.0f / len) * r;
}

Vector3f operator&(const Vector3f &src, GLfloat r) {
    float len = std::sqrt(src.x * src.x + src.y * src.y + src.z * src.z);
    return src * (1.0f / len) * r;
}

void operator&=(Vector2f &src, GLfloat r) {
    float len = std::sqrt(src.x * src.x + src.y * src.y);
    src *= (1.0f / len * r);
}

void operator&=(Vector3f &src, GLfloat r) {
    float len = std::sqrt(src.x * src.x + src.y * src.y + src.z * src.z);
    src *= (1.0f / len * r);
}

Vector2f operator|(const Vector2f &src, const Vector2f &ctl) {
    return {2.0f * ctl.x - src.x, 2.0f * ctl.y - src.y};
}

Vector3f operator|(const Vector3f &src, const Vector3f &ctl) {
    return {2.0f * ctl.x - src.x,
            2.0f * ctl.y - src.y,
            2.0f * ctl.z - src.z};
}

void operator|=(Vector2f &src, const Vector2f &ctl) {
    src.x = 2.0f * ctl.x - src.x;
    src.y = 2.0f * ctl.y - src.y;
}

void operator|=(Vector3f &src, const Vector3f &ctl) {
    src.x = 2.0f * ctl.x - src.x;
    src.y = 2.0f * ctl.y - src.y;
    src.z = 2.0f * ctl.z - src.z;
}

double operator/(const Vector2f &src, const Vector2f &dst) {
    double x2 = std::pow(dst.x - src.x, 2.0);
    double y2 = std::pow(dst.y - src.y, 2.0);
    return std::sqrt(x2 + y2);
}

double operator/(const Vector3f &src, const Vector3f &dst) {
    double x2 = std::pow(dst.x - src.x, 2.0);
    double y2 = std::pow(dst.y - src.y, 2.0);
    double z2 = std::pow(dst.z - src.z, 2.0);
    return std::sqrt(x2 + y2 + z2);
}

Matrix4f operator*(Matrix4f &a, Matrix4f &b) {
    Matrix4f ret{};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ret[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] + a[i][3] * b[3][j];
    return ret;
}

std::ostream &operator<<(std::ostream &out, Matrix4f &mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            out << mat[i][j] << " ";
        out << std::endl;
    }
    return out;
}
