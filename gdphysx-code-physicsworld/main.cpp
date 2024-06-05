#include <iostream>
#include <math.h>
#include <iomanip>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Class/Model3D.h"
#include "Class/Shader.h"
#include "Class/Camera/MyCamera.h"
#include "Class/Camera/OrthoCamera.h"
#include "Class/Camera/PerspectiveCamera.h"

#include "P6/MyVector.h"
#include "P6/P6Particle.h"
#include "P6/PhysicsWorld.h"

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
constexpr std::chrono::nanoseconds timestep(16ms);


int SCREEN_WIDTH = 700;
int SCREEN_HEIGHT = 700;

float timer = 0.0f;
float done = 0;

PerspectiveCamera* persCamera = new PerspectiveCamera();
OrthoCamera* orthoCamera = new OrthoCamera();


std::string getPlacement(int done) {
    std::string placement;
    switch (done) {
    case 0: placement = "1st"; break;
    case 1: placement = "2nd"; break;
    case 2: placement = "3rd"; break;
    case 3: placement = "4th"; break;
    }
    return placement;
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PC01 - Andrea Maxene Legaspi", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    std::vector<Model3D*> models;

    P6::PhysicsWorld pWorld = P6::PhysicsWorld();

    P6::P6Particle p1 = P6::P6Particle();
    p1.Position = P6::MyVector(-700, 700, 201);
    p1.Velocity = P6::MyVector(80, -80, -80);
    p1.Acceleration = P6::MyVector(14.5, -14.5, -14.5);
    pWorld.AddParticle(&p1);

    P6::P6Particle p2 = P6::P6Particle();
    p2.Position = P6::MyVector(700, 700, 173);
    p2.Velocity = P6::MyVector(-90, -90, -130);
    p2.Acceleration = P6::MyVector(-8, -8, -8);
    pWorld.AddParticle(&p2);

    P6::P6Particle p3 = P6::P6Particle();
    p3.Position = P6::MyVector(700, -700, -300);
    p3.Velocity = P6::MyVector(-130, 130, 130);  
    p3.Acceleration = P6::MyVector(-1, 1, 1);
    pWorld.AddParticle(&p3);

    P6::P6Particle p4 = P6::P6Particle();
    p4.Position = P6::MyVector(-700, -700, -150);
    p4.Velocity = P6::MyVector(110, 110, 110);
    p4.Acceleration = P6::MyVector(3, 3, 3);
    pWorld.AddParticle(&p4);


    Model3D* sphere1 = new Model3D(p1.Position,
                                   glm::vec3(15, 15, 15),
                                   glm::vec4(254 / 254.0f, 0.0f / 254.0f, 0.0f / 254.0f, 1.0f));
    models.push_back(sphere1);

    Model3D* sphere2 = new Model3D(p1.Position,
                                   glm::vec3(15, 15, 15),
                                   glm::vec4(52.0f / 254.0f, 235.0f / 254.0f, 70.0f / 254.0f, 1.0f));
    models.push_back(sphere2);

    Model3D* sphere3 = new Model3D(p3.Position,
                                   glm::vec3(15, 15, 15),
                                   glm::vec4(26.0f / 254.0f, 94.0f / 254.0f, 254.0f / 254.0f, 1.0f));
    models.push_back(sphere3);

    Model3D* sphere4 = new Model3D(p3.Position,
                                   glm::vec3(15, 15, 15),
                                   glm::vec4(233.0f / 254.0f, 245.0f / 254.0f, 10.0f / 254.0f, 1.0f));
    models.push_back(sphere4);


    //--------CREATE SHADER--------
    Shader* shader = new Shader();
    GLuint shaderProg = shader->createShader("Shaders/shader.vert", "Shaders/shader.frag");
    glLinkProgram(shaderProg);
    shader->deleteShader();


    //--------SPHERE MODEL--------
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices = sphere1->loadModel("3D/sphere.obj", &attributes);
    
    //bind buffers
    GLuint VAO, VBO;
    sphere1->bindBuffers(attributes, mesh_indices, &VAO, &VBO);

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);


        //FIXED UPDATE
        curr_time = clock::now();       

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        
        timer += (float)dur.count() / 1000;

        prev_time = curr_time;     

        curr_ns += dur; 

        if (curr_ns >= timestep){
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);

            curr_ns -= curr_ns;

            pWorld.Update((float)ms.count() / 1000);


        }
  
        
       if (p1.IsAtOrigin() && !p1.IsDestroyed()) {
            p1.Destroy();
            std::string placement = getPlacement(done);
            std::cout << "Red: " << placement << std::endl;
            std::cout << "Mag. of Velocity: " << p1.Velocity.getMagnitude() << "m/s" << std::endl;
            std::cout << timer << "secs" << std::endl << std::endl;
            done++;
       }

       if (p2.IsAtOrigin() && !p2.IsDestroyed()) {
           p2.Destroy();
           std::string placement = getPlacement(done);
           std::cout << "Green: " << placement << std::endl;
           std::cout << "Mag. of Velocity: " << p2.Velocity.getMagnitude() << "m/s" << std::endl;
           std::cout << timer << "secs" << std::endl << std::endl;
           done++;
       }

       if (p3.IsAtOrigin() && !p3.IsDestroyed()) {
           p3.Destroy();
           std::string placement = getPlacement(done);
           std::cout << "Blue: " << placement << std::endl;
           std::cout << "Mag. of Velocity: " << p3.Velocity.getMagnitude() << "m/s" << std::endl;
           std::cout << timer << "secs" << std::endl << std::endl;
           done++;
       }

       if (p4.IsAtOrigin() && !p4.IsDestroyed()) {
           p4.Destroy();
           std::string placement = getPlacement(done);
           std::cout << "Yellow: " << placement << std::endl;
           std::cout << "Mag. of Velocity: " << p3.Velocity.getMagnitude() << "m/s" << std::endl;
           std::cout << timer << "secs" << std::endl << std::endl;
           done++;
       }

        //--------ORTHO CAMERA-------
        orthoCamera->setPosition(-SCREEN_WIDTH, SCREEN_WIDTH, -SCREEN_HEIGHT, SCREEN_HEIGHT);
        glm::mat4 viewMatrix = orthoCamera->giveView();
        glm::mat4 projection = orthoCamera->giveProjection();
        
        //--------DRAW MODEL--------
        glUseProgram(shaderProg);

        sphere1->setPosition(p1.Position);
        sphere2->setPosition(p2.Position);
        sphere3->setPosition(p3.Position);
        sphere4->setPosition(p4.Position);

        sphere1->bindCamera(shaderProg, projection, viewMatrix);
        
        for (int i = 0; i < models.size(); i++) {
            models[i]->drawModel(mesh_indices, shaderProg, &VAO);
        }
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

