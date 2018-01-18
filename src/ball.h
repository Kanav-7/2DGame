#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y,float r,double vx,double vy, color_t color);
    glm::   vec3 position;
    float rotation;
    float radius;
    double speedx;
    double speedy;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void set_radius(float r);
    void set_speed(double vx,double vy);
    void tick();
    void move(float x,float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
