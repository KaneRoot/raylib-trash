#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_FPS 60
#define TEXT_SIZE 30
#define TEXT_COLOR WHITE

#define DELTA_TIME (1.0f / SCREEN_FPS)
#define POG_SCALING 1.0f
#define POG_VELOCITY 100.0f

#define TEXT_DURATION 5.0f
#define TEXT_SPACE 5.0f
#define TEXT_POSITION (Vector2) {100,100}

#define FILE_TEXTURE "./image.png"
#define FILE_SHADER  "./shader.fs"
#define FILE_FONT    "./font.ttf"

Font reload_font() { return LoadFont(FILE_FONT); }

void draw_some_text (Font font, double text_to_print_time, double current_time)
{
	if (text_to_print_time > current_time) {
		char msg[100];
		snprintf(msg, 100
			, "Text => still (%lf) sec"
			, text_to_print_time - current_time);
		DrawTextEx(font, msg, TEXT_POSITION, TEXT_SIZE, TEXT_SPACE, TEXT_COLOR);
	}
}

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Magnificient test.");

	Texture2D pog = LoadTexture(FILE_TEXTURE);
	Shader shader = LoadShader(0, TextFormat(FILE_SHADER, 330));

	int secondsLoc = GetShaderLocation(shader, "secondes");
	int freqXLoc   = GetShaderLocation(shader, "freqX");
	int freqYLoc   = GetShaderLocation(shader, "freqY");
	int ampXLoc    = GetShaderLocation(shader, "ampX");
	int ampYLoc    = GetShaderLocation(shader, "ampY");
	int speedXLoc  = GetShaderLocation(shader, "speedX");
	int speedYLoc  = GetShaderLocation(shader, "speedY");
	int sizeLoc    = GetShaderLocation(shader, "size");

	// Shader uniform values that can be updated at any time
	float freqX  = 25.0f;
	float freqY  = 25.0f;
	float ampX   = 5.0f;
	float ampY   = 5.0f;
	float speedX = 8.0f;
	float speedY = 8.0f;

	float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
	SetShaderValue(shader, sizeLoc,    &screenSize, SHADER_UNIFORM_VEC2);
	SetShaderValue(shader, freqXLoc,   &freqX,      SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, freqYLoc,   &freqY,      SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, ampXLoc,    &ampX,       SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, ampYLoc,    &ampY,       SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, speedXLoc,  &speedX,     SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, speedYLoc,  &speedY,     SHADER_UNIFORM_FLOAT);

	float seconds = 0.0f;

	SetTargetFPS(SCREEN_FPS);

	Vector2 pog_position = {0};
	Vector2 pog_velocity = {
		POG_VELOCITY * DELTA_TIME,
		POG_VELOCITY * DELTA_TIME
	};

	double current_time = 0.0f;

	Font font = reload_font();
	double text_to_print_time = 0.0f;

	while (!WindowShouldClose()) {
		seconds += GetFrameTime();

		current_time = GetTime();
		SetShaderValue(shader, secondsLoc, &seconds, SHADER_UNIFORM_FLOAT);


		if (pog_position.y + pog.height * POG_SCALING > SCREEN_HEIGHT ||
			pog_position.y < 0) {
			pog_velocity.y = -pog_velocity.y;
		}

		if (pog_position.x + pog.width * POG_SCALING > SCREEN_WIDTH ||
			pog_position.x < 0) {
			pog_velocity.x = -pog_velocity.x;
		}

		// In case we press the "x" button.
		if (IsKeyPressed(KEY_F)) {
			text_to_print_time = current_time + TEXT_DURATION;
			font = reload_font();
		}
		if (IsKeyPressed(KEY_S)) {
			font = reload_font();
		}
		if (IsKeyPressed(KEY_Q)) {
			return 0;
		}

		pog_position = Vector2Add(pog_position, pog_velocity);

		BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(600,20);

			BeginShaderMode(shader);
				DrawTextureEx(pog, pog_position, 0.0f, POG_SCALING, WHITE);
			EndShaderMode();

			DrawText("Hello this is me, Mario!", 20, 20, TEXT_SIZE, TEXT_COLOR);
			DrawText("'f' to reload font",   20, 460, TEXT_SIZE, TEXT_COLOR);
			DrawText("'s' to reload shader", 20, 500, TEXT_SIZE, TEXT_COLOR);
			DrawText("'q' to quit",          20, 540, TEXT_SIZE, TEXT_COLOR);
			draw_some_text (font, text_to_print_time, current_time);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
