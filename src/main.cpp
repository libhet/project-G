

#include <iostream>

// GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include "GL/gl.h"
// GLFW
#include <GLFW/glfw3.h>


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm/gtx/string_cast.hpp"

#include <vector>
#include <string>
#include <cmath>

#include "shader.h"
#include "texture.h"

#include "geometry/math_vector.h"

#include <thread>
#include <chrono>

// using namespace std::chrono_literal;

/// Vertices data ////////////////////////////
std::vector<GLfloat> vertices = {
    -0.5,   0.5,   0,      1.0f,   0.0f,   0.0f,    0.0f,   1.0f,
    0.5,   0.5,   0,       0.0f,   1.0f,   0.0f,    1.0f,   1.0f,
    0.5,   -0.5,   0,      0.0f,   0.0f,   1.0f,    1.0f,   0.0f,
    -0.5,   -0.5,   0,     0.0f,   0.0f,   1.0f,    0.0f,   0.0f,
};

GLfloat vert_cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.8f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
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
Shader  default_shader, 
        hipno_shader, 
        shift_shader,   
        texture_shader;
        
GLint vertexColorLocation;
GLint vertexShiftLocation;
GLint textureSamplerLocation;

GLint levelLocation;

GLint transformLocation;

GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;

Texture tex1, tex2;
/////////////////////////////////////////////

/// Buffers /////////////////////////////////
GLuint VBO;     // Vertex Buffer Object
GLuint VAO;     // Vertex Array Object
GLuint EBO;         // Element Buffer Object
/////////////////////////////////////////////

/// For Errors //////////////////////////////
GLint success;
GLchar inflog[512];
/////////////////////////////////////////////


void CreateShaderProgramms() {
    // default_shader  = Shader("../shaders/default.vert", "../shaders/incolor.frag");
    // hipno_shader    = Shader("../shaders/default.vert", "../shaders/uniform_color.frag");
    // shift_shader    = Shader("../shaders/shift.vert", "../shaders/incolor.frag");

    // vertexColorLocation = hipno_shader.GetUniformLocation("ourColor2");

    // vertexShiftLocation = shift_shader.GetUniformLocation("Shift");

    texture_shader  = Shader("../shaders/texture.vert", "../shaders/texture.frag");
    // textureSamplerLocation = texture_shader.GetUniformLocation("ourTexture");
    levelLocation   = texture_shader.GetUniformLocation("level");
    // transformLocation   = texture_shader.GetUniformLocation("transform");

    modelLoc        = texture_shader.GetUniformLocation("model");
    viewLoc         = texture_shader.GetUniformLocation("view");
    projectionLoc   = texture_shader.GetUniformLocation("projection");



    
}


    

void Init() {
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    CreateShaderProgramms();

    tex1 = Texture("../resources/yachik2.jpg");
    tex2 = Texture("../resources/pika.jpeg");
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




}



// void BindBufferData1() {
//     glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indeces.size(), indeces.data(), GL_STATIC_DRAW);
// }

// void BindBufferData2() {
//     glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertices_2.size(), vertices_2.data(), GL_STATIC_DRAW);

// }

void BindBufferData3() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_cube), vert_cube, GL_STATIC_DRAW);
}

// void BindVertexAtributes() {
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
//     glEnableVertexAttribArray(2);
// }

// void BindVertexAtributes2() {
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
// }

void BindVertexAtributes3() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

// void InitVAO() {
//     glBindVertexArray(VAOs[0]);
//         BindBufferData1();
//         BindVertexAtributes();
//     glBindVertexArray(0);
// }

// void InitVAO2() {
//     glBindVertexArray(VAOs[1]);
//         BindBufferData2();
//         BindVertexAtributes2();
//     glBindVertexArray(0);
// }

void InitVAO3() {
    glBindVertexArray(VAO);
        BindBufferData3();
        BindVertexAtributes3();
    glBindVertexArray(0);
}



void MainDraw() {
        
    
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);



    
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);






    texture_shader.Use();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        

    // GLfloat time = glfwGetTime();

    /// Uniforms after Use()

    glUniform1f(levelLocation, mix_level);

    glActiveTexture(GL_TEXTURE0);
    tex1.Bind();
    glUniform1i(texture_shader.GetUniformLocation("ourTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    tex2.Bind();
    glUniform1i(texture_shader.GetUniformLocation("ourTexture1"), 1);
    
    // glm::mat4 transform;

    // Siquence of transformation is matter
    // transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    // transform = glm::rotate(transform, 45.0f * time, glm::vec3(0,0,1));
    // transform = glm::scale(transform, glm::vec3(0.5,0.5,0.5));
    // glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));






    glBindVertexArray(VAO);

    for(auto i = 0u; i < 10u; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 15.0f * (i+1)), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    // glm::mat4 transform2;
    // transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.5f, 0.0f));
    // transform2 = glm::scale(transform2, glm::vec3(0.5 * sin(time)  + 0.5, 0.5 * sin(time) + 0.5, 0.5 * sin(time) + 0.5));
    // glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform2));

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    glBindVertexArray(0);

    // hipno_shader.Use();
    // glBindVertexArray(VAOs[1]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0);
}

void MainUpdate() {
    // GLfloat time = glfwGetTime();
    // GLfloat redValue    = ((sin(time + (M_PI/3)))+ 1.0) / 2;
    // GLfloat greenValue  = ((sin(time + (2*M_PI/3))) + 1.0) / 2;
    // GLfloat blueValue   = ((sin(time + (3*M_PI/3))) + 1.0) / 2;

    
    // hipno_shader.Use();
    // glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);
    // shift_shader.Use();
    // glUniform2f(vertexShiftLocation, (sin(time))*0.3, 1);


    // texture_shader.Use();
    
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
    // InitVAO();
    // InitVAO2();
    InitVAO3();

    /////////////////////////////////////////


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
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

