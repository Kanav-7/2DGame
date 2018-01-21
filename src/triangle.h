#include "main.h"

#ifndef TRIANGLE_H
#define TRIANGLE_H


class Triangle {
public:
    Triangle() {}
    Triangle(float x, float y, color_t color);
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
