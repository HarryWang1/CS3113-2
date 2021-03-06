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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 viewMatrix2, modelMatrix2, projectionMatrix2;

GLuint ctgTexture;
GLuint diceTexture;
float lastTicks = 0.0f;

GLuint LoadTexture(const char* filePath)
{
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL)
	{
		std::cout << "Unable to load image." << std::endl;
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}

/*
//When models were loaded in Render() it doesn't load so it was moved to the game loop
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
	program.SetModelMatrix(modelMatrix);

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, ctgTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);


	SDL_GL_SwapWindow(displayWindow);
}

void Update()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;
}

void Initialize()
{
	float rotate_z = 0.0f;
}

*/

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("First Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 640);

	ShaderProgram program;
	ShaderProgram program2;

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	program2.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	ctgTexture = LoadTexture("ctg.png");
	diceTexture = LoadTexture("diceRoll.png");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	float rotate_z = 45.0f;
	float player_x = -1.0f;

	glm::mat4 modelMatrix = glm::mat4(1.0f);	 
	glm::mat4 viewMatrix = glm::mat4(1.0f);	

	glm::mat4 modelMatrix2 = glm::mat4(1.0f);
	glm::mat4 viewMatrix2 = glm::mat4(1.0f);

	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -3.75f, 3.75f); 
	projectionMatrix2 = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -3.75f, 3.75f);

	program.SetModelMatrix(modelMatrix);
	program.SetProjectionMatrix(projectionMatrix); 
	program.SetViewMatrix(viewMatrix);

	program2.SetModelMatrix(modelMatrix2);
	program2.SetProjectionMatrix(projectionMatrix2);
	program2.SetViewMatrix(viewMatrix2);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	SDL_Event event;
	while (gameIsRunning)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				gameIsRunning = false;
			}
		}

		//Initialize();
		//Render();
		//Update();

		glClear(GL_COLOR_BUFFER_BIT);

		//Model 1
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate_z), glm::vec3(1.0f, 1.0f, 1.0f));
		program.SetModelMatrix(modelMatrix);

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glBindTexture(GL_TEXTURE_2D, ctgTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Model 2

		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(player_x, 0.0f, 0.0f)); 
		program2.SetModelMatrix(modelMatrix2); 
		glVertexAttribPointer(program2.positionAttribute, 2, GL_FLOAT, false, 0, vertices); 
		glEnableVertexAttribArray(program2.positionAttribute); 
		glVertexAttribPointer(program2.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program2.texCoordAttribute);

		glBindTexture(GL_TEXTURE_2D, diceTexture); 
		glDrawArrays(GL_TRIANGLES, 0, 6); 				

		glDisableVertexAttribArray(program2.positionAttribute); 											
		glDisableVertexAttribArray(program2.texCoordAttribute); 

		//Tick timer
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float deltaTime = ticks - lastTicks;
		lastTicks = ticks;

		rotate_z += 45.0f * deltaTime;	//Spinning controller
		player_x += 1.0f * deltaTime; //X-Movement controller

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
