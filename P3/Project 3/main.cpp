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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ShaderProgram program, program2;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, terrainMatrix, leftMatrix;

GLuint font;

SDL_Window* displayWindow;
bool gameIsRunning = true;

float rightPosY = 0.0f;
float leftPosY = 0.0f;

float landerPosX = 0.0f;
float landerPosY = 3.0f;

float landerSpeedX = 0.0005f;
float landerSpeedY = 0.0005f;

float landerAccelX = 0.0f;
float landerAccelY = 0.0f;

float ballRotateAngle = 0.0f;

GLuint LoadTexture(const char* filePath)
{
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL)
	{
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

	stbi_image_free(image);
	return textureID;
}

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	terrainMatrix = glm::mat4(1.0f);
	leftMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	program2.SetProjectionMatrix(projectionMatrix);
	program2.SetViewMatrix(viewMatrix);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint font = LoadTexture("textSheet.png");
}


void Shutdown() {
	SDL_Quit();
}


void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position)
{
	float width = 1.0f / 15.0f;
	float height = 1.0f / 15.0f;

	std::vector<float> vertices;
	std::vector<float> texCoords;

	for (int i = 0; i < text.size(); i++)
	{
		int index = (int)text[i];
		float offset = (size + spacing) * i;

		float u = (float)(index % 16) / 16.0f;
		float v = (float)(index / 16) / 16.0f;

		vertices.insert(vertices.end(),
			{
				offset + (-0.5f * size), 0.5f * size,
				offset + (-0.5f * size), -0.5f * size,
				offset + (0.5f * size), 0.5f * size,
				offset + (0.5f * size), -0.5f * size,
				offset + (0.5f * size), 0.5f * size,
				offset + (-0.5f * size), -0.5f * size
			});

		texCoords.insert(texCoords.end(),
			{
				u, v,
				u, v + height,
				u + width, v,
				u + width, v + height,
				u + width, v,
				u, v + height
			});
	}

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	glUseProgram(program->programID);
	
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, fontTextureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
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

		//ProcessInput();
		//Update();
		//Render();

		if (landerAccelX != 0) landerAccelX -= 0.0000001;

		glClear(GL_COLOR_BUFFER_BIT);
		program.SetProjectionMatrix(projectionMatrix);
		program.SetViewMatrix(viewMatrix);


		// Keyboard Controller
		const Uint8* keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_LEFT]) {
			landerAccelX += 0.0000005f;
			ballRotateAngle -= 0.25f;
		}
		if (keys[SDL_SCANCODE_RIGHT]) {
			landerAccelX -= 0.0000005f;
			ballRotateAngle += 0.25f;
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Terrain
		terrainMatrix = glm::mat4(1.0f);
		terrainMatrix = glm::translate(terrainMatrix, glm::vec3(0.0f, rightPosY, 0.0f));
		program.SetModelMatrix(terrainMatrix);

		float terrain[] = { -4.5f, -4.0f, 
							-2.5f, -1.0f,

							-2.5f, -1.0f,
							-0.5f, -3.0f,

							-0.5f, -3.0f,
							0.0f, -3.0f,

							0.0f, -3.0f,
							0.0f, -2.0f,

							0.0f, -2.0f,  // TARGET LINES
							1.0f, -2.0f,
				
							1.0f, -2.0f, 
							2.0f, -3.0f,

							2.0f, -3.0f,
							3.0f, -1.0f,

							3.0f, -1.0f,
							3.0f, 1.5f,

							3.0f, 1.5f,
							4.5f, 1.5f
						};


		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, terrain);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_LINES, 0, 18);
		glDisableVertexAttribArray(program.positionAttribute);


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Controls for Lander


		landerSpeedX += landerAccelX;
		landerSpeedY += landerAccelY;


		landerPosX += landerSpeedX;
		landerPosY -= landerSpeedY;

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(landerPosX, landerPosY, 0.0f)); 

		modelMatrix = glm::rotate(modelMatrix, 
										glm::radians(ballRotateAngle),
										glm::vec3(0.0f, 0.0f, 1.0f));

		program.SetModelMatrix(modelMatrix);

		float lander[] = { 
							-0.15f, -0.15f, 
							-0.05f, 0.15f, 
							-0.05f, -0.15f, 
							
							0.05f, 0.15f, 
							0.15f, -0.15f, 
							0.05f, -0.15f,

							-0.05f, -0.15f,
							-0.05f, 0.15f,
							0.05f, 0.15f,

							0.05f, 0.15f,
							-0.05f, -0.15f,
							0.05f, -0.15f
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, lander);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 12);

		glDisableVertexAttribArray(program.positionAttribute);

		GLuint font = LoadTexture("textSheet.png");
		DrawText(&program, font, "Victory", 0.5f, -0.25f, glm::vec3(-2.0f, 1.0f, 0.0f));

		SDL_GL_SwapWindow(displayWindow);
	}

	Shutdown();
	return 0;
}
