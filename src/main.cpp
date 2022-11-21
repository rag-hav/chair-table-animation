#include <camera.hpp>
#include <main.hpp>
#include <model.hpp>
#include <utils.hpp>

using namespace std;

Model chair;
Model table;

const int FPS = 30;
const int TIMEBETWEENFRAMES = 1000 / FPS;
int lastFrameTime = 0;
int curTime = 0;
int timeDelta = 0;
float speed = 0.001;

glm::mat4 chairRot;

/*

ll ans  = 0;

for (int i = 0; i < n; i++) {
    ll mx = 0, sum = 0;
    for (int j = i; j < n; j++) {
        mx = max(mx, arr[i]);
        ( sum += mx ) %= mod;
        ( ans += sum ) %= mod;

    }
}

*/

int main(int argc, char **argv) {

    glutInit(&argc, argv);

    const int width = 800;
    const int height = 800;
    const unsigned int displayMode =
        GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

    glutInitDisplayMode(displayMode);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowSize(width, height);

    int window = glutCreateWindow(argv[0]);
    glload::LoadFunctions();

    cout << "Initializing..." << endl;
    init();
    glutDisplayFunc(display);
    // glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    cout << "Starting main loop..." << endl;
    glutMainLoop();
}

void display() {
    curTime = glutGet(GLUT_ELAPSED_TIME);
    // cout << curTime << endl;
    timeDelta = curTime - lastFrameTime;
    if (timeDelta < TIMEBETWEENFRAMES) {
        glutPostRedisplay();
        return;
    }
    glClearDepth(1.0f);
    glClearColor(0.2f, 0.3f, 0.7f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 vp = getPV(curTime, timeDelta);

    chair.translate(glm::vec3(-0.267639, -0.051634, 0));
    chair.rotate(timeDelta * speed, glm::vec3(0, 0, 1.0));
    chair.translate(glm::vec3(0.267639, 0.051634, 0));
    table.translate(glm::vec3(-0.000003 * timeDelta, 0, 0));

    chair.draw(vp);
    table.draw(vp);

    lastFrameTime = curTime;
    glutSwapBuffers();
    glutPostRedisplay();
}

void init() {

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 10.0f);
    glEnable(GL_DEPTH_CLAMP);

    cameraInit();

    table.color = glm::vec3(0.0, 0.3, 0);

    getEmbeddedData(data_chair_obj);
    getEmbeddedData(data_table_obj);
    chair.loadFromString(data_chair_obj);
    table.loadFromString(data_table_obj);

    chair.translate(glm::vec3(0.5, 0.0, 0.0));

    chair.scale(glm::vec3(0.4, 0.4, 0.4));
    table.scale(glm::vec3(0.5, 0.5, 0.5));

    GLuint program = initShaders();

    chair.init(program);
    table.init(program);
}

int initShaders() {
    std::vector<GLuint> shaderList;

    // get the location of embedded fragment shader
    getEmbeddedData(shader_vertex_vert);
    getEmbeddedData(shader_fragment_frag);

    shaderList.push_back(LoadShader(GL_VERTEX_SHADER, shader_vertex_vert));
    shaderList.push_back(LoadShader(GL_FRAGMENT_SHADER, shader_fragment_frag));

    GLuint programId = CreateProgram(shaderList);

    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
    glUseProgram(programId);
    return programId;
}
