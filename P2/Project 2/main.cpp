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

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, modelLeftMatrix, modelRightMatrix;

float lastTicks = 0.0f;

SDL_Window* displayWindow;
bool gameIsRunning = true;

float leftBarPosY = 0.0f;
float rightBarPosY = 0.0f;

float ballPosX = 0.0f;
float ballPosY = 0.0f;

float ballSpeed = 0.00005f;


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	modelLeftMatrix = glm::mat4(1.0f);
	modelRightMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.75f, 3.75f, -1.0f, 1.0f);


	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(program.programID);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

/*
void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			gameIsRunning = false;
		}
	}
}

void Update() { }

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	float leftBar[] = { 3.5f, -0.5f, 3.4f, 0.5f, 3.5f, 0.5f, 3.4f, 0.5f, 3.5f, -0.5f, 3.4f, -0.5f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftBar);
	glEnableVertexAttribArray(program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	program.SetModelMatrix(modelMatrix);

	float rightBar[] = { -3.5f, -0.5f, -3.4f, 0.5f, -3.5f, 0.5f, -3.4f, 0.5f, -3.5f, -0.5f, -3.4f, -0.5f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightBar);
	glEnableVertexAttribArray(program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	program.SetModelMatrix(modelMatrix);

	float ball[] = { -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
	glEnableVertexAttribArray(program.positionAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program.positionAttribute);


	SDL_GL_SwapWindow(displayWindow);
}


*/

void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		//ProcessInput();
		//Update();
		//Render();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				gameIsRunning = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float deltaTime = ticks - lastTicks;
		lastTicks = ticks;

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_W]) {
			rightBarPosY += 0.001;
		}
		if (keys[SDL_SCANCODE_S]) {
			rightBarPosY -= 0.001;
		}
		if (keys[SDL_SCANCODE_UP]) {
			leftBarPosY += 0.001;
		}
		if (keys[SDL_SCANCODE_DOWN]) {
			leftBarPosY -= 0.001;
		}

		modelLeftMatrix = glm::translate(modelLeftMatrix, glm::vec3(0.0f, leftBarPosY, 0.0f));
		program.SetModelMatrix(modelLeftMatrix);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);

		glUseProgram(program.programID);


		float leftBar[] = { 3.5f, -0.5f, 3.4f, 0.5f, 3.5f, 0.5f, 3.4f, 0.5f, 3.5f, -0.5f, 3.4f, -0.5f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftBar);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);

		modelRightMatrix = glm::translate(modelRightMatrix, glm::vec3(0.0f, rightBarPosY, 0.0f));
		program.SetModelMatrix(modelRightMatrix);

		float rightBar[] = { -3.5f, -0.5f, -3.4f, 0.5f, -3.5f, 0.5f, -3.4f, 0.5f, -3.5f, -0.5f, -3.4f, -0.5f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightBar);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(program.positionAttribute);



		if (ballPosY <= -0.0135 || ballPosY >= 0.0135)
		{
			ballSpeed = -ballSpeed;
			ballPosX = 0.0f;
		}

		ballPosX += ballSpeed;
		ballPosY += ballSpeed;

		modelMatrix = glm::translate(modelMatrix, glm::vec3(ballPosX, ballPosY, 0.0f));
		program.SetModelMatrix(modelMatrix);

		float ball[] = { -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);


		SDL_GL_SwapWindow(displayWindow);
	}

	Shutdown();
	return 0;
}