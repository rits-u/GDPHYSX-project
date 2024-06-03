#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Class/Model3D.h"
#include "Class/Shader.h"
#include "Class/Camera/MyCamera.h"
#include "Class/Camera/OrthoCamera.h"
#include "Class/Camera/PerspectiveCamera.h"

#include "P6/MyVector.h"
#include "P6/P6Particle.h"
//using namespace P6;

//openGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <chrono>
using namespace std::chrono_literals;
//time in between "frames"
constexpr std::chrono::nanoseconds timestep(64ms);


int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 600;

float x = 0, y = 0, z = 0, timer = 0;
bool bInput = false;

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
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Andrea Legaspi / Jan Vingno", NULL, NULL);
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


     //VARIABLES
    std::cout << "VELOCITY" << std::endl << "X: ";
    std::cin >> x;

    std::cout << "Y: ";
    std::cin >> y;

    std::cout << "Z: ";
    std::cin >> z;


    P6::P6Particle particle = P6::P6Particle();
    particle.Position = P6::MyVector(0, -700, 0);
    particle.Velocity = P6::MyVector(x, y, z);
    particle.Acceleration = P6::MyVector(0, -50, 0);


    //initialize the clock and variables
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    bool bShowTime = false;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //auto startTime = clock::now();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


         //FIXED UPDATE
        curr_time = clock::now();       //gets current time
        //check the duration in between the last iteration
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);


        timer += (float)dur.count() / 1000;

        prev_time = curr_time;      //set the prev with the current for the next iteration 

        curr_ns += dur; //add the duration since last iteration to the time since our last "frame"

        if (curr_ns >= timestep) {
            //convert ns to ms  
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            std::cout << "MS: " << (float)ms.count() << "\n";

            //reset
            curr_ns -= curr_ns;
           // std::cout << "P6 update" << std::endl;
            std::cout << particle.Position.y << std::endl;
            

            particle.Update((float)ms.count() / 1000);
        }

        std::cout << particle.Position.y << std::endl;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        //--------ORTHO CAMERA-------
        orthoCamera->setPosition(1000.0f, -1000.0f, -1000.0f, 1000.0f);
        orthoCamera->set_zFar(1.0f);
        orthoCamera->set_zNear(-1.0f);
        viewMatrix = orthoCamera->giveView();
        projection = orthoCamera->giveProjection();

        //--------DRAW MODEL--------
        glUseProgram(shaderProg);

        sphere->setPosition(particle.Position.x, particle.Position.y, particle.Position.z);
        sphere->setScale(100, 100, 100);
        sphere->bindCamera(shaderProg, projection, viewMatrix);
        sphere->drawModel(mesh_indices, shaderProg, &VAO);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (particle.Position.y < -800.0f)
            break;

        //print timer here
        if (particle.Position.y < -700 && !bShowTime)
        {
            std::cout << "It took " << timer << " seconds for it to land" << std::endl;
            bShowTime = true;
        }

    }



    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}