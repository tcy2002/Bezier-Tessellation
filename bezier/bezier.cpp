#include "bezier.h"

Bezier::~Bezier() {
    delete[] control_vertices;
    delete[] curve_vertices;
}

GLfloat Bezier::getParam(int n, int x, double t) {
    double ret = std::pow(t, x) * std::pow(1.0 - t, n - x);
    int up = 1, down = 1;
    for (int i = 0; i < x; i++) {
        up *= (n - i);
        down *= (i + 1);
    }
    return (GLfloat)(1.0 * up / down * ret);
}

void Bezier::calcLastPoint(const Vector2f &src, const Vector2f &ctl, Vector2f &dst) {
    Vector2f sym, nor;
    sym = src | ctl;
    nor = sym - ctl;
    nor &= 0.5f;
    dst = ctl + nor;
}

void Bezier::updateSamples() {
    sample_num = ceil(total_length / CURVE_GRANULARITY);
    delete[] curve_vertices;
    curve_vertices = nullptr;
    curve_vertices = new Vector2f[(sample_num + 1) * 2]{};

    for (int i = 0; i <= sample_num; i++) {
        double t = 1.0 * i / (double)sample_num;
        for (int n = 0; n < control_num; n++) {
            GLfloat param = getParam(control_num - 1, n, t);
            curve_vertices[i] += {control_vertices[n].x * param, control_vertices[n].y * param};
        }
    }

    int idx = sample_num + 1;
    GLfloat x, y;
    for (int i = 0; i <= sample_num; i++) {
        x = curve_vertices[i].x;
        y = curve_vertices[i].y;
        curve_vertices[idx++] = {-x, y};
    }
}

void Bezier::adjustVertex(int n, const Vector2f &pos) {
    if (n > 0) {
        total_length -= (control_vertices[n - 1] / control_vertices[n]);
        total_length += (control_vertices[n - 1] / pos);
    }
    if (n < control_num - 1) {
        total_length -= (control_vertices[n + 1] / control_vertices[n]);
        total_length += (control_vertices[n + 1] / pos);
    }
    if (n >= control_num - 2)
        calcLastPoint(control_vertices[control_num - 2], control_vertices[control_num - 1], control_vertices[control_num]);

    control_vertices[n] = pos;
    updateSamples();
}

void Bezier::addVertex(const Vector2f &vert) {
    if (state == DONE)
        return;
    state = DOING;

    if (control_num > 0 && (control_vertices[control_num - 1] == vert))
        return;

    if (control_num > 0 && control_num % 4 == 0) {
        Vector2f *tmp = control_vertices;
        control_vertices = new Vector2f[control_num + 5];
        memcpy(control_vertices, tmp, sizeof(Vector2f) * control_num);
        delete[] tmp;
    }

    control_vertices[control_num] = vert;
    if (control_num > 0) {
        total_length += (control_vertices[control_num - 1] / control_vertices[control_num]);
        calcLastPoint(control_vertices[control_num - 1], control_vertices[control_num], control_vertices[control_num + 1]);
    }

    control_num++;
    updateSamples();
}

void Bezier::bindBuffer(const VO_BEZIER &vo) const {
    if (curve_vertices == nullptr)
        return;

    glBindVertexArray(vo.pVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vo.pVBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizei)(sizeof(Vector2f) * (control_num + 1)), control_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), nullptr);

    glBindVertexArray(vo.cVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vo.cVBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizei)(sizeof(Vector2f) * (sample_num + 1) * 2), curve_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), nullptr);
}

bezierManager::bezierManager(): shader(simple_vert, simple_frag) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), vs, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
}

bezierManager::~bezierManager() {
    for (int i = 0; i < num; i++) {
        glDeleteBuffers(1, &vos[i].pVBO);
        glDeleteBuffers(1, &vos[i].cVBO);
        delete curves[i];
    }
}

void bezierManager::addCurve() {
    vos.push_back({0, 0, 0, 0});
    glGenVertexArrays(1, &vos.back().pVAO);
    glGenVertexArrays(1, &vos.back().cVAO);
    glGenBuffers(1, &vos.back().pVBO);
    glGenBuffers(1, &vos.back().cVBO);

    curves.push_back(new Bezier());
    num++;
}

void bezierManager::endCurve() {
    if (curves.empty())
        return;
    curves.back()->end();
    state = DONE;
}

void bezierManager::end() {
    if (num > 0)
        curves.back()->end();
    state = ALL_DONE;
}

void bezierManager::addVertex(const Vector2f &vert) {
    if (state == ALL_DONE)
        return;

    if (state == START) {
        addCurve();
        state = DOING;
    } else if (state == DONE) {
        addCurve();
        curves[num - 2]->end();
        curves.back()->addVertex(curves[num - 2]->getEndVertex());
        curves.back()->bindBuffer(vos.back());
        state = DOING;
    }

    curves.back()->addVertex(vert);
    curves.back()->bindBuffer(vos.back());
}

bezierManager::selected_points *bezierManager::findVertex(const Vector2f &vert) {
    auto list = new selected_points();
    select = {-1, -1};
    for (int i = 0; i < num; i++) {
        int n = curves[i]->getVertexNum();
        for (int j = 0; j < n; j++) {
            Vector2f v = curves[i]->getVertex(j);
            if (v / vert < 0.04) {
                list->emplace_back(i, j);
                select = {i, j};
            }
        }
    }
    return list;
}

void bezierManager::adjustVertex(const std::pair<int, int> &vert_id, const Vector2f &pos) {
    curves[vert_id.first]->adjustVertex(vert_id.second, pos);
    curves[vert_id.first]->bindBuffer(vos[vert_id.first]);
}

void bezierManager::render() {
    shader.use();
    shader.setFloat("aspect", aspect);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(1);

    glLineWidth(1.2f);
    shader.setVec4("myColor", {0.64f, 0.15f, 0.64f, 1.0f});
    glDrawArrays(GL_LINES, 0, 2);

    for (int i = 0; i < num; i++) {
        glBindVertexArray(vos[i].cVAO);
        glEnableVertexAttribArray(1);

        glLineWidth(1.6f);
        shader.setVec4("myColor", {0.0f, 0.0f, 0.0f, 1.0f});
        int size = curves[i]->getSampleNum();
        glDrawArrays(GL_LINE_STRIP, 0, size);
        glDrawArrays(GL_LINE_STRIP, size, size);

        glBindVertexArray(vos[i].pVAO);
        glEnableVertexAttribArray(1);
        int point_num = curves[i]->getVertexNum();

        shader.setVec4("myColor", {0.93f, 0.1f, 0.14f, 1.0f});
        if (i != select.first) {
            glPointSize(5.0f);
            glDrawArrays(GL_POINTS, 0, point_num);
        } else {
            glPointSize(5.0f);
            glDrawArrays(GL_POINTS, 0, select.second);
            glDrawArrays(GL_POINTS, select.second + 1, point_num - select.second - 1);
            glPointSize(8.0f);
            glDrawArrays(GL_POINTS, select.second, 1);
        }

        glLineWidth(0.8f);
        shader.setVec4("myColor", {0.6f, 0.85f, 0.92f, 1.0f});
        glDrawArrays(GL_LINE_STRIP, 0, point_num);
        if ((state != ALL_DONE && curves[i]->getState() == DONE && i == num - 1 && point_num > 1) ||
            (state == ALL_DONE && i != num - 1)) {
            shader.setVec4("myColor", {0.71f, 0.9f, 0.11f, 1.0f});
            glDrawArrays(GL_LINE_STRIP, point_num - 1, 2);
        }
    }
}

void bezierManager::to3dCurve(Vector3f *&curve, GLsizei &size) {
    size = 0;
    if (num == 0)
        return;

    for (const auto &c : curves)
        size += c->getSampleNum() - 1;
    size++;

    curve = new Vector3f[size];
    int p = 0, len;
    Vector2f sample;
    for (const auto &c : curves) {
        len = c->getSampleNum() - 1;
        for (int i = 0; i < len; i++) {
            sample = c->getSample(i);
            curve[p].x = sample.x;
            curve[p].y = sample.y;
            curve[p++].z = 0.0f;
        }
    }
    sample = curves.back()->getSample(len);
    curve[p].x = sample.x;
    curve[p].y = sample.y;
    curve[p].z = 0.0f;
}
