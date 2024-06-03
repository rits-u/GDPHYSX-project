#include "Model3D.h"

Model3D::Model3D() {
    this->pos_x = 0.0f;
    this->pos_y = 0.0f;
    this->pos_z = 0.0f;
    this->scale_x = 1.0f;
    this->scale_y = 1.0f;
    this->scale_z = 1.0f;
    this->axis_x = 0.0f;
    this->axis_y = 1.0f;
    this->axis_z = 0.0f;
    this->theta_x = 0.0f;
    this->theta_y = 0.0f;
    this->theta_z = 0.0f;
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
        glm::vec3(this->pos_x, this->pos_y, this->pos_z)
    );

    //scale
    transformation_matrix = glm::scale(transformation_matrix,
        glm::vec3(this->scale_x, this->scale_y, this->scale_z)
    );

    //rotate
    //X-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_x),
        glm::normalize(glm::vec3(1, 0, 0))
    );

    //Y-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_y),
        glm::normalize(glm::vec3(0, 1, 0))
    );

    //Z-AXIS
    transformation_matrix = glm::rotate(transformation_matrix,
        glm::radians(this->theta_z),
        glm::normalize(glm::vec3(0, 0, 1))
    );

    glUseProgram(shaderProg);

    //transform
    unsigned int transformloc = glGetUniformLocation(shaderProg, "transform");
    glUniformMatrix4fv(transformloc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

    //draw
    glBindVertexArray(*VAO);
    glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
}

void  Model3D::bindCamera(GLuint shaderProg, glm::mat4 projection, glm::mat4 viewMatrix) {
    unsigned int projLoc = glGetUniformLocation(shaderProg, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void Model3D::setPosition(float pos_x, float pos_y, float pos_z) {
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->pos_z = pos_z;
}

void Model3D::setScale(float scale_x, float scale_y, float scale_z) {
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->scale_z = scale_z;
}


//GETTERS AND SETTERS
//position
float Model3D::getPos_X() {
    return this->pos_x;
}

float Model3D::getPos_Y() {
    return this->pos_y;
}

float Model3D::getPos_Z() {
    return this->pos_z;
}

void Model3D::setPos_X(float pos_x) {
    this->pos_x = pos_x;
}

void Model3D::setPos_Y(float pos_y) {
    this->pos_y = pos_y;
}

void Model3D::setPos_Z(float pos_z) {
    this->pos_z = pos_z;
}

//SCALE
float Model3D::getScale_X() {
    return this->scale_x;
}

float Model3D::getScale_Y() {
    return this->scale_y;
}

float Model3D::getScale_Z() {
    return this->scale_z;
}

void Model3D::setScale_X(float scale_x)
{
    this->scale_x = scale_x;
}

void Model3D::setScale_Y(float scale_y)
{
    this->scale_y = scale_y;
}

void Model3D::setScale_Z(float scale_z)
{
    this->scale_z = scale_z;
}

//AXIS
float Model3D::getAxis_X() {
    return this->axis_x;
}

float Model3D::getAxis_Y() {
    return this->axis_y;
}

float Model3D::getAxis_Z() {
    return this->axis_z;
}

void Model3D::setAxis_X(float axis_x) {
    this->axis_x = axis_x;
}

void Model3D::setAxis_Y(float axis_y) {
    this->axis_y = axis_y;
}

void Model3D::setAxis_Z(float axis_z) {
    this->axis_z = axis_z;
}

//THETA
float Model3D::getTheta_X() {
    return this->theta_x;
}

float Model3D::getTheta_Y() {
    return this->theta_y;
}

float Model3D::getTheta_Z() {
    return this->theta_z;
}

void Model3D::setTheta_X(float theta_x) {
    this->theta_x = theta_x;
}

void Model3D::setTheta_Y(float theta_y) {
    this->theta_y = theta_y;
}

void Model3D::setTheta_Z(float theta_z) {
    this->theta_z = theta_z;
}
