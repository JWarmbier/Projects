#pragma once
#ifndef MY_CAMERA_H
#define MY_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#define M_PI 3.14159265358979323846
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class MyCamera
{
public:

	// Constructor with vectors
	MyCamera(glm::vec3 position, glm::vec3 reference, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		Position = Position_tmp =  position;
		/*Reference = reference;
		Reference.x = Position.x + R*sin(Alpha);
		Reference.y = 0;
		Reference.z = Position.z + R*cos(Alpha);*/
		
		Up = up;
		R = 100;
		Alpha = 5*M_PI/4;
		Beta = 0;
		sens = 0.05;
		mouseSens = 0.002 ;
		Reference.x = Position.x + R*sin(Alpha);
		Reference.y = 0;
		Reference.z = Position.z + R*cos(Alpha);
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	void setPosition(glm::vec3 position) {
		Position = position;
		przelicz();
	}
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Reference, Up);
	}

	void  SetR(float r) {
		this->R = r;
	}
	void SetAlpha(float alpha) {
		this->Alpha = alpha;
	}

	void MoveForward() {
		Position.x += sens*sin(Alpha);
		Position.z += sens*cos(Alpha);
		przelicz();
	}
	void MoveBackward() {
		Position.x -= sens*sin(Alpha);
		Position.z -= sens*cos(Alpha);
		przelicz();
	}
	void MoveLeft() {
		Position.x += sens*cos(Alpha);
		Position.z -= sens*sin(Alpha);
		przelicz();
	}
	void MoveRight() {
		Position.x -= sens*cos(Alpha);
		Position.z += sens*sin(Alpha);
		przelicz();
	}

	void MoveMouse(float xoffset, float yoffset) {
		Alpha -= xoffset * mouseSens;
		Beta += yoffset * mouseSens;
		przelicz();
	}
	glm::vec3 getPosition() { return Position; }
private:
	void przelicz(){
		Reference.x = Position.x + R*sin(Alpha);
		Reference.y = R*sin(Beta);
		Reference.z = Position.z + R*cos(Alpha);
	}
	glm::vec3 Position;
	glm::vec3 Position_tmp;
	glm::vec3 Reference;
	glm::vec3 Up;
	float R;
	float Alpha;
	float Beta;
	float sens;
	float mouseSens;
	
};
#endif