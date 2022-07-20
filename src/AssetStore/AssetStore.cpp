#include "AssetStore.h"
#include "../Logger/Logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

AssetStore::AssetStore() {
	Logger::info("AssetStore constructor called");
}

AssetStore::~AssetStore() {
	clearAssets();
	Logger::info("AssetStore destructor called");
}

void AssetStore::clearAssets() {
	Logger::info("Clearing all the assets");
	for (auto texture: textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetStore::addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	textures.emplace(assetId, texture);
	Logger::info("New texture added to AssetStore with id " + assetId);
}

SDL_Texture* AssetStore::getTexture(const std::string& assetId) {
	return textures[assetId];
}