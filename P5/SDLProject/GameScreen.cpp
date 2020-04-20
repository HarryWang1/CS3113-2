//
//  GameScreen.cpp
//  Project 3
//
//  Created by Ethan Yao on 4/20/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "GameScreen.hpp"

#define LEVEL2_ENEMY_COUNT 1
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8


void GameScreen::Initialize()
{
    
    state.nextScene = -1;
}

void GameScreen::Update(float deltaTime)
{
    std::cout << "Hello" << std::endl;
}

void GameScreen::Render(ShaderProgram *program)
{
    std::cout << "Hello" << std::endl;
}

