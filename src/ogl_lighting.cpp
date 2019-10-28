/**
 * This is second part of lessons abonus Open GL.
 * Lighting.
*/

// GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include "GL/gl.h"

// GLFW
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>

#include "shader.h"
#include "texture.h"
#include "geometry/math_vector.h"


void do_movement();

/**
 * Vertices of cube
*/
GLfloat vert_cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0,0,-1,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0,0,-1,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0,0,-1,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0,0,-1,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0,0,-1,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0,0,-1,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0,0,1,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0,0,1,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0,0,1,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0,0,1,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0,0,1,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0,0,1,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1,0,0,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   -1,0,0,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1,0,0,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1,0,0,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   -1,0,0,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1,0,0,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1,0,0,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1,0,0,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1,0,0,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1,0,0,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1,0,0,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1,0,0,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0,-1,0,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0,-1,0,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0,-1,0,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0,-1,0,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0,-1,0,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0,-1,0,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0,1,0,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0,1,0,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0,1,0,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0,1,0,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0,1,0,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0,1,0,
};


static float  mix_level = 1.0;



/// Shaders /////////////////////////////////
Shader  default_shader, 
        hipno_shader, 
        shift_shader,   
        texture_shader,
        lightingShader,
        lightingShaderSrc;
        
GLint vertexColorLocation;
GLint vertexShiftLocation;
GLint textureSamplerLocation;

GLint levelLocation;

GLint transformLocation;

GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;

GLint modelSrcLoc;
GLint viewSrcLoc;
GLint projectionSrcLoc;

Texture tex1, tex2;
/////////////////////////////////////////////

/// Buffers
GLuint VBO;     // Vertex Buffer Object
GLuint VAO;     // Vertex Array Object
GLuint lightVAO;// Vertex Array Object



/// For Errors //////////////////////////////
GLint success;
GLchar inflog[512];
/////////////////////////////////////////////


void CreateShaderProgramms();

void Init();


void BindBufferData_Cube() 
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_cube), vert_cube, GL_STATIC_DRAW);
}


void BindVertexAtributes() 
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}


void InitVAO() 
{
    glBindVertexArray(VAO);
        BindBufferData_Cube();
        BindVertexAtributes();
    glBindVertexArray(0);

    glBindVertexArray(lightVAO);
        BindBufferData_Cube();
        BindVertexAtributes();
    glBindVertexArray(0);
}


//////////////////////////////////////////////////////

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void UpdateGlobalTime() {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}


GLfloat lastX = 400, lastY = 300;   /// Center of screen 800x600
GLfloat yaw   = -90.0f;
GLfloat pitch = 0.0f;

GLfloat fov = 45;


//////////////////////////////////////////////////////


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ChangeMixLevel(float value);

void MainUpdate();
void MainDraw();


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


glfwWindowHint(GLFW_SAMPLES, 16);
glEnable(GL_MULTISAMPLE);  


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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    /////////////////////////////////////////
    Init();
    // InitVAO();
    // InitVAO2();
    InitVAO();

    /////////////////////////////////////////


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        
        // Render
        // Clear the colorbuffer
        // glClearColor(0.05f, 0.07f, 0.07f, 1.0f);
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
    GLfloat cameraSpeed = 0.05f;

    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        ChangeMixLevel(-0.05);

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        ChangeMixLevel(0.05);
    // if (key == GLFW_KEY_W)
    //     cameraPos += cameraSpeed * cameraFront;
    // if (key == GLFW_KEY_S)
    //     cameraPos -= cameraSpeed * cameraFront;
    // if (key == GLFW_KEY_A)
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // if (key == GLFW_KEY_D)
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.024f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

void do_movement() {
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if(keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if(keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if(keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


void Init() {



    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    CreateShaderProgramms();

    tex1 = Texture("../resources/yachik2.jpg");
    tex2 = Texture("../resources/pika.jpeg");
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

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

void CreateShaderProgramms() {

    // texture_shader  = Shader("../shaders/texture.vert", "../shaders/texture.frag");
    // levelLocation   = texture_shader.GetUniformLocation("level");

    // modelLoc        = texture_shader.GetUniformLocation("model");
    // viewLoc         = texture_shader.GetUniformLocation("view");
    // projectionLoc   = texture_shader.GetUniformLocation("projection");

    lightingShader     = Shader("../shaders/light.vert", "../shaders/light.frag");
    lightingShaderSrc  = Shader("../shaders/light.vert", "../shaders/light_source.frag");

    modelLoc        = lightingShader.GetUniformLocation("model");
    viewLoc         = lightingShader.GetUniformLocation("view");
    projectionLoc   = lightingShader.GetUniformLocation("projection");

    modelSrcLoc        = lightingShaderSrc.GetUniformLocation("model");
    viewSrcLoc         = lightingShaderSrc.GetUniformLocation("view");
    projectionSrcLoc   = lightingShaderSrc.GetUniformLocation("projection");

}

void MainDraw() {
        
    GLfloat radius  = 10.0f;
    GLfloat camX    = sin(glfwGetTime()) * radius;
    GLfloat camZ    = cos(glfwGetTime()) * radius;

    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    glm::vec3 cameraTarget  = glm::vec3(0.0f, 0.0f, 0.0f);   
    glm::vec3 up            = glm::vec3(0.0f, 1.0f, 0.0f);

    view        = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection  = glm::perspective(glm::radians(fov), 800.0f/600.0f, 0.1f, 100.0f);

    auto lightPos = glm::vec3(1.2,1.0,2.0);

    lightingShader.Use();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


/// Uniforms after Use()

    GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
    GLint lightColorLoc  = glGetUniformLocation(lightingShader.Program, "lightColor");
    GLint lightPosLoc    = glGetUniformLocation(lightingShader.Program, "lightPos");
    GLint viewPosLoc     = glGetUniformLocation(lightingShader.Program, "viewPos");
    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f); 

    glBindVertexArray(VAO);
    {
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, g);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);



/// Draw Source
    lightingShaderSrc.Use();
    glUniformMatrix4fv(viewSrcLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionSrcLoc, 1, GL_FALSE, glm::value_ptr(projection));


    glBindVertexArray(lightVAO);
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelSrcLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    // glUniform1f(levelLocation, mix_level);

    // glActiveTexture(GL_TEXTURE0);
    // tex1.Bind();
    // glUniform1i(texture_shader.GetUniformLocation("ourTexture1"), 0);

    // glActiveTexture(GL_TEXTURE1);
    // tex2.Bind();
    // glUniform1i(texture_shader.GetUniformLocation("ourTexture1"), 1);


    // glBindVertexArray(VAO);
    // for(auto i = 0u; i < 10u; ++i) {
    //     glm::mat4 model = glm::mat4(1.0f);
    //     model = glm::translate(model, cubePositions[i]);
    //     model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
    //         model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 15.0f * (i+1)), glm::vec3(0.5f, 1.0f, 0.0f));
    //     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // glDrawArrays(GL_TRIANGLES, 0, 36);
    // }
    // glBindVertexArray(0);


;


}

void MainUpdate() {
    UpdateGlobalTime();
    do_movement();  
}

