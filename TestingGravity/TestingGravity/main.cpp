#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <time.h>
#include <string>
#include <stdio.h>
#include <sstream>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *pug;
SDL_Texture *poopt;
SDL_Texture *skyt;
SDL_Texture *grasst;
SDL_Texture *text;
SDL_Event e;
SDL_RendererFlip flip = SDL_FLIP_NONE;
bool running = true;
bool isjumping = false;
const short sh = 500;
const short sw = 800;
int frames = 0;
float currenttime;
float pasttime;
float timedif;
float timer;
SDL_Rect player = { 55,400,50,50 };
const Uint8* SDL_GetKeyboardState(int* numkeys);
const Uint8 *state = SDL_GetKeyboardState(NULL);
Uint32 SDL_GetTicks(void);

float vy = 12;
float vypoop[600];
float gravity = 1;
SDL_Rect ground = { 0, 448, 800, 52 };
SDL_Rect grass = { 0, 444, 800, 6 };
SDL_Rect skypos = { 0,0, 800, 500 };
using namespace std;

typedef struct 
{
	SDL_Rect pos = {NULL, NULL, NULL, NULL };
}Poop;

Poop poop[600];
void setup() 
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
	window = SDL_CreateWindow("A pug", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	for (int num = 0; num < 400; num++)
	{
		vypoop[num] = 1;
	}
}

int wall() 
{
	if (player.x > 750)
	{
		player.x = 750;
	}
	if (player.x < 0)
	{
		player.x = 0;
	}
	if (player.y > 450) 
	{
		player.y = 450;
	}
	if (player.y < 0)
	{
		player.y = 0;
	}
	return 0; 
}
int time() 
{
	pasttime = currenttime;
	currenttime = SDL_GetTicks();
	timedif = (currenttime - pasttime) * 0.05;
	return 0;
}

int timerfunc() 
{
	timer = SDL_GetTicks() * 0.0005;
	return 0;
}

int main(int ac, char** av)
{
	setup();
	SDL_Surface *pugi = IMG_Load("pugicon.jpg");
	SDL_SetWindowIcon(window, pugi);
	SDL_Surface *pugsurf = IMG_Load("pug.png");
	pug = SDL_CreateTextureFromSurface(renderer, pugsurf);
	SDL_FreeSurface(pugsurf);
	SDL_Surface *poopsurf = IMG_Load("poop.png");
	poopt = SDL_CreateTextureFromSurface(renderer, poopsurf);
	SDL_FreeSurface(poopsurf);
	SDL_Surface *skysurf = IMG_Load("sky.png");
	skyt = SDL_CreateTextureFromSurface(renderer, skysurf);
	SDL_FreeSurface(skysurf);
	SDL_Surface *grasssurf = IMG_Load("grass.png");
	grasst = SDL_CreateTextureFromSurface(renderer, grasssurf);
	SDL_FreeSurface(grasssurf);

	SDL_ShowCursor(1);
	while (running) 
	{
		//SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, skyt, NULL, &skypos);
		SDL_SetRenderDrawColor(renderer, 77, 56, 66, 255);
		SDL_RenderFillRect(renderer, &ground);
		//SDL_SetRenderDrawColor(renderer, 144, 245, 0, 255);
		//SDL_RenderFillRect(renderer, &grass);
		SDL_RenderCopy(renderer, grasst, NULL, &grass);
		time();
		timerfunc();

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) 
			{
				running = false;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE) 
			{
				running = false;
			}
			if (e.key.keysym.sym == SDLK_p) 
			{
				for (int num = 0; num < 600; num++)
				{
					if ((&poop[num].pos.x) && (poop[num].pos.y) == NULL)
					{
						poop[num].pos = { player.x, player.y + 34, 16,16 };
						break;
					}
				}
			}
		}
		for (int num = 0; num < 600; num++)
		{
			if ((poop[num].pos.y != NULL) && (poop[num].pos.y < 434))
			{
				poop[num].pos.y += (vypoop[num] *timedif);
				vypoop[num] += (gravity * timedif);
			}
			if (poop[num].pos.y >= 434)
			{
				poop[num].pos.y = 434;
			}
		}
		if (state[SDL_SCANCODE_SPACE])
		{
			player.y -= 0.1;
			isjumping = true;
		}
		if (state[SDL_SCANCODE_D])
		{
			player.x += 4;
			flip = SDL_FLIP_HORIZONTAL;
		}
		if (state[SDL_SCANCODE_A])
		{
			player.x -= 4;
			flip = SDL_FLIP_NONE;
		}
		if (isjumping == true && player.y < 400) 
		{
			player.y = player.y - vy * timedif;
			vy -= (gravity * timedif);
		}
		if (player.y >= 400) 
		{
			isjumping = false;
			vy = 12;
			player.y = 400;
		}
		wall();
		for (int num = 0; num < 400; num++)
		{
			SDL_RenderCopy(renderer, poopt, NULL, &poop[num].pos);
		}
		SDL_RenderCopyEx(renderer, pug, NULL, &player, 0, NULL, flip);
		SDL_RenderPresent(renderer);
	}
	IMG_Quit();
	SDL_Quit();
	return 0;
}