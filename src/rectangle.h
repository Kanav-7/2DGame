#include "main.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H


class Rectangle {
public:
    Rectangle() {}
    Rectangle(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bounding_box_r bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
