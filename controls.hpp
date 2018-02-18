#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
void positionY(float v);
bool checkGravity();
float getPositionY();
glm::vec3 getPosition();

#endif