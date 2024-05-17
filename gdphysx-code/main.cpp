#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Class/Model3D.h"
#include "Class/Shader.h"
#include "Class/Camera/MyCamera.h"
#include "Class/Camera/OrthoCamera.h"
#include "Class/Camera/PerspectiveCamera.h"
#include "P6/MyVector.h"  

//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 600;

PerspectiveCamera* persCamera = new PerspectiveCamera();
OrthoCamera* orthoCamera = new OrthoCamera();
Model3D* sphere = new Model3D();
    
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Andrea Maxene Legaspi", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    //--------CREATE SHADER--------
    Shader* shader = new Shader();
    GLuint shaderProg = shader->createShader("Shaders/shader.vert", "Shaders/shader.frag");
    glLinkProgram(shaderProg);
    shader->deleteShader();


    //--------SPHERE MODEL--------
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices = sphere->loadModel("3D/sphere.obj", &attributes);
    
    //bind buffers
    GLuint VAO, VBO;
    sphere->bindBuffers(attributes, mesh_indices, &VAO, &VBO);
    std::cout << "sphere model loaded" << std::endl;

    P6::MyVector sample(0, 0, 0);
    P6::MyVector toAdd(100, 100, 0);
    //sample.x = 5;

  //  sample = sample + toAdd;
    sample += toAdd;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
  

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        //--------ORTHO CAMERA-------
        orthoCamera->setPosition(-400.0f, 400.0f, 400.0f, -400.0f);
       // orthoCamera->setPosition(-1.0f, 1.0f, 1.0f, -1.0f);
        viewMatrix = orthoCamera->giveView();
        projection = orthoCamera->giveProjection();
        
        //--------DRAW MODEL--------
        glUseProgram(shaderProg);
        
        sphere->setPosition(sample.x, sample.y, sample.z);
        sphere->setScale(100, 100, 100);
        sphere->bindCamera(shaderProg, projection, viewMatrix);
        sphere->drawModel(mesh_indices, shaderProg, &VAO);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}