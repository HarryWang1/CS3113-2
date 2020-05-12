//
//  Level2.cpp
//  Project 3
//
//  Created by Ethan Yao on 5/11/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level2.hpp"

void Level2::Initialize()
{
    state.nextScene = -1;
}

void Level2::Update(float deltaTime)
{
    std::cout << "Hello" << std::endl;
}

void Level2::Render(ShaderProgram *program)
{
    std::cout << "Hello" << std::endl;
}

void Level2::Reset()
{
    Level2::Initialize();
}
