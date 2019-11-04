#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

/*---------- Global engine variables. ----------*/
bool g_bRunning = false;
int g_iSpeed = 5;				// Speed of the box actor.
const Uint8* g_iKeystates;		// Keyboard state container.
Uint32 g_start, g_end, g_delta, g_fps;
SDL_Window* g_pWindow;			// This represents the SDL window.
SDL_Renderer* g_pRenderer;		// This represents the buffer to draw to.

/*---------- Globals for sprite. ----------*/
SDL_Texture* g_pTexture;		// Pixel map for the source image.
SDL_Rect g_src, g_dst;			// Rectangle objects.

bool init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	/*---------- Initialize SDL. ----------*/
	cout << "Initializing game." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)	// Everything is okay.
	{
		/*---------- Create the window. ----------*/
		g_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (g_pWindow != nullptr)
		{
			/*---------- Initialize subsystems. ----------*/
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
			if (g_pRenderer != nullptr)
			{
				/*---------- Init png. ----------*/
				if (IMG_Init(IMG_INIT_PNG))
				{
					g_pTexture = IMG_LoadTexture(g_pRenderer, "ship.png");
				}
				else return false; // IMG init fail.
			}
			else return false;	// Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false;	//=	===	===	===	===	=> Everything is not okay.
	/*---------- Everything is okay. Start engine. ----------*/
	g_fps = (Uint32)round((1 / (double)FPS) * 1000);
	g_iKeystates = SDL_GetKeyboardState(nullptr);
	
	// Create the spring rectangles.
	g_src = { 0,0,154,221 };
	g_dst = {width/2 - g_src.w/6, height - g_src.h/2, g_src.w/3, g_src.h/3 };

	g_bRunning = true;
	cout << "Success!" << endl;
	return true;
}

void wake()
{
	g_start = SDL_GetTicks();
}

void sleep()
{
	g_end = SDL_GetTicks();
	g_delta = g_end - g_start;
	if (g_delta < g_fps)
		SDL_Delay(g_fps - g_delta);
}

void handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_bRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				g_bRunning = false;
			break;
		}
	}
}

bool keyDown(SDL_Scancode c)
{
	if (g_iKeystates != nullptr)
	{
		if (g_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void update()
{
	if (keyDown(SDL_SCANCODE_W))
		g_dst.y -= g_iSpeed;
	if (keyDown(SDL_SCANCODE_S))
		g_dst.y += g_iSpeed;
	if (keyDown(SDL_SCANCODE_A))
		g_dst.x -= g_iSpeed;
	if (keyDown(SDL_SCANCODE_D))
		g_dst.x += g_iSpeed;
}

void render()
{
	/*---------- Clearing window. ----------*/
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(g_pRenderer);
	/*---------- Render stuff. ----------*/
	SDL_RenderCopy(g_pRenderer, g_pTexture, &g_src, &g_dst);
	/*---------- Drawing new. ----------*/
	SDL_RenderPresent(g_pRenderer);
}

void clean()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyTexture(g_pTexture);
	SDL_DestroyRenderer(g_pRenderer);
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (init("GAME1007 SDL Setup", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;			// Exit program
	while (g_bRunning)		// Main engine loop.
	{
		wake();
		handleEvents();		// Input
		update();			// Processing.
		render();			// Output.

		if (g_bRunning)
			sleep();
	}
	clean();
	return 0;
}