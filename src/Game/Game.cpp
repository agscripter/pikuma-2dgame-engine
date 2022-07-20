#include "Game.h"

#include <iostream>
#include <memory>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifecycleSystem.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() {
	Logger::info("Creating a Game instance");
	isRunning = false;
	isDebug = false;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
}

Game::~Game() {
	Logger::info("Destroying a Game instance");
}

void Game::initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::error("Error initializing SDL.");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = 800;
    windowHeight = 600;
    window = SDL_CreateWindow(
        "2D Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        Logger::error("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::error("Error creating SDL renderer.");
        return;
    }

    // Initialize the camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    isRunning = true;
}

void Game::loadLevel(int level) {
	// Add the systems that need to be processed in our game
	registry->addSystem<MovementSystem>();
	registry->addSystem<RenderSystem>();
	registry->addSystem<AnimationSystem>();
	registry->addSystem<CollisionSystem>();
	registry->addSystem<RenderColliderSystem>();
	registry->addSystem<DamageSystem>();
	registry->addSystem<KeyboardControlSystem>();
	registry->addSystem<CameraMovementSystem>();
	registry->addSystem<ProjectileEmitSystem>();
	registry->addSystem<ProjectileLifecycleSystem>();

	// Adding assets
	assetStore->addTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->addTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->addTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	assetStore->addTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetStore->addTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
	assetStore->addTexture(renderer, "bullet-image", "./assets/images/bullet.png");

	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;
	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	if (mapFile.fail()) {
		Logger::error("Level map not found");
    	return;
	}

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;

			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;

			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;

			mapFile.ignore();

			Entity tile = registry->createEntity();
			tile.addComponent<TransformComponent>(
				glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), 
				glm::vec2(tileScale, tileScale),
				0.0
			);

			tile.addComponent<SpriteComponent>(
				"tilemap-image",
				0,
				tileSize, tileSize,
				false,
				srcRectX, srcRectY
			);
		}
	}

	mapFile.close();
	mapWidth = mapNumCols * tileSize * tileScale;
	mapHeight = mapNumRows * tileSize * tileScale;

	// Create an Entity
	Entity chopper = registry->createEntity();
	chopper.tag("player");
	chopper.addComponent<TransformComponent>(glm::vec2(32.0, 32.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.addComponent<SpriteComponent>("chopper-image", 2, 32, 32);
	chopper.addComponent<AnimationComponent>(2, 10, true);
	chopper.addComponent<KeyboardControlledComponent>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80), glm::vec2(-80, 0));
	chopper.addComponent<CameraFollowComponent>();
	chopper.addComponent<HealthComponent>(100);
	chopper.addComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 10000, 0, true);

	Entity tank = registry->createEntity();
	tank.group("enemies");
    tank.addComponent<TransformComponent>(glm::vec2(400.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.addComponent<SpriteComponent>("tank-image", 1, 32, 32);
    tank.addComponent<BoxColliderComponent>(32, 32);
    tank.addComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 0, false);
    tank.addComponent<HealthComponent>(100);

    Entity truck = registry->createEntity();
    truck.group("enemies");
    truck.addComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    truck.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.addComponent<SpriteComponent>("truck-image", 2, 32, 32);
    truck.addComponent<BoxColliderComponent>(32, 32);
    truck.addComponent<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000, 5000, 0, false);
    truck.addComponent<HealthComponent>(100);

	Entity radar = registry ->createEntity();
	radar.addComponent<TransformComponent>(glm::vec2(700.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
	radar.addComponent<SpriteComponent>("radar-image", 3, 64, 64, true);
	radar.addComponent<AnimationComponent>(8, 5, true);	
}

void Game::setup() {
	loadLevel(1);
}

void Game::update() {
	if (currentTicks < skipTicks) {
		currentTicks++;
		return;
	}

	// If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();
   
    // Reset all event handlers for the current frame
    eventBus->reset();

    // Perform the subscription of the events for all systems
    registry->getSystem<DamageSystem>().subscribeToEvents(eventBus);
    registry->getSystem<KeyboardControlSystem>().subscribeToEvents(eventBus);
    registry->getSystem<ProjectileEmitSystem>().subscribeToEvents(eventBus);

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->update();
    
    // Invoke all the systems that need to update 
    registry->getSystem<MovementSystem>().update(deltaTime);
    registry->getSystem<AnimationSystem>().update();
    registry->getSystem<CollisionSystem>().update(eventBus);
    registry->getSystem<ProjectileEmitSystem>().update(registry);
    registry->getSystem<ProjectileLifecycleSystem>().update();
    registry->getSystem<CameraMovementSystem>().update(camera);
}

void Game::processInput() {
	SDL_Event sdlEvent;
	while(SDL_PollEvent(&sdlEvent)) {
		switch(sdlEvent.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				if (sdlEvent.key.keysym.sym == SDLK_d) {
					isDebug = !isDebug;
				}
				eventBus->emit<KeyPressedEvent>(sdlEvent.key.keysym.sym);
				break;
		}
	}
}
	
void Game::render() {
	SDL_SetRenderDrawColor(renderer,21,21,21,255);
	SDL_RenderClear(renderer);

	registry->getSystem<RenderSystem>().update(renderer, assetStore, camera);

	// Update Render Collider System
	if (isDebug) {
		registry->getSystem<RenderColliderSystem>().update(renderer, camera);
	}

	SDL_RenderPresent(renderer);
}

void Game::run() {
	setup();

	while(isRunning) {
		processInput();
		update();
		render();
	}
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}