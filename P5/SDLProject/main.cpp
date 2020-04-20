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

#include "Util.hpp"
#include "Entity.hpp"
#include "Map.hpp"
#include "Scene.hpp"

#include "GameScreen.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, terrainMatrix, leftMatrix;

SDL_Window* displayWindow;
bool gameIsRunning = true;

Scene *currentScene;
Scene *sceneList[4];

GLuint font;

void SwitchToScene(Scene *scene)
{
    currentScene = scene;
    currentScene->Initialize();
}

void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    int cols = 4;
    int rows = 4;
    
    float u = (float)(index % cols) / (float)cols;
    float v = (float)(index / cols) / (float)rows;
    
    float width = 1.0f / (float)cols;
    float height = 1.0f / (float)rows;
    
    float texCoords[] = {
        u, v + height,
        u + width, v + height,
        u + width, v,
        
        u, v + height,
        u + width, v,
        u, v
    };
    
    float vertices[] = {
        -0,5, -0.5,
        0.5, -0.5,
        0.5, 0.5,
        
        -0.5, -0.5,
        0.5, 0.5,
        -0.5, 0.5
    };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    
    Util::DrawText(&program, font, "Press and Hold Spacebar to Fly", 0.5f, -0.25f, glm::vec3(5.0f, 0.0f, 0.0f));
    Util::DrawText(&program, font, "Press 'F' to fire Flamespin", 0.5f, -0.25f, glm::vec3(5.0f, 1.0f, 0.0f));
    
    currentScene->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP)
    {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP && currentScene->state.player->isActive) //Checks to make sure player is
                                                                // still alive to keep enemy movement
    {
        currentScene->Update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5 || currentScene->state.player->position.x < 12)
    {
        viewMatrix = glm::translate(viewMatrix,
                                    glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y - 2.0, 0));
    }
    else
    {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
}

void ProcessInput()
{
    // Keyboard Controller
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT])
    {
        currentScene->state.player->movement.x += -0.005f;
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        currentScene->state.player->movement.x += 0.005f;
    }
    if (keys[SDL_SCANCODE_SPACE]) // Player's Flying ability
    {
        if (!currentScene->state.player->jump) currentScene->state.player->jump = true;
    }
    if (keys[SDL_SCANCODE_F])
    {
        if (!currentScene->state.player->shootFlamespin)
        {
            currentScene->state.player->shootFlamespin = true;
            
            currentScene->state.flamespin->position = currentScene->state.player->position;
            
            currentScene->state.flamespin->position.x = currentScene->state.player->position.x + currentScene->state.player->width; //sets flamespin to next
                                                                            //to player instead of on top of
            
            currentScene->state.flamespin->isActive = true;
        }
    }
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    terrainMatrix = glm::mat4(1.0f);
    leftMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 4.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    font = Util::LoadTexture("font.png");
    
    sceneList[0] = new GameScreen();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    SwitchToScene(sceneList[1]);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                gameIsRunning = false;
            }
        }
        
        ProcessInput();
        Update();
        
        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        
        Render();
    }
    
    Shutdown();
    return 0;
}
