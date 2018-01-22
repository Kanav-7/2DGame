#include "main.h"

#ifndef SEMI_H
#define SEMI_H


class Semi {
public:
    Semi() {}
    Semi(float x, float y,float r,float rot,color_t color);
    glm::   vec3 position;
    float rotation;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_radius(float r);
//    void set_speed(double vx,double vy);
    void tick();
    void move(float x,float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
