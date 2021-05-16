#include"TextureManager.h"
#include"tgaImage.h"
#include<iostream>

TextureManager* TextureManager::_instance = nullptr;

void TextureManager::initManager()
{
	if (_instance == nullptr)
	{
		_instance = new TextureManager();

		//TODO 初始化一个默认的贴图
		TGAImage* defaultTexture = new TGAImage(800, 600, 4);
		TGAColor color(255, 0, 0, 255);

		for (int x = 0; x < 800; x++)
		{
			for (int y = 0; y < 600; y++)
			{
				defaultTexture->set(x, y, color);
			}
		}

		_instance->_textures[TEXTURE_DEFAULT] = defaultTexture;
	}
}

void TextureManager::destoryManager()
{
	if (_instance != nullptr)
	{
		delete _instance;
		_instance = nullptr;
	}
}

TGAImage* TextureManager::getTexture(const char* name)
{
	if (_instance == nullptr) 
		return nullptr;

	TGAImage* res = nullptr;


	if (_instance->_textures.find(name) != _instance->_textures.end())
	{
		res = _instance->_textures[name];
	}
	else {
		res = _instance->_textures[TEXTURE_DEFAULT];
	}

	return res;
}

void TextureManager::LoadTexture(const char* filename)
{
	if (filename == nullptr || _instance == nullptr)
		return;

	if (_instance->_textures.find(filename) != _instance->_textures.end())
	{
		return;
	}


	TGAImage* tex = new TGAImage();

	if (tex->read_tga_file(filename)) {

		_instance->_textures[filename] = tex;
		std::cerr <<std::endl <<"----------------------------------------------------------"<<std::endl;
		std::cerr << "loaded texture '"<<filename<<" ' # w = "<<tex->get_width()<<",h = "<<tex->get_height()<<std::endl;
		std::cerr <<"----------------------------------------------------------" << std::endl;

	}
	else {
		std::cerr << std::endl << "----------------------------------------------------------" << std::endl;
		std::cerr << "failed to load texture '" << filename << " '" << std::endl;
		std::cerr << "----------------------------------------------------------" << std::endl;
	}


}

TextureManager::~TextureManager()
{
	for (auto n = _textures.begin(); n != _textures.end(); n++)
	{
		TGAImage* ptr = n._Ptr->_Myval.second;

		if(ptr!=nullptr)
			delete ptr;
	}
}
