

#include <iostream>

// GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include "GL/gl.h"
// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include "geometry/math_vector.h"

/// Vertices data ////////////////////////////
std::vector<GLfloat> vertices = {
    0,      0.5,    0,  ///< First
    0.5,    0.0,    0,
    -0.5,   0,      0,

    // -0.5,      -0.7,    0,  ///< Second
    // 0.0,    -0.2,    0,
    // -1.0,   -0.2,      0, 
};
std::vector<GLfloat> vertices_2 = {

    -0.5,      -0.7,    0,  ///< Second
    0.0,    -0.2,    0,
    -1.0,   -0.2,      0, 
};

std::vector<GLuint> indeces = {
    1,2,3
};
//////////////////////////////////////////////


/// Shaders source ///////////////////////////
std::string vertexShader_source = 
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\n";

std::string fragmentShader_source =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

/////////////////////////////////////////////

/// Shaders /////////////////////////////////
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
/////////////////////////////////////////////

/// Buffers /////////////////////////////////
GLuint VBO; // Vertex Buffer Object
GLuint VAO; // Vertex Array Object
GLuint VBO2; // Vertex Buffer Object
GLuint VAO2; // Vertex Array Object
GLuint EBO; // Element Buffer Object
/////////////////////////////////////////////

/// For Errors //////////////////////////////
GLint success;
GLchar inflog[512];
/////////////////////////////////////////////


void CreateShaderProgramm() {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vertexShaderData = vertexShader_source.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderData, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, inflog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
        inflog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentShaderData = fragmentShader_source.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderData, NULL);
    glCompileShader(fragmentShader);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, inflog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
        inflog << std::endl;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, inflog);
        std::cout << inflog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Init() {
    glEnable(GL_PROGRAM_POINT_SIZE);
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);
    CreateShaderProgramm();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}



void BindBufferData1() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indeces.size(), indeces.data(), GL_STATIC_DRAW);
}

void BindBufferData2() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_2.size(), vertices_2.data(), GL_STATIC_DRAW);

}

void BindVertexAtributes() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
}

void InitVAO() {
    glBindVertexArray(VAO);
        BindBufferData1();
        BindVertexAtributes();
    glBindVertexArray(0);
}

void InitVAO2() {
    glBindVertexArray(VAO2);
        BindBufferData2();
        BindVertexAtributes();
    glBindVertexArray(0);
}


void MainDraw() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 3);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
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

