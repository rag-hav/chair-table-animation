#include <main.hpp>
#include <camera.hpp>
#include <utils.hpp>
#include <utility>

using namespace std;

glm::mat4 viewMat, prespectiveMat, defaultView;
float pitch = 0, yaw = 0;

// spherical system, right hand axes
/*
    Y
    |   /
    |  /
    |y/
    |_______ X
   /\ x
  /  \
 /
Z


x -> alpha: angle projected by direction vector on xz plane
y -> beta: angle between direction vector and y

Y = cos(y)
X = sin(y) cos(x)
Z = sin(y) sin(x)


--------------------------------------------------------------

viewMat = [Right Up Dir 0]

*/

const GLfloat angleSpeed = 0.08f, posSpeed = 0.002f;
const float pi = glm::pi<float>();

enum {
    leftClick,
    middleClick,
    rightClick,
    scrollDown,
    scrollUp,
    scrollRight,
    scrollLeft
};

enum {
    moveUp = 'w',
    moveLeft = 'a',
    moveDown = 's',
    moveRight = 'd',
    moveCloser = 'q',
    moveFurther = 'z',
    resetView = 'r'
};

int scrollVert = 0, scrollHor = 0;
int moveVert = 0, moveHor = 0, moveAlong = 0;

void cameraInit() {

    defaultView = viewMat = glm::lookAt(
        glm::vec3(0, 0.2, 2), glm::vec3(0, 0.2, 0), glm::vec3(0, 1, 0));

    cout << viewMat << endl;

    prespectiveMat[0][0] = 0.5f;
    prespectiveMat[1][1] = 0.5f;

    const float zNear = 0.2f;
    const float zFar = 10.0f;
    prespectiveMat[2][2] = (zFar + zNear) / (zNear - zFar);
    prespectiveMat[2][3] = 2 * zFar * zNear / (zNear - zFar);
    prespectiveMat[0][2] = -1;

    prespectiveMat = glm::perspective(60.0f, 1.2f, 0.1f, 10.6f);
}

void mouse(int eventType, int movement, int mouseX, int mouseY) {
    // ignore key up events
    if (movement == 0)
        return;

    switch (eventType) {
    case scrollLeft:
        scrollHor--;
        break;
    case scrollRight:
        scrollHor++;
        break;
    case scrollUp:
        scrollVert++;
        break;
    case scrollDown:
        scrollVert--;
        break;
    default:
        return; // ignore everything else
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int mouseX, int mouseY) {
    switch (key) {
    case moveUp:
        moveVert++;
        break;
    case moveDown:
        moveVert--;
        break;
    case moveLeft:
        moveHor--;
        break;
    case moveRight:
        moveHor++;
        break;
    case moveCloser:
        moveAlong++;
        break;
    case moveFurther:
        moveAlong--;
        break;
    case resetView:
        viewMat = defaultView;
        break;
    default:
        glutPostRedisplay();
    }
}

glm::mat4 getPV(int curTime, int timeDelta) {

    if (scrollVert) {
        viewMat = glm::rotate(viewMat, timeDelta * angleSpeed * scrollVert,
                              glm::vec3(viewMat[0][0], viewMat[1][0],
                                        viewMat[2][0])); // rotate along right

        scrollVert = 0;
    }

    if (scrollHor) {
        viewMat = glm::rotate(viewMat, timeDelta * angleSpeed * scrollHor,
                              glm::vec3(viewMat[0][1], viewMat[1][1],
                                        viewMat[2][1])); // rotate along up
        scrollHor = 0;
    }

    float posDelta = timeDelta * posSpeed;
    viewMat[3] += glm::vec4(-moveHor * posDelta, -moveVert * posDelta,
                            moveAlong * posDelta, 0);
    moveVert = moveHor = moveAlong = 0;

    return prespectiveMat * viewMat;
}
