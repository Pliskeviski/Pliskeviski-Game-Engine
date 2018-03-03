#include "Texture.h"

//int s = 0;

Texture::Texture(std::string path, TextureType type) {
	assert(type != TYPE_CUBEMAP);
	this->texture.id = loadTexture(path);
	this->texture.type = type;
}

Texture::Texture(std::vector<std::string> faces, TextureType type) {
	assert(type == TYPE_CUBEMAP);
	this->texture.id = loadCubeMap(faces);
	this->texture.type = type;
}

GLuint Texture::loadCubeMap(std::vector<std::string> faces) {
	int width, height;
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		std::cout << "Loading cubemap path: " << faces[i] << std::endl;
		if (image) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		else {
			std::cout << "Failed to load cubemap texture [" << i << "]." << std::endl;
			SOIL_free_image_data(image);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;

}

GLuint Texture::loadTexture(std::string path) {
	int width, height;
	GLuint textureID;

	std::cout << "Loading texture path: " << path << std::endl;
	
	//glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	unsigned char *image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == nullptr) {
		std::cout << "Failed to load texture" << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
