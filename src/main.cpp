

#include <iostream>

// GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include "GL/gl.h"
// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <cmath>

#include "shader.h"
#include "texture.h"

#include "geometry/math_vector.h"

/// Vertices data ////////////////////////////
std::vector<GLfloat> vertices = {
    -0.5,   0.5,   0,      1.0f,   0.0f,   0.0f,    0.0f,   1.0f,
    0.5,   0.5,   0,       0.0f,   1.0f,   0.0f,    1.0f,   1.0f,
    0.5,   -0.5,   0,      0.0f,   0.0f,   1.0f,    1.0f,   0.0f,
    -0.5,   -0.5,   0,     0.0f,   0.0f,   1.0f,    0.0f,   0.0f,
};

std::vector<GLfloat> vertices_2 = {
    -0.5,   -0.7,    0,     1.0f,   0.0f,   0.0f,     
    -1.0,   -0.2,    0,     0.0f,   0.0f,   1.0f,
     0.0,   -0.2,    0,     0.0f,   1.0f,   0.0f,
};

std::vector<GLuint> indeces = {
    0,1,2,
    2,3,0,
};
//////////////////////////////////////////////

static float  mix_level = 1.0;

void ChangeMixLevel(float value) {
    float level = mix_level + value;
    if(level > 1.0) {
        mix_level = 1;
    } else if(level < 0) {
        mix_level = 0;
    } else {
        mix_level = level;
    }
}

/// Shaders /////////////////////////////////
Shader  default_shader, hipno_shader, 
        shift_shader,   texture_shader;
GLint vertexColorLocation;
GLint vertexShiftLocation;
GLint textureSamplerLocation;

GLint levelLocation;

Texture tex1, tex2;
/////////////////////////////////////////////

/// Buffers /////////////////////////////////
GLuint VBOs[2];     // Vertex Buffer Object
GLuint VAOs[2];     // Vertex Array Object
GLuint EBO;         // Element Buffer Object
/////////////////////////////////////////////

/// For Errors //////////////////////////////
GLint success;
GLchar inflog[512];
/////////////////////////////////////////////


void CreateShaderProgramms() {
    default_shader  = Shader("../shaders/default.vert", "../shaders/incolor.frag");
    hipno_shader    = Shader("../shaders/default.vert", "../shaders/uniform_color.frag");
    shift_shader    = Shader("../shaders/shift.vert", "../shaders/incolor.frag");

    vertexColorLocation = hipno_shader.GetUniformLocation("ourColor2");

    vertexShiftLocation = shift_shader.GetUniformLocation("Shift");

    texture_shader  = Shader("../shaders/texture.vert", "../shaders/texture.frag");
    // textureSamplerLocation = texture_shader.GetUniformLocation("ourTexture");
    levelLocation   = texture_shader.GetUniformLocation("level");
}


void Init() {
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);
    CreateShaderProgramms();

    tex1 = Texture("../resources/yachik2.jpg");
    tex2 = Texture("../resources/pika.jpeg");
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void BindBufferData1() {
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indeces.size(), indeces.data(), GL_STATIC_DRAW);
}

void BindBufferData2() {
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_2.size(), vertices_2.data(), GL_STATIC_DRAW);

}

void BindVertexAtributes() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void BindVertexAtributes2() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void InitVAO() {
    glBindVertexArray(VAOs[0]);
        BindBufferData1();
        BindVertexAtributes();
    glBindVertexArray(0);
}

void InitVAO2() {
    glBindVertexArray(VAOs[1]);
        BindBufferData2();
        BindVertexAtributes2();
    glBindVertexArray(0);
}


void MainDraw() {
    texture_shader.Use();

    glUniform1f(levelLocation, mix_level);

    glActiveTexture(GL_TEXTURE0);
    tex1.Bind();
    glUniform1i(texture_shader.GetUniformLocation("ourTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    tex2.Bind();
    glUniform1i(texture_shader.GetUniformLocation("ourTexture1"), 1);

    glBindVertexArray(VAOs[0]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // hipno_shader.Use();
    // glBindVertexArray(VAOs[1]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0);
}

void MainUpdate() {
    GLfloat time = glfwGetTime();
    GLfloat redValue    = ((sin(time + (M_PI/3)))+ 1.0) / 2;
    GLfloat greenValue  = ((sin(time + (2*M_PI/3))) + 1.0) / 2;
    GLfloat blueValue   = ((sin(time + (3*M_PI/3))) + 1.0) / 2;

    
    hipno_shader.Use();
    glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);
    shift_shader.Use();
    glUniform2f(vertexShiftLocation, (sin(time))*0.3, 1);
}




// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }    

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);  
    glViewport(0, 0, width, height);


    /////////////////////////////////////////
    Init();
    InitVAO();
    InitVAO2();

    /////////////////////////////////////////


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        MainDraw();

        MainUpdate();

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        ChangeMixLevel(-0.05);

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        ChangeMixLevel(0.05);
}

