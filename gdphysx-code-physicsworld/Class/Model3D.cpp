#include "Model3D.h"

Model3D::Model3D() {
    this->position = P6::MyVector(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    this->axis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->theta = glm::vec3(0.0f, 0.0f, 0.0f);
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Model3D::Model3D(MyVector position, glm::vec3 scale, glm::vec4 color) {
    this->position = position;
    this->scale = scale;
    this->axis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->theta = glm::vec3(0.0f, 0.0f, 0.0f);
    this->color = color;
}



std::vector<GLuint> Model3D::loadModel(std::string objSrc, tinyobj::attrib_t* attributes) {
    std::string path = objSrc;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    //tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
        
    return mesh_indices;
}

void  Model3D::bindBuffers(tinyobj::attrib_t attributes, std::vector<GLuint> mesh_indices, GLuint* VAO, GLuint* VBO) {
    GLuint EBO;
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), &mesh_indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void  Model3D::drawModel(std::vector<GLuint> mesh_indices, GLuint shaderProg, GLuint* VAO) {
    glm::mat4 transformation_matrix;

    //translate
    transformation_matrix = glm::translate(glm::mat4(1.0f),
        glm::vec3(this->position.x, this->position.y, this->position.z)
    );

    //scale
    transformation_matrix = glm::scale(transformation_matrix,
        glm::vec3(this->scale.x, this->scale.y, this->scale.z)
    );

    //rotate
    //X-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta.x),
        glm::normalize(glm::vec3(1, 0, 0))
    );

    //Y-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta.y),
        glm::normalize(glm::vec3(0, 1, 0))
    );

    //Z-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta.z),
        glm::normalize(glm::vec3(0, 0, 1))
    );

    glUseProgram(shaderProg);

    //transform
    unsigned int transformloc = glGetUniformLocation(shaderProg, "transform");
    glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    //color
    GLuint colorLoc = glGetUniformLocation  (shaderProg, "modelColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(this->color));

    //draw
    glBindVertexArray(*VAO);
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}

void Model3D::bindCamera(GLuint shaderProg, glm::mat4 projection) {
    unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Model3D::bindCamera(GLuint shaderProg, glm::mat4 projection, glm::mat4 viewMatrix) {
    unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void Model3D::setPosition(MyVector position) {
    this->position = position;
}

void Model3D::setScale(glm::vec3 scale) {
    this->scale = scale;
}

void Model3D::setColor(glm::vec4 color) {
    this->color = color;
}

