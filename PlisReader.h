#pragma once
bool readPlis(std::string filepath, std::vector<unsigned short>& indices, std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &uv, std::vector<glm::vec3> &normals);

bool loadAssImp(const char * path, std::vector<unsigned short> & indices, std::vector<glm::vec3> & vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3> & normals);