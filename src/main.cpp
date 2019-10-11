

#include <iostream>

// GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include "GL/gl.h"
// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include <geometry/math_vector.h>


std::vector<Vector3f> vertices = {
    Vector3f(0,1,0),
    Vector3f(1,0,0),
    Vector3f(-1,0,0),
};

GLuint VBO; // Vertex Buffer Object
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

std::string vertexShader_source = 
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
"gl_Position = vec4(position.x, postition.y, position.z, 1.0);\n"
"}\n";

GLuint vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);

glShaderSource(vertexShader, 1, &(vertexShader_source.c_str()), NULL);
glCompileShader(vertexShader);

GLint success;
GLchar inflog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
    inflog << std::endl;
}

std::string fragmentShader_source =
"#version 330 core\n"
"out vec4 color\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

GLuint fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, &(fragmentShader_source.c_str()),NULL);
glCompileShader(fragmentShader);

GLuint shaderProgram;
shaderProgram = glCreateProgram();

glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
    std::cout << inflog << std::endl;
}

glUseProgramm(shaderProgram);

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
glEnableVertexAttribArray(0);

GLuint VAO; // Vertex Array Object
glGenVertexArrays(1, &VAO);

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

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
}

