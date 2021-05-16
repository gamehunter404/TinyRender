#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include<map>
#include<string>

#define TEXTURE_DEFAULT "default_Tex"

class TGAImage;

class TextureManager 
{
public:
	static void initManager();
	static void destoryManager();
	static TGAImage* getTexture(const char* name);
	static void LoadTexture(const char* filename);

private:
	TextureManager() {}
	TextureManager(const TextureManager&) {}
	~TextureManager();

private:
	static TextureManager *_instance;

	std::map<std::string, TGAImage*> _textures;
};


#endif