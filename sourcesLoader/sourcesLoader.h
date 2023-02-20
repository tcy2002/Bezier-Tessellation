#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "GL/glew.h"

std::string openFileDialog(const char *filter, bool exist=false);
std::string openObjFile();
std::string writeObjFile();
std::string openBmpFile();

class FileLoader {
private:
    int size;

public:
    char *buf;

    explicit FileLoader(const char *name);
    ~FileLoader() {delete[] buf;}
};

class BmpLoader {
private:
    int channel;
    BITMAPFILEHEADER bfh{};
    BITMAPINFOHEADER bih{};

public:
    char *textureData;
    int width, height;
    int format;

    explicit BmpLoader(const char *file);
    ~BmpLoader() {delete[] textureData;}
};
