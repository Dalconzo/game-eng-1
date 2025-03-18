#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../System.h"
#include "../ECSManager.h"
#include "../components/Transform.h"
#include "../components/RenderComponent.h"
#include "../components/Camera.h"
#include <iostream>

class RenderSystem : public System {
public:
    RenderSystem() {
        requiredComponents = {
            ECSManager::getComponentTypeId(typeid(Transform)),
            ECSManager::getComponentTypeId(typeid(RenderComponent))
        };
    }

    void init(GLFWwindow* win) {
        if (!win) {
            std::cerr << "Error: GLFW window is null!" << std::endl;
            return;
        }
        window = win;
    
        // Make the window's context current
        glfwMakeContextCurrent(window);
        if (!glfwGetCurrentContext()) {
            std::cerr << "Error: Failed to make GLFW context current!" << std::endl;
            return;
        }
    
        // Initialize GLEW - this must happen AFTER making a context current
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
            return;
        }
    
        // Verify GLEW initialized OpenGL functions
        if (!glGenVertexArrays) {
            std::cerr << "Error: glGenVertexArrays is not available! GLEW initialization may have failed." << std::endl;
            return;
        }
    
        // Now safe to call OpenGL functions
        glViewport(0, 0, 800, 600);
        glEnable(GL_DEPTH_TEST);
    
        // Vertex Shader
        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            void main() {
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
    
        // Fragment Shader
        const char* fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Orange color for simplicity
            }
        )";
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
    
        // Shader Program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    
        // Simple triangle vertices
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.0f,  0.5f, -0.5f
        };
        glGenVertexArrays(1, &VAO); // Line 84
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void update(ECSManager* ecs) override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Find the camera entity
        Entity cameraEntity = -1;
        for (Entity e = 0; e < ecs->nextEntityId; ++e) {
            if (ecs->getComponent<Camera>(e) && ecs->getComponent<Transform>(e)) {
                cameraEntity = e;
                break;
            } else {
                std::cout << "Camera or Transform component not available! 1" << std::endl;
            }
        }
        if (cameraEntity == -1) {
            std::cout << "No camera entity found!" << std::endl;
            return;
        }

        // Get camera components
        Transform* camTransform = ecs->getComponent<Transform>(cameraEntity);
        Camera* cam = ecs->getComponent<Camera>(cameraEntity);
        if (!camTransform || !cam) {
            std::cout << "Camera or Transform component not available! 2" << std::endl;
            return;
        }
        glm::vec3 cameraPos = glm::vec3(camTransform->posX, camTransform->posY, camTransform->posZ);

        // Compute view and projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(cam->fov), cam->aspect, cam->nearPlane, cam->farPlane);
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Look at origin for now

        // Render all entities
        auto entities = ecs->getEntitiesWithComponents(requiredComponents);
        for (Entity entity : entities) {
            Transform* transform = ecs->getComponent<Transform>(entity);
            RenderComponent* render = ecs->getComponent<RenderComponent>(entity);
            if (transform && render) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(transform->posX, transform->posY, transform->posZ));
                model = model * glm::rotate(glm::mat4(1.0f), glm::radians(transform->rotX), glm::vec3(1, 0, 0));
                model = model * glm::rotate(glm::mat4(1.0f), glm::radians(transform->rotY), glm::vec3(0, 1, 0));
                model = model * glm::rotate(glm::mat4(1.0f), glm::radians(transform->rotZ), glm::vec3(0, 0, 1));
                model = glm::scale(model, glm::vec3(transform->scaleX, transform->scaleY, transform->scaleZ));

                glUseProgram(shaderProgram);
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 3); // Simple triangle for now
                glBindVertexArray(0);
            } else {
                std::cout << "Transform or RenderComponent not available!" << std::endl;
            }
        }

        glfwSwapBuffers(window);
    }

private:
    GLFWwindow* window;
    GLuint shaderProgram, VAO, VBO;
};

#endif // RENDER_SYSTEM_H