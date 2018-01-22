#include "triangle.h"
#include "main.h"

Triangle::Triangle(float x, float y,color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = -0.01;
    static GLfloat vertex_buffer_data[100] ;
       vertex_buffer_data[0] =0.2f;
       vertex_buffer_data[1] = 0;
       vertex_buffer_data[2] = 0; // vertex 1
        vertex_buffer_data[3] = -0.2f;
        vertex_buffer_data[4] = 0;
        vertex_buffer_data[5] = 0; // vertex 2
        vertex_buffer_data[6] =  0 ;
        vertex_buffer_data[7] = 0.4f;
        vertex_buffer_data[8] = 0; // vertex 3


    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color, GL_FILL);
}

void Triangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Triangle::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Triangle::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_r Triangle::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_r bbox = { x, y, 0 ,0 };
    return bbox;
}
