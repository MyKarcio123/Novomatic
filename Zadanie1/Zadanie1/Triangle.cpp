#pragma once
#include "Triangle.h"

Triangle::Triangle(std::array<vec2, 3> points_, vec2 position_, float angle_, vec3 colidingColor_, vec3 color_)
    : points(points_), position(position_), angle(angle_), color(color_), colidingColor(colidingColor_)
{
    doubleArea = (points[1].x - points[0].x) * (points[2].y - points[0].y) - (points[2].x - points[0].x) * (points[1].y - points[0].y);
    moveSpeed = 0.0005f;
    rotateSpeed = 0.001f;

    //create and compile shaders
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCode = vertexShader.c_str();
    glShaderSource(vertexShaderID, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShaderID);
    checkShaderCompilation(vertexShaderID);

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCode = fragmentShader.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShaderID);
    checkShaderCompilation(fragmentShaderID);

    //create and link shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    //get uniform locations
    translationLocation = glGetUniformLocation(shaderProgram, "translation");
    rotationLocation = glGetUniformLocation(shaderProgram, "rotation");
    colorLocation = glGetUniformLocation(shaderProgram, "f_color");

    //generate and bind VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Triangle::~Triangle()
{
    //delete VAO and VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //delete shader program and shaders
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}
vec2 Triangle::getPoint(int index)const {
    vec2 p = points[index];
    float x = p.x * cos(angle) - p.y * sin(angle);
    float y = p.x * sin(angle) + p.y * cos(angle);
    vec2 newPoint(x, y);
    newPoint.add(position);
    newPoint.add(vec2(1, 1));
    newPoint.mul(320, 240);
    return newPoint;
}
void Triangle::draw() const {
    //use shader program
    glUseProgram(shaderProgram);

    //set uniforms
    glUniform2f(translationLocation, position.x, position.y);
    glUniform1f(rotationLocation, angle);
    if (isColliding)
        glUniform3f(colorLocation, colidingColor.r, colidingColor.g, colidingColor.b);
    else
        glUniform3f(colorLocation, color.r, color.g, color.b);

    //draw triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
void Triangle::move() {
    vec2 moveDir = { 0.0f,0.0f };
    if (glfwGetKey(Window::window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        moveDir.x = 1.0f;
    }
    if (glfwGetKey(Window::window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        moveDir.x = -1.0f;
    }
    if (glfwGetKey(Window::window, GLFW_KEY_UP) == GLFW_PRESS) {
        moveDir.y = 1.0f;
    }
    if (glfwGetKey(Window::window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        moveDir.y = -1.0f;
    }
    if (!(moveDir.x == 0 && moveDir.y == 0)) {
        moveDir.normalise();
        moveDir.mul(this->moveSpeed);
        move(moveDir.x, moveDir.y);
    }
    if (glfwGetKey(Window::window, GLFW_KEY_D) == GLFW_PRESS) {
        rotate(-rotateSpeed);
    }
    if (glfwGetKey(Window::window, GLFW_KEY_A) == GLFW_PRESS) {
        rotate(rotateSpeed);
    }
}
void Triangle::move(float x, float y) {
    position.x += x;
    position.y += y;
}
void Triangle::rotate(float angle) {
    this->angle += angle;
}
void Triangle::checkShaderCompilation(GLuint shaderID) const
{
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error:\n" << infoLog << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
void Triangle::checkProgramLinking(GLuint programID)
{
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Program linking error:\n" << infoLog << std::endl;
        std::exit(EXIT_FAILURE);
    }
}