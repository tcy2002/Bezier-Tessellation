#include "model.h"

Model::Model(): sample_size(0), index_size(0), vertices(nullptr), indices(nullptr) {

}

Model::~Model() {
    delete[] vertices;
    delete[] indices;
}

void Model::bindBuffer(const VO_MODEL &vo) {
    glBindVertexArray(vo.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vo.VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizei)(sizeof(Vertex) * sample_size), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)sizeof(Vector3f));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(Vector3f)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vo.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)(sizeof(GLuint) * index_size), indices, GL_STATIC_DRAW);
}

void Model::download(const char *path) {
    std::ofstream file(path);
    if (!file.is_open())
        return;

    //以注释的形式保存数据数目
    file << "# " << sample_size << " " << index_size << "\n";

    for (int i = 0; i < sample_size; i++)
        file << "v " << vertices[i].pos.x << " " << vertices[i].pos.y << " " << vertices[i].pos.z << "\n";
    for (int i = 0; i < sample_size; i++)
        file << "vn " << vertices[i].nor.x << " " << vertices[i].nor.y << " " << vertices[i].nor.z << "\n";
    for (int i = 0; i < sample_size; i++)
        file << "vt " << vertices[i].tex.x << " " << vertices[i].tex.y << "\n";

    GLuint d1, d2, d3, d4;
    for (int i = 0; i < index_size; i += 4) {
        d1 = indices[i] + 1;
        d2 = indices[i + 1] + 1;
        d3 = indices[i + 2] + 1;
        d4 = indices[i + 3] + 1;
        file << "f " << d1 << "/" << d1 << "/" << d1 << " "
                << d2 << "/" << d2 << "/" << d2 << " "
                << d3 << "/" << d3 << "/" << d3 << " "
                << d4 << "/" << d4 << "/" << d4 << "\n";
    }
    file.close();
}

void Model::upload(const char *path) {
    std::ifstream file(path);
    if (!file.is_open())
        return;

    //检查文件开头的注释，以确保是自己定义的正确格式
    char c;
    file >> c;
    if (c != '#')
        return;

    file >> sample_size >> index_size;
    delete[] vertices;
    delete[] indices;
    vertices = new Vertex[sample_size];
    indices = new GLuint[index_size];

    for (int i = 0; i < sample_size; i++)
        file >> c >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
    for (int i = 0; i < sample_size; i++)
        file >> c >> c >> vertices[i].nor.x >> vertices[i].nor.y >> vertices[i].nor.z;
    for (int i = 0; i < sample_size; i++)
        file >> c >> c >> vertices[i].tex.x >> vertices[i].tex.y;

    GLuint tmp;
    for (int i = 0; i < index_size; i += 4) {
        file >> c;
        for (int j = 0; j < 4; j++) {
            file >> tmp;
            indices[i + j] = --tmp;
            file >> c >> tmp >> c >> tmp;
        }
    }

    file.close();
}

RotationModel::RotationModel(const Vector3f *curve, GLsizei size): Model() {
    int delta = ceil(1.0 / SAMPLING_RATE);
    sample_num = size / delta + 1;
    sample_num = sample_num > 2 ? sample_num : 2;

    double avg_rad = 0.0f;
    avg_rad += curve[0].x;
    avg_rad += curve[size / 4].x;
    avg_rad += curve[size / 2].x;
    avg_rad += curve[size / 4 * 3].x;
    avg_rad += curve[size >= 1 ? size - 1 : 0].x;
    avg_rad /= 5;

    double delta_angle = CURVE_GRANULARITY / avg_rad * delta;
    loop_num = (int)floor(2.0 * PI / delta_angle);
    delta_angle = (GLfloat)(2.0 * PI / loop_num);
    //为支持纹理的拼接，增加一条尾扫描线，其位置与第一条扫描线重合，纹理坐标分开保存
    loop_num++;

    index_size = (sample_num - 1) * 4 * (loop_num - 1);
    sample_size = sample_num * loop_num;

    vertices = new Vertex[sample_size];
    indices = new GLuint[index_size];

    // 坐标
    double rad, ang;
    int idx = 0;
    for (int i = 0; i < loop_num - 1; i++) {
        ang = delta_angle * i;
        for (int j = 0; j < sample_num - 1; j++) {
            rad = curve[j * delta].x;
            vertices[idx++].pos = {(GLfloat) (rad * std::cos(ang)),
                                     curve[j * delta].y,
                                     (GLfloat) (rad * std::sin(ang))};
        }
        rad = curve[size - 1].x;
        vertices[idx++].pos = {(GLfloat) (rad * std::cos(ang)),
                                              curve[size - 1].y,
                                              (GLfloat) (rad * std::sin(ang))};
    }
    for (int i = 0; i < sample_num; i++)
        vertices[idx++].pos = vertices[i].pos;

    // 法矢量
    int idx1 = 0, idx2 = sample_num;
    for (int i = 0; i < loop_num - 1; i++) {
        for (int j = 0; j < sample_num - 1; j++) {
            Vector3f a = vertices[idx2 + j].pos - vertices[idx1 + j].pos;
            Vector3f b = vertices[idx1 + j + 1].pos - vertices[idx1 + j].pos;
            vertices[idx1 + j].nor = (a % b) & 1.0f;
        }
        idx1 += sample_num;
        idx2 += sample_num;
    }
    for (int i = 0; i < sample_num; i++)
        vertices[idx1++].nor = vertices[i].nor;
    idx1 = (loop_num - 1) * sample_num;
    idx2 = (loop_num - 2) * sample_num;
    for (int i = loop_num - 1; i > 0; i--) {
        vertices[idx1 + sample_num - 1].nor = (vertices[idx1 + sample_num - 2].nor + vertices[idx2 + sample_num - 2].nor) & 1.0f;
        for (int j = sample_num - 2; j > 0; j--)
            vertices[idx1 + j].nor = (vertices[idx1 + j].nor + vertices[idx1 + j - 1].nor +
                                      vertices[idx2 + j].nor + vertices[idx2 + j - 1].nor) & 1.0f;
        vertices[idx1].nor = (vertices[idx1].nor + vertices[idx2].nor) & 1.0f;
        idx1 -= sample_num;
        idx2 -= sample_num;
    }
    idx1 = (loop_num - 1) * sample_num;
    for (int i = 0; i < sample_num; i++)
        vertices[i].nor = vertices[idx1 + i].nor;

    // 索引
    idx = 0;
    idx1 = 0;
    idx2 = sample_num;
    for (int i = 0; i < loop_num - 1; i++) {
        for (int j = 0; j < sample_num - 1; j++) {
            indices[idx++] = idx2 + j + 1;
            indices[idx++] = idx1 + j + 1;
            indices[idx++] = idx1 + j;
            indices[idx++] = idx2 + j;
        }
        idx1 += sample_num;
        idx2 += sample_num;
    }

    // 贴图坐标
    int factor = sample_num - 1;
    for (int i = 0; i < sample_num; i++)
        vertices[i].tex.y = (GLfloat)(1.0 * (factor - i) / factor);
    factor = sample_size - sample_num;
    for (int i = 0; i < sample_size; i += sample_num)
        vertices[i].tex.x = (GLfloat)(1.0 * (factor - i) / factor);
    idx = sample_num;
    for (int i = 1; i < loop_num; i++)
        for (int j = 0; j < sample_num; j++)
            vertices[idx++].tex.y = vertices[j].tex.y;
    idx = idx1 = 0;
    for (int i = 0; i < loop_num; i++) {
        idx++;
        for (int j = 1; j < sample_num; j++)
            vertices[idx++].tex.x = vertices[idx1].tex.x;
        idx1 += sample_num;
    }
}

ModelManager::ModelManager():
        shader(model_vert, model_frag, model_tcs, model_tes, model_gs), view() {
    view.setTransInfo({0.0f, 0.0f, dist});
    view.setProjInfo({30.0f, 1.0f, 1.0f, 100.0f});
}

ModelManager::~ModelManager() {
    for (int i = 0; i < num; i++) {
        glDeleteBuffers(1, &vos[i].VBO);
        glDeleteBuffers(1, &vos[i].EBO);
        delete models[i];
    }
    int size = (int)texes.size();
    for (int i = 0; i < size; i++)
        delete texes[i];
}

void ModelManager::loadTexture() {
    std::string bmp_path = openBmpFile();
    if (bmp_path.empty())
        return;
    texes.push_back(new Texture(bmp_path.c_str()));
    tex_id++;
}

void ModelManager::switchTexture() {
    int size = (int)texes.size();
    if (size == 0)
        return;
    tex_id = (tex_id + 1) % size;
}

void ModelManager::increase() {
    if (dist < -4.0f) {
        dist += 0.1f;
        view.setTransInfo({0.0f, 0.0f, dist});
    }
}

void ModelManager::decrease() {
    if (dist > -12.0f) {
        dist -= 0.1f;
        view.setTransInfo({0.0f, 0.0f, dist});
    }
}

void ModelManager::rotateHorizontal(int a) {
    rot_hor += (GLfloat)(0.2 * a);
    if (rot_hor >= 360.0f)
        rot_hor -= 360.0f;
    else if (rot_hor < 0)
        rot_hor += 360.0f;
}

void ModelManager::rotateVertical(int a) {
    rot_ver += (GLfloat)(0.2 * a);
    if (rot_ver > 90.0f)
        rot_ver = 90.0f;
    else if (rot_ver < -90.0f)
        rot_ver = -90.0f;
}

void ModelManager::rotate() {
    view.setRotInfo({rot_ver,  rot_hor, 0.0f});
}

void ModelManager::addRotationModel(Vector3f *curve, GLsizei size) {
    if (size <= 1)
        return;

    vos.push_back({0, 0, 0});
    glGenVertexArrays(1, &vos.back().VAO);
    glGenBuffers(1, &vos.back().VBO);
    glGenBuffers(1, &vos.back().EBO);

    models.push_back(new RotationModel(curve, size));
    models.back()->bindBuffer(vos.back());

    num++;
}

bool ModelManager::loadRotationModel() {
    std::string path = openObjFile();
    if (path.empty())
        return false;

    vos.push_back({0, 0, 0});
    glGenVertexArrays(1, &vos.back().VAO);
    glGenBuffers(1, &vos.back().VBO);
    glGenBuffers(1, &vos.back().EBO);

    models.push_back(new Model());
    models.back()->upload(path.c_str());
    models.back()->bindBuffer(vos.back());

    num++;
    return true;
}

bool ModelManager::downloadLastModel() {
    if (num == 0)
        return false;

    std::string path = writeObjFile();
    if (path.empty())
        return false;

    models.back()->download(path.c_str());
    return true;
}

void ModelManager::render() {
    shader.use();
    shader.setMat4("gWorld", view.getWorldMat());
    shader.setMat4("gRot", view.getRotMat());
    shader.setMat4("gProj", view.getProjMat());
    shader.setInt("gSampler", 0);
    if (tex_id >= 0) {
        shader.setInt("gUseTex", 1);
        texes[tex_id]->bind(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE0);
    } else {
        shader.setInt("gUseTex", 0);
    }

    if (mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
    } else {
        glLineWidth(0.8f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_CULL_FACE);
    }

    for (int i = 0; i < num; i++) {
        glBindVertexArray(vos[i].VAO);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glDrawElements(GL_PATCHES, models.back()->getIndexSize(), GL_UNSIGNED_INT, nullptr);
    }
}
