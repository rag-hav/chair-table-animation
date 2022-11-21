#include <iterator>
#include <memory>
#include <model.hpp>
#include <sstream>

using namespace std;

template <class T>
void uploadDataToGPU(GLuint &bufferVariable, const vector<T> &bufferData) {
    assert(!bufferData.empty()); /* sanity check */
    glGenBuffers(1, &bufferVariable);
    glBindBuffer(GL_ARRAY_BUFFER, bufferVariable);
    glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(bufferData[0]),
                 bufferData.data(), GL_STATIC_DRAW);
}

void Model::loadFromString(const string &modelData) {
    // https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ

    struct Vertex {
        GLushort vertexIndex, normalIndex;
        Vertex(GLushort vertexIndex, GLushort normalIndex)
            : vertexIndex(vertexIndex), normalIndex(normalIndex){};
        bool operator<(const Vertex &other) const {
            return tie(vertexIndex, normalIndex) <
                   tie(other.vertexIndex, other.normalIndex);
        }
    };

    vector<glm::vec3> raw_normals;
    vector<glm::vec4> raw_vertices;
    vector<Vertex> raw_faces; // every 3 vertex are a face

    string line;
    stringstream modelDataStream(modelData);
    while (getline(modelDataStream, line)) {
        istringstream s(line);
        string type;
        s >> type;
        if (type == "v") {
            // a vertex
            glm::vec4 v;
            s >> v.x >> v.y >> v.z;

            if (!s.eof())
                s >> v.w;
            else
                v.w = 1.0f; // default 1
            raw_vertices.push_back(v);
        } else if (type == "f") {
            // a face
            for (int i = 0; i < 3; i++) {
                GLushort v, t, n;
                char c;
                s >> v >> c;
                s >> t >> c;
                s >> n;
                raw_faces.emplace_back(v - 1, n - 1);
            }
        } else if (type == "vn") {
            // a vertex normal
            glm::vec3 n;
            s >> n.x >> n.y >> n.z;
            raw_normals.push_back(n);
        } /* else if (type == "vt") {
            // a vertex texture
            glm::vec3 v;
            for (int i = 0; i < 3; i++) {
                if (!s.eof())
                    s >> v[i];
                else
                    v[i] = 0; // default 0
            }
            textureList.push_back(v);
        }  */

        /* anything else is ignored */
    }
    // duplicate vertices used with different normals

    // wavefont format defines normals with faces but opengl needs each
    // vertex to have its own normal, so we duplicate vertexes that have
    // been used with different normals

    // store index of vertex in the final vertex vector with 1 indexing
    std::map<Vertex, GLushort> indexes;

    for (auto v : raw_faces) {
        GLushort &res = indexes[v];
        if (res == 0) { // since we store 1 indexing, value 0 means that this
                        // vertex does not yet exist, so we create it
            assert(v.vertexIndex < raw_vertices.size());
            assert(v.normalIndex < raw_normals.size());
            verticesList.push_back(raw_vertices[v.vertexIndex]);
            normalsList.push_back(raw_normals[v.normalIndex]);

            // store index of last element in 1 indexing
            res = verticesList.size();
        }
        facesList.push_back(res - 1); // opengl requires 0 indexing
    }
}

void Model::init(int programId) {

    // get uniform locations
    mpvAttribute = glGetUniformLocation(programId, "mpv");
    colorAttribute = glGetUniformLocation(programId, "vertexColor");

    // upload data to gpu buffers
    uploadDataToGPU(vertices, verticesList);
    uploadDataToGPU(normals, normalsList);
    uploadDataToGPU(faces, facesList);

    // Intialize vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(verticesAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glVertexAttribPointer(verticesAttribute,
                          4,        // number of elements per vertex
                          GL_FLOAT, // type of each element
                          GL_FALSE, // take out value as is
                          0,        // no spacing between data
                          0         // no offset
    );

    glEnableVertexAttribArray(normalsAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, normals);
    glVertexAttribPointer(normalsAttribute,
                          3,        // number of elements per vertex
                          GL_FLOAT, // type of each element
                          GL_FALSE, // take out value as is
                          0,        // no spacing between data
                          0         // no offset
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces);

    // unload the vao
    glBindVertexArray(0);
}

void Model::draw(glm::mat4 vp) const {
    glBindVertexArray(vao);
    glm::mat4 mpv = vp * model;
    glUniform3f(colorAttribute, color.r, color.b, color.g);
    glUniformMatrix4fv(mpvAttribute, 1, GL_FALSE /* Row Major */,
                       glm::value_ptr(mpv));
    glDrawElements(GL_TRIANGLES, facesList.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void Model::translate(glm::vec3 offset) {
    model = glm::translate(model, offset);
}

void Model::scale(glm::vec3 scalingFactors) {
    model = glm::scale(model, scalingFactors);
}

void Model::rotate(float angle, glm::vec3 axis) {
    model = glm::rotate(model, angle, axis);
}

void Model::transform(glm::mat4 transformation) {
    model = transformation * model;
}

Model::~Model() {
    // Cleanup

    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vertices)
        glDeleteBuffers(1, &vertices);
    if (normals)
        glDeleteBuffers(1, &normals);
    if (faces)
        glDeleteBuffers(1, &faces);
}

bool Model::collision(const Model &other) { return true; }
