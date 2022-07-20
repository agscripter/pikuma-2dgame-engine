#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class AssetStore {

private:
	std::unordered_map<std::string, SDL_Texture*> textures;

public:
	AssetStore();
	~AssetStore();
	
	void clearAssets();
	void addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* getTexture(const std::string& assetId);

};

#endif