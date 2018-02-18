#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <glm\glm.hpp>
#include "PlisReader.h"


#include <GL\glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool readPlis(std::string filepath, std::vector<unsigned short>& indices, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normals) {

	FILE* f = fopen(filepath.c_str(), "r");
	if (f == NULL) {
		return false;
	}
	
	while (1) {
		char line[128];
		int res = fscanf(f, "%s", line);
		if (res == EOF)
			break;

		if (strcmp(line, "v") == 0) {
			glm::vec3 res;
			fscanf(f, "%f %f %f\n", &res.x, &res.y, &res.z);
			vertices.push_back(res);
			//printf("v %f %f %f\n", res.x, res.y, res.z);
		}

		if (strcmp(line, "n") == 0) {
			glm::vec3 res;
			fscanf(f, "%f %f %f\n", &res.x, &res.y, &res.z);
			normals.push_back(res);
			//printf("n %f %f %f\n", res.x, res.y, res.z);
		}

		if (strcmp(line, "u") == 0) {
			glm::vec2 res;
			fscanf(f, "%f %f %f\n", &res.x, &res.y);
			uv.push_back(res);
			//printf("u %f %f\n", res.x, res.y);
		}
		
		if (strcmp(line, "i") == 0) {
			unsigned short res;
			fscanf(f, "%hu\n", &res);
			indices.push_back(res);
			//printf("i %hu\n", res);
		}
	}

	fclose(f);
	return 1;
}

bool loadAssImp(const char * path, std::vector<unsigned short> & indices, std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> & normals) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_OptimizeMeshes);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}

	const aiMesh* mesh = scene->mMeshes[0];

	int c_mesh = scene->mNumMeshes;
	std::cout << "Num meshes: " << c_mesh << std::endl;

	// vertices
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}

	// uvs
	uvs.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D UVW = mesh->mTextureCoords[0][i];
		uvs.push_back(glm::vec2(UVW.x, 1.0f - UVW.y));
		printf("%f %f\n", UVW.x, UVW.y);
	}

	// normals
	normals.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));
	}

	// indices
	indices.reserve(3 * mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	return true;
}