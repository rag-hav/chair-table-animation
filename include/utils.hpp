#include <glload/gl_3_3.h>
#include <glload/gl_load.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glutil/Shader.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

// create string from embedded data
#define getEmbeddedData(x)                                                     \
    std::string x;                                                             \
    {                                                                          \
        extern char x##_start[] asm("_binary_" #x "_start");                   \
        extern char x##_end[] asm("_binary_" #x "_end");                       \
        x = std::string(x##_start, x##_end);                                   \
    }

GLuint LoadShader(GLenum eShaderType, const std::string &filename);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);

std::ostream &operator<<(std::ostream &os, const glm::vec3 &);
std::ostream &operator<<(std::ostream &os, const glm::vec4 &);
std::ostream &operator<<(std::ostream &os, const glm::mat4 &);
