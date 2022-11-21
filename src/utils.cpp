#include <utils.hpp>

using namespace std;

GLuint LoadShader(GLenum eShaderType, const std::string &shaderData) {

    try {
        return glutil::CompileShader(eShaderType, shaderData);
    } catch (std::exception &e) {
        cerr << "Failed to Compile Shader:\n" << shaderData << endl;
        cerr << e.what() << endl;
        throw;
    }
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList) {
    try {
        GLuint prog = glutil::LinkProgram(shaderList);
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        return prog;
    } catch (std::exception &e) {
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        cerr << e.what() << endl;
        throw;
    }
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
    return os << v.x << ' ' << v.y << ' ' << v.z << '\n';
}
std::ostream &operator<<(std::ostream &os, const glm::vec4 &v) {
    return os << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << '\n';
}
std::ostream &operator<<(std::ostream &os, const glm::mat4 &m) {
    for (int i = 0; i < 4; i++)
        os << m[i];
    return os;
}
