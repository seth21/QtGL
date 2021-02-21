#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

class Texture;

struct TextureResource {
	Texture* texture;
	int references;
	std::string resourcePath;
};

#endif