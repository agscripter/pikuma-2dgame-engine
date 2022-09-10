#include "AssetStore.h"
#include "../Logger/Logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

	for (auto font: fonts) {
		TTF_CloseFont(font.second);
	}

	textures.clear();
	fonts.clear();
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

void AssetStore::addFont(const std::string& assetId, const std::string& filePath, int fontSize) {
	fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
	Logger::info("New font added to AssetStore with id " + assetId);
}

TTF_Font* AssetStore::getFont(const std::string& assetId) {
	return fonts[assetId];
}

