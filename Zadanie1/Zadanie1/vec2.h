#pragma once
struct vec2 {
    float x, y;
    vec2(float x_, float y_);
    vec2 add(vec2 other) const;
    vec2 sub(vec2 other) const;
    vec2 mul(float scalar) const;
    void add(vec2 other);
    void lowerLeft(vec2 other);
    void upperRight(vec2 other);
    void mul(float scalar);
    void mul(float xScalar, float yScalar);
    void normalise();
    float magnitude() const;
};