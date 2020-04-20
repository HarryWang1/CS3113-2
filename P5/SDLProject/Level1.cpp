//
//  Level1.cpp
//  Project 3
//
//  Created by Ethan Yao on 4/19/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level1.hpp"

#define LEVEL1_ENEMY_COUNT 1
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

unsigned int level1_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize()
{
    state.nextScene = -1;
    
    // Setup Player Values
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5.0, 1.0, 0.0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0.0, -9.81, 0.0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 0.2f;
    state.player->jumpPower = 0.25f;
    state.player->textureID = Util::LoadTexture("playerWizard.png");
    
    
    
    
    // Setup Player's Flamespin Projectile Values
//    state.flamespin = new Entity();
//    state.flamespin->isActive = false;
//    state.flamespin->entityType = ATTACKOBJECT;
//    state.flamespin->position = state.player->position;
//    state.flamespin->movement = glm::vec3(1.0, 0.0, 0.0);
//    state.flamespin->velocity = glm::vec3(1.0, 0.0, 0.0);
//    state.flamespin->speed = 2.0f;
//    state.flamespin->textureID = Util::LoadTexture("flamespinRight.png");
    
    
    
    // Setup Platform
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    
    
    // Setup Enemies
//    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
//
//    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) // Generates the 3 enemies and sets defaults
//    {
//        state.enemies[i].entityType = ENEMY;
//        state.enemies[i].aiType = WALKER;
//        state.enemies[i].aiState = IDLE;
//        state.enemies[i].position = glm::vec3(10.0, 5.0, 0.0);
//        state.enemies[i].acceleration = glm::vec3(0.0, -9.81, 0.0);
//        state.enemies[i].speed = 1.0;
//    }
//
//    state.enemies[0].position = glm::vec3(5.0, 1.0, 0.0);
    
    state.enemies = new Entity;
    state.enemies->entityType = ENEMY;
    state.enemies->aiType = WALKER;
    state.enemies->aiState = IDLE;
    state.enemies->position = glm::vec3(12.0, 1.0, 0.0);
    state.enemies->acceleration = glm::vec3(0.0, -9.81, 0.0);
    state.enemies->speed = 1.0;
    
    state.enemies->isActive = true;
    
//    state.enemies[1].isActive = false;
//    state.enemies[1].position.y = 1.0;
//    state.enemies[1].aiType = JUMPER;
//
//    state.enemies[2].isActive = false;
//    state.enemies[2].aiType = WITCH;
    
    state.enemies->textureID = Util::LoadTexture("enemyPerson.png");
//    state.enemies[1].textureID = Util::LoadTexture("enemyVampire.png");
//    state.enemies[2].textureID = Util::LoadTexture("enemyWitch.png");
    
    // Setup Witch's Fireball Projectile Values
//    state.fireball = new Entity();
//    state.fireball->isActive = false;
//    state.fireball->entityType = ATTACKOBJECT;
//    state.fireball->position = state.enemies[2].position;
//    state.fireball->movement = glm::vec3(1.0, 0.0, 0.0);
//    state.fireball->velocity = glm::vec3(1.0, 0.0, 0.0);
//    state.fireball->speed = -2.0f;
//    state.fireball->textureID = Util::LoadTexture("fireballLeft.png");
}

void Level1::Update(float deltaTime)
{
    state.player->Update(deltaTime, state.player, state.enemies, 0, state.map);
    
    state.enemies->Update(deltaTime, state.player, state.enemies, 0, state.map);
    
    if (state.enemies->isActive == false)
    {
        state.nextScene = 1;
    }
    
    if (state.player->collidedLeft || state.player->collidedRight) //Check collision death
    {
        state.player->isActive = false;
        state.player->isWin = false;
    }
    
    //        for (int i = 0; i < ENEMY_COUNT; i++)
    //        {
    //            if (state.enemies[i].isActive) //If enemy is equal to level create
    //            {
    //                state.enemies[i].Update(FIXED_TIMESTEP, state.player, &state.enemies[i], state.fireball, state.map);
    //            }
    //
    //            if (state.enemies[i].shootFireball) //If statement check for witch fireball
    //            {
    //                state.fireball->Update(FIXED_TIMESTEP, &state.enemies[i], state.player, state.fireball, state.map, GROUND_COUNT);
    //
    //                if (glm::distance(state.fireball->position, state.player->position) > 4.0) //removes fireball
    //                                                                                    //if out of distance
    //                {
    //                    state.fireball->isActive = false;
    //                    state.enemies[i].shootFireball = false;
    //                }
    //
    //                if (!state.fireball->isActive && !state.player->isActive) //removes fireball if player killed
    //                {
    //                    state.enemies[i].shootFireball = false;
    //                }
    //            }
    //
    //            if (state.flamespin->isActive) //If statement check for player flamespin
    //            {
    //                state.flamespin->Update(FIXED_TIMESTEP, state.player, &state.enemies[i], state.fireball, state.map);
    //
    //                if (glm::distance(state.flamespin->position, state.player->position) > 4.0) //removes flamespin
    //                                                                                        //if out of distance
    //                {
    //                    state.flamespin->isActive = false;
    //                    state.player->shootFlamespin = false;
    //                }
    //
    //                if (!state.flamespin->isActive && !state.enemies[i].isActive)
    //                {
    //                    state.player->shootFlamespin = false; // Removes flamespin if enemy is killed by it
    //                }
    //            }
    //
    //            state.enemies[state.player->enemiesKilled].isActive = true; //Only generates enemy of each level
    //        }
}

void Level1::Render(ShaderProgram *program)
{
    state.map->Render(program);
    
    state.enemies->Render(program);
    
    state.player->Render(program); //Render Player
    
//    if (state.flamespin->isActive) //Render Flamespin if triggered
//    {
//        state.flamespin->Render(program);
//    }
//
//    if (state.fireball->isActive) //Render fireball if triggered
//    {
//        state.fireball->Render(program);
//    }
//
//    if (state.player->isActive) //Draw Game Over
//    {
//        Util::DrawText(program, font, "You Lose", 0.5f, -0.25f, glm::vec3(5.0f, 16.0f, 0.0f));
//    }
//    else if (state.player->enemiesKilled == 3) //Draw You Win
//    {
//        Util::DrawText(program, font, "You Win", 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0.0f));
//    }
//    else //If still in game draw instructions
//    {
//        Util::DrawText(program, font, "Press and Hold Spacebar to Fly", 0.5f, -0.25f, glm::vec3(-4.0f, 3.0f, 0.0f));
//        Util::DrawText(program, font, "Press 'F' to fire Flamespin", 0.5f, -0.25f, glm::vec3(-4.0f, 2.0f, 0.0f));
//    }
}
