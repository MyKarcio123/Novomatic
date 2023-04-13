#include "vec2.h"
#include <math.h>

vec2::vec2(float x_, float y_) : x(x_), y(y_) {}

vec2 vec2::add(vec2 other) const {
    return vec2(x + other.x, y + other.y);
}

vec2 vec2::sub(vec2 other) const {
    return vec2(x - other.x, y - other.y);
}

vec2 vec2::mul(float scalar) const {
    return vec2(x * scalar, y * scalar);
}

void vec2::add(vec2 other) {
    this->x += other.x;
    this->y += other.y;
}

void vec2::lowerLeft(vec2 other) {
    this->x = fmin(this->x, other.x);
    this->y = fmin(this->y, other.y);
}

void vec2::upperRight(vec2 other) {
    this->x = fmax(this->x, other.x);
    this->y = fmax(this->y, other.y);
}

void vec2::mul(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
}

void vec2::mul(float xScalar, float yScalar) {
    this->x *= xScalar;
    this->y *= yScalar;
}

void vec2::normalise() {
    float mag = this->magnitude();
    x = x / mag;
    y = y / mag;
}

float vec2::magnitude() const {
    return sqrt((x * x + y * y));
}