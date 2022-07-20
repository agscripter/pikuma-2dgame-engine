#ifndef GAME_H
#define GAME_H

#include <memory>

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

// Constants
const int FPS = 120;
const int MILLISECS_PER_FRAME = 1000 / FPS;

// Game class
class Game {

private:
	bool isRunning;
	bool isDebug;
	int millisecsPreviousFrame;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;

public:
	Game();
	~Game();

	void initialize();
	void loadLevel(int level);
	void setup();
	void run();
	void processInput();
	void update();
	void render();
	void destroy();

	int currentTicks = 0;
	int skipTicks = 3;

	static int windowWidth;
	static int windowHeight;
	static int mapWidth;
	static int mapHeight;

};

#endif