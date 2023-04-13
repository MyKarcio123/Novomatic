#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include "vec2.h"
#include "Window.h"

struct vec3 {
    float r, g, b;
    vec3(float x_, float y_, float z_) :r(x_), g(y_), b(z_) {}
};
struct Triangle {
    const std::string fragmentShader =
        "#version 330 core\n"
        "uniform vec3 f_color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(f_color, 1.0);\n"
        "}\n";
    const std::string vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 vertexPosition;\n"
        "uniform vec2 translation;\n"
        "uniform float rotation;\n"
        "void main()\n"
        "{\n"
        "vec2 rotatedPosition;\n"
        "rotatedPosition.x = vertexPosition.x * cos(rotation) - vertexPosition.y * sin(rotation);\n"
        "rotatedPosition.y = vertexPosition.x * sin(rotation) + vertexPosition.y * cos(rotation);\n"
        "vec2 finalPosition = rotatedPosition + translation;\n"
        "gl_Position = vec4(finalPosition, 0.0, 1.0);\n"
        "}\n";

    std::array<vec2, 3> points;
    vec2 position;
    vec3 color;
    vec3 colidingColor;
    float doubleArea;
    float angle;
    float moveSpeed;
    float rotateSpeed;
    bool isColliding;

    GLuint VAO, VBO;
    GLuint shaderProgram, vertexShaderID, fragmentShaderID;
    GLint translationLocation, rotationLocation, colorLocation;

    Triangle(std::array<vec2, 3> points_, vec2 position_ = { 0.0f,0.0f }, float angle_ = 0.0f, vec3 colidingColor_ = { 1.0f,0.0f,0.0f }, vec3 color_ = { 1.0f,1.0f,1.0f });
    ~Triangle();
    vec2 getPoint(int index)const;
    void draw() const;
    void move();
    void move(float x, float y);
    void rotate(float angle);
private:
    void checkShaderCompilation(GLuint shaderID) const;
    void checkProgramLinking(GLuint programID);
};