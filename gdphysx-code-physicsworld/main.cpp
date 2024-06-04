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
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
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
constexpr std::chrono::nanoseconds timestep(32ms);


int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 600;

PerspectiveCamera* persCamera = new PerspectiveCamera();
OrthoCamera* orthoCamera = new OrthoCamera();



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its O      penGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "hello", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::vector<Model3D*> models;

    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    P6::P6Particle p1 = P6::P6Particle();
    p1.Position = P6::MyVector(-200, 0, 0);
    p1.Velocity = P6::MyVector(5, 0, 0);
    p1.Acceleration = P6::MyVector(-3, 0, 0);
    pWorld.AddParticle(&p1);

    P6::P6Particle p2 = P6::P6Particle();
    p2.Position = P6::MyVector(20, 0, 0);
    p2.Velocity = P6::MyVector(-10, 0, 0);
    p2.Acceleration = P6::MyVector(-30, 0, 0);
    pWorld.AddParticle(&p2);


    Model3D* sphere1 = new Model3D(p1.Position,
                                   glm::vec3(100, 100, 100), 
                                   glm::vec4(126.0f / 254.0f, 194.0f / 254.0f, 254.0f / 254.0f, 1.0f));
    models.push_back(sphere1);

    Model3D* sphere2 = new Model3D(p2.Position,
                                   glm::vec3(100, 100, 100),
                                   glm::vec4(26.0f / 254.0f, 94.0f / 254.0f, 254.0f / 254.0f, 1.0f));
    models.push_back(sphere2);


    //--------CREATE SHADER--------
    Shader* shader = new Shader();
    GLuint shaderProg = shader->createShader("Shaders/shader.vert", "Shaders/shader.frag");
    glLinkProgram(shaderProg);
    shader->deleteShader();


    //--------SPHERE MODEL--------
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices = sphere1->loadModel("3D/sphere.obj", &attributes);
   // std::vector<GLuint> mesh_indices2 = sphere2->loadModel("3D/sphere.obj", &attributes);
    
    //bind buffers
    GLuint VAO, VBO;
    sphere1->bindBuffers(attributes, mesh_indices, &VAO, &VBO);
  //  sphere2->bindBuffers(attributes, mesh_indices2, &VAO, &VBO);
    std::cout << "sphere model loaded" << std::endl;


   


    //initialize the clock and variables
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        //FIXED UPDATE
        curr_time = clock::now();       //gets current time
        //check the duration in between the last iteration
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        
        prev_time = curr_time;      //set the prev with the current for the next iteration 

        curr_ns += dur; //add the duration since last iteration to the time since our last "frame"

        if (curr_ns >= timestep){
            //convert ns to ms  
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            std::cout << "MS: " << (float)ms.count() << "\n";

            //reset
            curr_ns -= curr_ns;
            std::cout << "P6 update" << std::endl;

          //  particle.Update((float)ms.count() / 1000);
            pWorld.Update((float)ms.count() / 1000);

            //if destroy
//            if(p1.Position == P6::MyVector())


        }
  
        std::cout << "Normal Update" << std::endl;

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        //--------ORTHO CAMERA-------
        orthoCamera->setPosition(-1000.0f, 1000.0f, 1000.0f, -1000.0f);
        //orthoCamera->setPosition(-1.0f, 1.0f, 1.0f, -1.0f);
        viewMatrix = orthoCamera->giveView();
        projection = orthoCamera->giveProjection();
        
        //--------DRAW MODEL--------
        glUseProgram(shaderProg);

        sphere1->setPosition(p1.Position);
        sphere2->setPosition(p2.Position);
        
        for (int i = 0; i < models.size(); i++) {
           // models[i]->setPosition(positions[i]);
            models[i]->bindCamera(shaderProg, projection, viewMatrix);
            models[i]->drawModel(mesh_indices, shaderProg, &VAO);
        }
        



       // sphere->setPosition(sample.x, sample.y, sample.z);
     //   sphere->setPosition(particle.Position.x, particle.Position.y, particle.Position.z);
       // sphere->setPosition((glm::vec3)sample); typecast version
        //  sphere1->setScale(100, 100, 100)
        //sphere1->setPosition(p1.Position);
        //sphere1->bindCamera(shaderProg, projection, viewMatrix);
        //sphere1->drawModel(mesh_indices, shaderProg, &VAO);

        ////sphere2->setPosition(p2.Position.x, p2.Position.y, p2.Position.z);
        ////sphere2->setScale(100, 100, 100);
        //sphere2->setPosition(p2.Position);
        //sphere2->bindCamera(shaderProg, projection, viewMatrix);    
        //sphere2->drawModel(mesh_indices, shaderProg, &VAO);

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


//pls do:
//update model3D class, vector/list of models
    //add color attribute for models