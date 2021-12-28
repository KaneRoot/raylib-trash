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
#define TEXT_SPACE 0.0f
#define TEXT_POSITION1 (Vector2) { 20, 20  }
#define TEXT_POSITION2 (Vector2) { 20, 460 }
#define TEXT_POSITION3 (Vector2) { 20, 500 }
#define TEXT_POSITION4 (Vector2) { 20, 540 }

#define FILE_TEXTURE "./image.png"
#define FILE_SHADER  "./shader.fs"
#define FILE_FONT    "./font.ttf"

Font reload_font() { return LoadFont(FILE_FONT); }

#if 0
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
#endif

Shader reload_shader(
	  float *ptr_freqX
	, float *ptr_freqY
	, float *ptr_ampX
	, float *ptr_ampY
	, float *ptr_speedX
	, float *ptr_speedY
	, float *ptr_screenSize[2])
{

	Shader shader = LoadShader(0, TextFormat(FILE_SHADER, 330));
	int freqXLoc   = GetShaderLocation(shader, "freqX");
	int freqYLoc   = GetShaderLocation(shader, "freqY");
	int ampXLoc    = GetShaderLocation(shader, "ampX");
	int ampYLoc    = GetShaderLocation(shader, "ampY");
	int speedXLoc  = GetShaderLocation(shader, "speedX");
	int speedYLoc  = GetShaderLocation(shader, "speedY");
	int sizeLoc    = GetShaderLocation(shader, "size");

	SetShaderValue(shader, freqXLoc,   ptr_freqX,      SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, freqYLoc,   ptr_freqY,      SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, ampXLoc,    ptr_ampX,       SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, ampYLoc,    ptr_ampY,       SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, speedXLoc,  ptr_speedX,     SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, speedYLoc,  ptr_speedY,     SHADER_UNIFORM_FLOAT);
	SetShaderValue(shader, sizeLoc,    ptr_screenSize, SHADER_UNIFORM_VEC2);

	return shader;
}

int main(void)
{

	SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(
		  FLAG_WINDOW_UNDECORATED
		| FLAG_WINDOW_UNFOCUSED
		| FLAG_WINDOW_TOPMOST
		| FLAG_WINDOW_TRANSPARENT
		// | FLAG_MSAA_4X_HINT
		);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Magnificient test.");

	Texture2D pog = LoadTexture(FILE_TEXTURE);

	// Shader uniform values that can be updated at any time
	float freqX  = 25.0f;
	float freqY  = 25.0f;
	float ampX   = 5.0f;
	float ampY   = 5.0f;
	float speedX = 8.0f;
	float speedY = 8.0f;
	float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };

	Shader shader = reload_shader(
		  &freqX
		, &freqY
		, &ampX
		, &ampY
		, &speedX
		, &speedY
		, &screenSize);

	int secondsLoc = GetShaderLocation(shader, "secondes");

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
			shader = reload_shader(
				  &freqX, &freqY
				, &ampX, &ampY
				, &speedX, &speedY
				, &screenSize);
		}
		if (IsKeyPressed(KEY_Q)) {
			return 0;
		}

		pog_position = Vector2Add(pog_position, pog_velocity);

		BeginDrawing();
			ClearBackground(BLANK);
			DrawFPS(600,20);

			BeginShaderMode(shader);
				DrawTextureEx(pog, pog_position, 0.0f, POG_SCALING, WHITE);
			EndShaderMode();

			DrawTextEx (font, "Hello this is me, Mario!", TEXT_POSITION1, TEXT_SIZE, TEXT_SPACE, TEXT_COLOR);
			DrawTextEx (font, "'f' to reload font",       TEXT_POSITION2, TEXT_SIZE, TEXT_SPACE, TEXT_COLOR);
			DrawTextEx (font, "'s' to reload shader",     TEXT_POSITION3, TEXT_SIZE, TEXT_SPACE, TEXT_COLOR);
			DrawTextEx (font, "'q' to quit",              TEXT_POSITION4, TEXT_SIZE, TEXT_SPACE, TEXT_COLOR);
			// draw_some_text (font, text_to_print_time, current_time);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
