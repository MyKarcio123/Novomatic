#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <memory>
#include <math.h>
#include <vector>
#include "vec2.h"
#include "Window.h"
#include "Triangle.h"

#define uniqueTriangle std::unique_ptr<Triangle>
int HEIGHT = 480;
int WIDTH = 640;
bool isBoxColliding(const Triangle& a, const Triangle& b) {
    //Triangle a boundary box:
    vec2 aLowerLeft = a.getPoint(0);
    vec2 aUpperRight = a.getPoint(0);
    
    //Triangle b boundary box:
    vec2 bLowerLeft = b.getPoint(0);
    vec2 bUpperRight = b.getPoint(0);
    
    for (int i = 0; i < 2; ++i) {
        aLowerLeft.lowerLeft(a.getPoint(i+1));
        aUpperRight.upperRight(a.getPoint(i + 1));
        bLowerLeft.lowerLeft(b.getPoint(i + 1));
        bUpperRight.upperRight(b.getPoint(i + 1));
    }
    
    //checkForCollision:

    if (aLowerLeft.x > bUpperRight.x) {
        return false;
    }
    if (aUpperRight.x < bLowerLeft.x) {
        return false;
    }
    if (aLowerLeft.y > bUpperRight.y) {
        return false;
    }
    if (aUpperRight.y < bLowerLeft.y) {
        return false;
    }
    return true;
}
float det(vec2 first, vec2 second) {
    return first.x * second.y - first.y * second.x;
}
bool checkForTriangleInsideOther(const vec2 p1, const vec2 p2, const vec2 p3, const vec2 p4) {
    float d1 = det(p3.sub(p2), p1.sub(p2));
    float d2 = det(p4.sub(p3), p1.sub(p3));
    float d3 = det(p2.sub(p4), p1.sub(p4));
    return(d1 > 0 && d2 > 0 && d3 > 0);
}
bool isTriangleInsideOther(const Triangle& a, const Triangle& b) {
    if (a.doubleArea < b.doubleArea) return checkForTriangleInsideOther(a.getPoint(0), b.getPoint(0), b.getPoint(1), b.getPoint(2));
    return checkForTriangleInsideOther(b.getPoint(0), a.getPoint(0), a.getPoint(1), a.getPoint(2));
}
bool isLineIntersecting(const vec2 p1,const vec2 p2,const vec2 p3,const vec2 p4) {
    float denominator = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
    if (denominator == 0) return false;
    float u_a = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / denominator;
    float u_b = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / denominator;

    if (u_a >= 0 && u_a <= 1 && u_b >= 0 && u_b <= 1)
    {
        return true;
    }
    return false;
}
bool isColliding(const Triangle& a,const Triangle& b) {
    if (!isBoxColliding(a, b)) return false;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (isLineIntersecting(a.getPoint(i), a.getPoint((i + 1) % 3), b.getPoint(j), b.getPoint((j + 1) % 3))) {
                return true;
            }
            if (isTriangleInsideOther(a, b)) {
                return true;
            }
        }
    }
    return false;
}
float randFloat() {
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return (2.0f * r - 1.0f); 
}
int main(void)
{
    std::srand(std::time(nullptr));

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
    Window::window = window;
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    std::vector<uniqueTriangle> colliders;
    int collidersToGenerate = rand() % 5 + 4;
    for (float i = 0; i < collidersToGenerate; ++i) {
        uniqueTriangle collider(new Triangle({ vec2{-0.1f, -0.2f},vec2{0.3f, -0.4f},vec2{0.0f, 0.5f} }, vec2{ randFloat() ,randFloat() }, rand() % 180, vec3{ 0.0f,1.0f,0.0f }));
        colliders.push_back(std::move(collider));
    }
    // Set up the triangle
    Triangle triangle({ vec2{ -0.05f, -0.05f },vec2{ 0.05f, -0.05f },vec2{ 0.0f, 0.05f } });
    

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        triangle.move();
        triangle.isColliding = false;

        colliders[0]->rotate(0.001f);
        colliders[1]->rotate(-0.002f);

        
        for (uniqueTriangle& collider : colliders) {
            if (isColliding(triangle, *collider)) {
                triangle.isColliding = true;
                collider->isColliding = true;
            }
            else {
                collider->isColliding = false;
            }
            collider->draw();
        }
        triangle.draw();
   
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return EXIT_SUCCESS;
}
