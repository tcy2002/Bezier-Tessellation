/********************************************
 * 此文件定义了一些需要用到的常数、类型与工具函数
 *******************************************/

#pragma once

#include <iostream>
#include <cmath>
#include "GL/glew.h"

#define PI 3.141593f

#define ORI_WIDTH 600
#define ORI_HEIGHT 600

#define CURVE_GRANULARITY 0.02f
#define SAMPLING_RATE 0.25f

typedef struct vector2f {
    GLfloat x, y;
} Vector2f;
typedef struct vector3f {
    GLfloat x, y, z;
} Vector3f;
typedef struct vector4f {
    GLfloat x, y, z, a;
} Vector4f;

struct Matrix4f {
    GLfloat mat[16];
    GLfloat *operator[](int r) {
        return &mat[r * 4];
    }
};

typedef struct vertex {
    Vector3f pos;
    Vector3f nor;
    Vector2f tex;
} Vertex;

GLfloat degToRad(GLfloat deg);
GLfloat radToDeg(GLfloat rad);

Vector2f operator+(const Vector2f &a, const Vector2f &b);
Vector3f operator+(const Vector3f &a, const Vector3f &b);
void operator+=(Vector2f &dst, const Vector2f &src);
void operator+=(Vector3f &dst, const Vector3f &src);

Vector2f operator-(const Vector2f &a, const Vector2f &b);
Vector3f operator-(const Vector3f &a, const Vector3f &b);

void operator-=(Vector2f &dst, const Vector2f &src);
void operator-=(Vector3f &dst, const Vector3f &src);

Vector2f operator*(const Vector2f &a, GLfloat b);
Vector3f operator*(const Vector3f &a, GLfloat b);

void operator*=(Vector2f &dst, GLfloat src);
void operator*=(Vector3f &dst, GLfloat src);

bool operator==(const Vector2f &a, Vector2f b);

//点积
double operator*(const Vector3f &a, const Vector3f &b);
//叉积
Vector3f operator%(const Vector3f &src, const Vector3f &dst);

//计算标准化向量：左侧源点，右侧相对于归一化向量的长度比例
Vector2f operator&(const Vector2f &src, GLfloat r);
//计算标准化向量：左侧源点，右侧相对于归一化向量的长度比例
Vector3f operator&(const Vector3f &src, GLfloat r);

//计算标准化向量，给出相对于归一化向量的长度比例
void operator&=(Vector2f &src, GLfloat r);
//计算标准化向量，给出相对于归一化向量的长度比例
void operator&=(Vector3f &src, GLfloat r);

//计算对称点：左侧源点，右侧对称中心点
Vector2f operator|(const Vector2f &src, const Vector2f &ctl);
//计算对称点：左侧源点，右侧对称中心点
Vector3f operator|(const Vector3f &src, const Vector3f &ctl);

//计算对称点：给出对称中心点
void operator|=(Vector2f &src, const Vector2f &ctl);
//计算对称点：给出对称中心点
void operator|=(Vector3f &src, const Vector3f &ctl);

//计算距离：左侧源点，右侧终点
double operator/(const Vector2f &src, const Vector2f &dst);
//计算距离：左侧源点，右侧终点
double operator/(const Vector3f &src, const Vector3f &dst);

//四维矩阵乘法
Matrix4f operator*(Matrix4f &a, Matrix4f &b);

//四维矩阵输出
std::ostream &operator<<(std::ostream &out, Matrix4f &mat);
