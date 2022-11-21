#include <cstring>
#include <fstream>
#include <glload/gl_3_3.h>
#include <glload/gl_load.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Model {
    std::vector<glm::vec4> verticesList;
    std::vector<glm::vec3> normalsList, textureList;
    std::vector<GLushort> facesList;
    const GLuint verticesAttribute = 0;
    const GLuint normalsAttribute = 1;
    GLuint mpvAttribute = -1, colorAttribute = -1;

    GLuint vao = 0;
    GLuint vertices = 0, normals = 0, faces = 0;
    glm::mat4 model;

  public:
    glm::vec3 color = glm::vec3(1.0f); // white default
    void translate(glm::vec3);
    void transform(glm::mat4);
    void rotate(float, glm::vec3);
    void scale(glm::vec3);
    void loadFromString(const std::string &);
    void init(int programId);
    void draw(glm::mat4) const;
    bool collision(const Model &);
    ~Model();
};
