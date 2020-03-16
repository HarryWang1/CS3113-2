//
//  Entity.hpp
//  SDLProject
//
//  Created by Ethan Yao on 3/15/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <vector>

class Entity
{
public:
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    float speed;
    float accelerationX;
    
    float rotateAngle;
    
    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    int animCols = 0;
    int animRows = 0;
    
    float width = 1.0;
    float height = 1.0;
    
    bool isActive = true;
    bool isWin = false;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    Entity();
    
    void Update(float deltaTime, Entity* rocks, Entity* landing, int rockCount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
};

#endif /* Entity_hpp */
