#pragma once
#ifndef CAR_H
#include <map>
#include "model.h"
#include "my_camera.h"
#define CAR_H
#define MAX_FORWARD 0.2
#define MAX_BACKWARD -0.1
class Car {
private:
	Model *dashboard;
	Model *steeringWheel;
	Model *armChairs;
	Model *chassis;
	Model *frontSideLeft;
	Model *frontSideRight;
	Model *backSideLeft;
	Model *backSideRight;
	Model *windscreen;
	Model *rearWindow;
	Model *wheel;
	Model *mirrors;
	glm::vec3 Translation;
	float Rotation;
	float Turning;
	float Velocity;
	glm::vec3 Move;
	glm::vec3 lightPosition;
	float LengthMove;
	float windowTransparency;
public:
	//Cameras
	MyCamera *behindCamera;
	MyCamera *inCamera;
public:
	
	Car() {
		dashboard = new Model("resources/objects/car/dashboard.obj");
		steeringWheel = new Model("resources/objects/car/steering_wheel.obj");
		armChairs = new Model("resources/objects/car/armchair.obj");
		chassis = new Model("resources/objects/car/chassis.obj");
		frontSideLeft = new Model("resources/objects/car/side_front_left.obj");
		frontSideRight = new Model("resources/objects/car/side_front_right.obj");
		backSideLeft = new Model("resources/objects/car/side_back_left.obj");
		backSideRight = new Model("resources/objects/car/side_back_right.obj");
		windscreen = new Model("resources/objects/car/front_glass.obj");
		rearWindow = new Model("resources/objects/car/back_glass.obj");
		wheel = new Model("resources/objects/car/back_wheel.obj");
		mirrors = new Model("resources/objects/car/mirrors.obj");
		windowTransparency = (0.3f);
		Translation = glm::vec3();
		Turning = 0;
		Velocity = 0;
		
		Move = glm::vec3(0.0f, 0.0f, 0.0f);
		LengthMove = 0.0f;
		behindCamera = new MyCamera(glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f,0.0f));
		behindCamera->SetAlpha(M_PI);
		inCamera = new MyCamera(glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}


	void Draw(Shader shader, MyCamera camera) {
		glm::mat4 model;

		//draw dashboard
		model = glm::mat4();
		model = glm::translate(model, Translation);
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		dashboard->Draw(shader);

		//draw seats
		model = glm::mat4();
		model = glm::translate(model, Translation);
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		armChairs->Draw(shader);

		//draw mirrors

		model = glm::mat4();
		model = glm::translate(model, Translation);
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		mirrors->Draw(shader);

		//draw chassis
		model = glm::mat4();
		model = glm::translate(model, Translation);
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		chassis->Draw(shader);

		// steeering wheel
		model = glm::mat4();
		steeringWheel->setPosition(glm::vec3(1.82f, 1.9f, 0.60f));
		glm::vec3 pos = steeringWheel->getPosition();
		//model = glm::translate(model, glm::vec3(1.82f, 1.9f, -0.35f) + Translation);
		double staticAngle = atan2(steeringWheel->getPosition().z, steeringWheel->getPosition().x);
		model = glm::translate(model, glm::vec3(steeringWheel->getRadius()*cos(staticAngle + Rotation / 180 * M_PI), pos.y, steeringWheel->getRadius()*sin(staticAngle + Rotation / 180 * M_PI)) + Translation);
		model = glm::rotate(model, -Turning, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 29.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
		
		shader.setMat4("model", model);
		steeringWheel->Draw(shader);

		//front-left wheel
		//model = clearingMatrix;
		//model = glm::translate(model, glm::vec3(2.9f, 2.855f, -0.5f));
		//model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
		//shader.setMat4("model", model);
		frontSideLeft->setTransparency(windowTransparency);
		frontSideLeft->setPosition(glm::vec3(2.9f, 2.855f, -0.5f));
		//frontSideLeft->Draw(shader);

		//fornt-right wheel
		//model = clearingMatrix;
		//model = glm::translate(model, glm::vec3(-2.9f, 2.855f, -0.5f));
		//model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
		//shader.setMat4("model", model);
		frontSideRight->setTransparency(windowTransparency);
		frontSideRight->setPosition(glm::vec3(-2.9f, 2.855f, -0.5f));
		//frontSideRight->Draw(shader);

		//rear-left wheel
		//model = clearingMatrix;
		//model = glm::translate(model, glm::vec3(2.9f, 2.855f, -4.25f));
		//model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
		//shader.setMat4("model", model);
		backSideLeft->setTransparency(windowTransparency);
		backSideLeft->setPosition(glm::vec3(2.9f, 2.855f, -4.25f));
		//backSideLeft->Draw(shader);

		//rear-right wheel
		//model = clearingMatrix;
		//model = glm::translate(model, glm::vec3(-2.9f, 2.855f, -4.25f));
		//model = glm::scale(model, glm::vec3(1.05f, 1.05f, 1.05f));
		//shader.setMat4("model", model);
		backSideRight->setTransparency(windowTransparency);
		backSideRight->setPosition(glm::vec3(-2.9f, 2.855f, -4.25f));
		//backSideRight->Draw(shader);

		//windscreen
		//model = clearingMatrix;
		//model = glm::translate(model, glm::vec3(0.0f, 2.80f, 2.05f));
		//model = glm::scale(model, glm::vec3(1.01f, 1.05f, 1.05f));
		//shader.setMat4("model", model);
		windscreen->setTransparency(windowTransparency);
		windscreen->setPosition(glm::vec3(0.0f, 2.80f, 2.05f));
		//windscreen->Draw(shader);

		//rear window
		//model = clearingMatrix;
		//model = glm::translate(model, glm::vec3(0.0f, 2.80f, -5.45f));
		//model = glm::scale(model, glm::vec3(1.08f, 1.05f, 1.05f));
		//shader.setMat4("model", model);
		rearWindow->setTransparency(windowTransparency);
		rearWindow->setPosition(glm::vec3(0.0f, 2.80f, -5.45f));
		//rearWindow->Draw(shader);



		//front-left wheel
		model = glm::mat4();
		int sign;
		float rot = 360.0f  * LengthMove / (2 *1.8 * M_PI);
		wheel->setPosition(glm::vec3(3.0f, -1.0f, 2.60f));
		model = glm::translate(model, glm::vec3(wheel->getRadius()*cos(0.710907 + Rotation / 180 * M_PI), -1.0f, wheel->getRadius()*sin(0.710907+ Rotation / 180 * M_PI)) + Translation);
		model = glm::rotate(model, -Rotation - Turning, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rot , glm::vec3(-1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wheel->Draw(shader);

		//front-right wheel
		model = glm::mat4();
		wheel->setPosition(glm::vec3(3.0f , -1.0f , -2.60f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f) );
		model = glm::translate(model, glm::vec3(wheel->getRadius()*cos(0.710907 - Rotation / 180 * M_PI) - Translation.x, -1.0f + Translation.y, wheel->getRadius()*sin(-0.710907 + Rotation / 180 * M_PI) - Translation.z) );
		model = glm::rotate(model, -Rotation -Turning, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rot , glm::vec3(-1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wheel->Draw(shader);

		//rear-left wheel
		model = glm::mat4();
		wheel->setPosition(glm::vec3(3.0f, -1.0f, -4.60f));
		model = glm::translate(model, glm::vec3(wheel->getRadius()*cos(0.992894 - Rotation / 180 * M_PI), -1.0f, wheel->getRadius()*sin(-0.992894 + Rotation / 180 * M_PI)) + Translation);
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rot, glm::vec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wheel->Draw(shader);

		//rear-right wheel
		model = glm::mat4();
		wheel->setPosition(glm::vec3(3.0f, -1.0f, 4.60f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::translate(model, glm::vec3(wheel->getRadius()*cos(0.992894 + Rotation / 180 * M_PI) - Translation.x, -1.0f + Translation.y, wheel->getRadius()*sin(0.992894 + Rotation / 180 * M_PI) - Translation.z));
		model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rot, glm::vec3(-1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		wheel->Draw(shader);

		vector <Model*> glasses;
		glasses.push_back(frontSideLeft);
		glasses.push_back(frontSideRight);
		glasses.push_back(backSideLeft);
		glasses.push_back(backSideRight);
		glasses.push_back(windscreen);
		glasses.push_back(rearWindow);

		std::map <float, Model*> sorted;
		for (int i = 0; i < glasses.size(); i++){
			float distance = glm::length(camera.getPosition() - glasses[i]->getPosition());
			sorted[distance] = glasses[i];
		}

		for (std::map<float, Model*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4();
			glm::vec3 pos = it->second->getPosition();
			double staticAngle = atan2(it->second->getPosition().z, it->second->getPosition().x);
			model = glm::translate(model, glm::vec3(it->second->getRadius()*cos(staticAngle + Rotation / 180 * M_PI) ,pos.y, it->second->getRadius()*sin(staticAngle + Rotation / 180 * M_PI)) + Translation);
			model = glm::scale(model, glm::vec3(1.03f, 1.03f, 1.03f));
			model = glm::rotate(model, -Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			shader.setMat4("model", model);
			it->second->Draw(shader);
		}
	}
	void moveTranslate(glm::vec3 move) {
		Translation += move;
		if(move.z > 0)	Rotation += Turning * Velocity/MAX_FORWARD /100;
		else if (move.z < 0) Rotation += Turning * Velocity / MAX_FORWARD / 200;

		if (Velocity > 0) {
			LengthMove += abs(glm::length(move));
		}
		else if (Velocity < 0) {
			LengthMove -= abs(glm::length(move));
		}
	}
	void moveRotate(float rotation) {
		Rotation -= rotation;
	}
	float getRotation() {
		return Rotation;
	}
	float getTurning() { return Turning; }
	void setTurning(float turning) { Turning = turning; }
	float getVelocity() { return Velocity; }
	void setVelocity(float velocity) { Velocity = velocity; }

	glm::mat4 getBehindCamera() {
		behindCamera->setPosition(glm::vec3(20.0f * sin(Rotation / 180 * M_PI), 10.0f, -20.0f * cos(Rotation/180 * M_PI)) + Translation);
		return behindCamera->GetViewMatrix();
	}
	glm::vec3 getBehindCameraPostion() {
		return behindCamera->getPosition();
	}

	glm::mat4 getInCamera() {
		inCamera->setPosition(glm::vec3(  -3 * sin(-M_PI / 8 * 6 + Rotation / 180 * M_PI), 3.0f, 3.0f * cos(-M_PI /8 *6 + Rotation / 180 * M_PI)) + Translation);
		return inCamera->GetViewMatrix();
	}
	glm::vec3 getInCameraPostion() {
		return inCamera->getPosition();
	}
	void incrementWindowTransparency() {
		windowTransparency += 0.001;
	}
	void decrementWindowTransparency() {
		windowTransparency -= 0.001;
	}
	glm::vec3 getPosition() { return Translation; }
	glm::vec3 getLightPosition() {
		lightPosition = (glm::vec3(-8.0f * sin(Rotation / 180 * M_PI), 2.0f, 8.0f * cos(Rotation / 180 * M_PI)) + Translation);
		return lightPosition;
	}
};

#endif