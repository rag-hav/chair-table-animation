#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include <glload/gl_load.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glutil/Shader.h>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const float mouseSpeed = 0.1f;

void keyboard(unsigned char, int, int);
void mouse(int, int, int, int);
void processMouse(int timeDelta);
glm::mat4 getPV(int, int);
void cameraInit();
