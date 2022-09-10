#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AssetStore {

private:
	std::unordered_map<std::string, SDL_Texture*> textures;
	std::unordered_map<std::string, TTF_Font*> fonts;

public:
	AssetStore();
	~AssetStore();
	
	void clearAssets();

	void addTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* getTexture(const std::string& assetId);

	void addFont(const std::string& assetId, const std::string& filePath, int fontSize);
	TTF_Font* getFont(const std::string& assetId);

};

#endif