#ifndef SCORE_H
#define SCORE_H

#include "digit.h"

class Score
{
public:
    Score();
    void draw(glm::mat4 VP);
    void update(int score);
    void add(int d);
    void subtract(int d);
private:
    int score, digslen;
    Digit digs[50];
};

#endif // SCORE_H
