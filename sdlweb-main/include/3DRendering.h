#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include<vector>
#include<cmath>

struct Triangle {
	Triangle(std::vector<SDL_Vertex> points, SDL_Color color) {
		for (int i = 0; i < 3; i++) {
			this->triangle[i] = points[i];
			this->triangle[i].color = color;
			printf("the point x is: %f \n",triangle[i].position.x);
		}
		this->triangle[0].tex_coord.x = 0;
		this->triangle[0].tex_coord.y = 0;
		this->triangle[1].tex_coord.x = 0;
		this->triangle[1].tex_coord.y = 1;
		this->triangle[2].tex_coord.x = 1;
		this->triangle[2].tex_coord.y = 1;
	}

	SDL_Vertex triangle[3];
};

class ObjectData {
public:
	ObjectData() {
		xPos = 0;
		yPos = 0;
		rotation = 0;
	};

	ObjectData(std::vector<Triangle> objectTriangles , int xPos, int zPos, int yPos, int rotation, int width, int height) {
		this->objectTriangles = objectTriangles;
		this->xPos = xPos;
		this->zPos = zPos;
		this->yPos = yPos;
		this->rotation = rotation;
		this->height = height;
		this->width = width;
	}

	void Set2DPos(int xPos, int zPos) {
		this->xPos = xPos;
		this->zPos = zPos;
	}

	void SetAll(const std::vector<Triangle>& objectTriangles, int xPos, int zPos, int yPos, int rotation, int width, int height) {

		this->objectTriangles = objectTriangles;


		this->xPos = xPos;
		this->zPos = zPos;
		this->yPos = yPos;
		this->rotation = rotation;
		this->height = height;
		this->width = width;
	}

	void SetFlatSquarePosition(int xPos, int yPos, int width, int height, double rotationAmount) {
		int halfWidth = width / 2;
		int halfHeight = height / 2;
		rotationAmount =  rotationAmount * M_PI / 180;
		objectTriangles[0].triangle[0].position.x =  xPos + ((-halfWidth) * cos(rotationAmount) - (-halfHeight) * sin(rotationAmount));
		objectTriangles[0].triangle[0].position.y = yPos + ((-halfHeight) * cos(rotationAmount) + (- halfWidth) * sin(rotationAmount));
		objectTriangles[0].triangle[1].position.x = xPos +((-halfWidth) * cos(rotationAmount) - (halfHeight) * sin(rotationAmount));
		objectTriangles[0].triangle[1].position.y = yPos + ((halfHeight) * cos(rotationAmount) + (-halfWidth) * sin(rotationAmount));
		objectTriangles[0].triangle[2].position.x = xPos + ((halfWidth) * cos(rotationAmount) - (halfHeight) * sin(rotationAmount));
		objectTriangles[0].triangle[2].position.y = yPos + ((halfHeight)*cos(rotationAmount) + (halfWidth) * sin(rotationAmount));

		objectTriangles[1].triangle[0].position.x = xPos + ((-halfWidth) * cos(rotationAmount) - (-halfHeight) * sin(rotationAmount));
		objectTriangles[1].triangle[0].position.y = yPos + ((-halfHeight) * cos(rotationAmount) + (-halfWidth) * sin(rotationAmount));
		objectTriangles[1].triangle[1].position.x = xPos + ((halfWidth) * cos(rotationAmount) - (-halfHeight) * sin(rotationAmount));
		objectTriangles[1].triangle[1].position.y = yPos + ((-halfHeight) * cos(rotationAmount) + (halfWidth) * sin(rotationAmount));
		objectTriangles[1].triangle[2].position.x = xPos + ((halfWidth)*cos(rotationAmount) - (halfHeight)*sin(rotationAmount));
		objectTriangles[1].triangle[2].position.y = yPos + ((halfHeight)*cos(rotationAmount) + (halfWidth)*sin(rotationAmount));
	}
	
	void SetTopLeftVertex(int xPos, int yPos) {
		objectTriangles[0].triangle[0].position.x = xPos;
		objectTriangles[0].triangle[0].position.y = yPos;
		objectTriangles[1].triangle[0].position.x = xPos;
		objectTriangles[1].triangle[0].position.y = yPos;
	} 

	void SetTopRightVertex(int xPos, int yPos) {
		objectTriangles[1].triangle[1].position.x = xPos;
		objectTriangles[1].triangle[1].position.y = yPos;
	}

	void SetBottomRightVertex(int xPos, int yPos) {
		objectTriangles[0].triangle[2].position.x = xPos;
		objectTriangles[0].triangle[2].position.y = yPos;
		objectTriangles[1].triangle[2].position.x = xPos;
		objectTriangles[1].triangle[2].position.y = yPos;
	}

	void SetBottomLeftVertex(int xPos, int yPos) {
		objectTriangles[0].triangle[1].position.x = xPos;
		objectTriangles[0].triangle[1].position.y = yPos;
	}

	void SetLeftEdge(int xPos, int yPos, int width, int height) {
		objectTriangles[0].triangle[0].position.y = yPos - height/2;
		objectTriangles[0].triangle[1].position.y = yPos + height / 2;
		objectTriangles[1].triangle[0].position.y = yPos - height / 2;
	}

	void SetRightEdge(int xPos, int yPos, int width, int height) {
		objectTriangles[0].triangle[2].position.y = yPos + height / 2;
		objectTriangles[1].triangle[2].position.y = yPos - height / 2;
		objectTriangles[1].triangle[1].position.y = yPos + height / 2;
	}

	void SetTopEdge(int xPos, int yPos, int width, int height) {
		objectTriangles[0].triangle[0].position.x = xPos - width / 2; 
		objectTriangles[1].triangle[0].position.x = xPos - width / 2; 
		objectTriangles[1].triangle[2].position.x = xPos + width / 2; 
	}

	void SetBottomtEdge(int xPos, int yPos, int width, int height) {
		objectTriangles[0].triangle[1].position.x = xPos - width / 2;
		objectTriangles[0].triangle[2].position.x = xPos + width / 2;
		objectTriangles[1].triangle[1].position.x = xPos + width / 2;
	}

	int GetWidth() {
		return width;
	}

	int GetHeight() {
		return height;
	}

	int GetXPos() {
		return xPos;
	}

	int GetYPos() {
		return yPos;
	}

	int GetZPos() {
		return zPos;
	}

	int GetRotation() {
		return yPos;
	}

	std::vector<Triangle> GetTriangles() {
		return objectTriangles;
	}

private:
	std::vector<Triangle> objectTriangles;
	int xPos = 0;
	int yPos = 0;
	int zPos = 0;
	int rotation = 0; //In degrees, clockwise around the y axis
	int width = 0;
	int height = 0;
};

struct PlayerData {
	double xPos = 250;
	double zPos = 250;
	double yPos = 0;
	int rotation = 0; //In degrees, clockwise around the y axis

	void rotate(int rotationAmount) {
		rotation += rotationAmount;
		if (rotation > 360) {
			rotation -= 360;
		}
		if (rotation < 0) {
			rotation = 360 - rotation;
		}
	}

	void moveForward(double distance) {
		int xSignController = 1;
		int zSignController = 1;

		int angle = rotation % 90;

		if (rotation >= 90 && rotation <= 180) {
			zSignController = -1;
			angle = 90 - angle;

		}
		else if (rotation > 180 && rotation < 270) {
			xSignController = -1;
			zSignController = -1;
		}
		else if (rotation >= 270) {
			xSignController = -1;
			angle = 90 - angle;
		}

		double radianAngle = angle * M_PI / 180;
		xPos -= sin(angle) * distance;
		zPos -= cos(angle) * distance;
	}
};

struct CameraData {
	void RePosition(double playerX, double playerZ, double playerY ) {
		xPos = playerX +  (cos(cameraRotation) * verticalOffset);
		zPos = playerZ + (sin(cameraRotation) * horizonatlOffset);
	}

	int horizonatlOffset = 0; //Pls don't use
	int verticalOffset = 0;
	int heightDiffrence = 100;
	double xPos = 0;
	double zPos = 0;
	int yPos = 0;
	int cameraRotation = 0;//Rotation angle from z or x axis depending on y rotation
	int fov = 90;
	int halfFov = fov / 2;
	int cameraDeadAngle = cameraRotation - (fov / 2);


};


class WorldObjectRenderer {
public:
	WorldObjectRenderer() = default;

	WorldObjectRenderer(SDL_Renderer* renderer, int screenWidth, int screenHight) {
		this->renderer = renderer;
		this->screenHight = screenHight;
		this->screenMidHeight = screenHight / 2;
		this->screenWidth = screenWidth;
		this->screenMidWidth = screenWidth / 2;
		
		worldToScreenX = 500 / screenWidth;
		scalingFactor = 0.5f;
	}

	double DistanceCalculator(int xpos1, int ypos1, int xpos2, int ypos2) {
		int xSquared = std::pow(xpos2 - xpos1, 2);
		int ySquared = std::pow(ypos2 - ypos1, 2);
		double distance = std::sqrt(xSquared + ySquared);
		return distance;
	}



	void renderObject(SDL_Renderer* renderer, std::vector<ObjectData*> objects, PlayerData* player, CameraData* camera) {
		
		int playerDirection = player->rotation % 180;
		for (ObjectData* object : objects) {
		//	ObjectData* object = objects[0];
		

			int objectRotation = object->GetRotation();

			if (player->rotation % 90 != 0) {
				//do rotated things
			}

			double distanceHorizontal = 0;
			double distanceVertical = 0;
			
			int xQuadrentModifier = 1; //Changes the sign of the x co-ordinate depending on what quadrent the object is in relative to the camera view
			int zQuadrentModifier = 1; //Same as last line but for y co-ordinate

			double testx = -24;
			double testy = 5;
			double angle = atan2(testx, testy);
			angle = angle* 180 / pi;
			//printf("The angle is: %f \n", angle);

			int objectx = object->GetXPos();
			int objectz = object->GetZPos();

			double diffrenceInX = objectx - camera->xPos;
			double diffrenceInz = objectz - camera->zPos;

			/*if (diffrenceInX < 0.1) {
				diffrenceInX = 0.1;
			}
			if (diffrenceInz < 0.1) {
				diffrenceInz = 0.1;
			}*/

			double diffrenceInAngleToXAxis = atan2(diffrenceInz, diffrenceInX);
			diffrenceInAngleToXAxis =  diffrenceInAngleToXAxis * 180 / pi;
			diffrenceInAngleToXAxis = diffrenceInAngleToXAxis - (camera->cameraRotation%360);
			if (diffrenceInAngleToXAxis > 180) {
				diffrenceInAngleToXAxis = -180 + (diffrenceInAngleToXAxis - 180);
			}
			else if (diffrenceInAngleToXAxis < -180) {
				diffrenceInAngleToXAxis = 180 + (diffrenceInAngleToXAxis + 180);
			}
			else if (diffrenceInAngleToXAxis == 180) {
				diffrenceInAngleToXAxis = 179.9;
			}
			//printf("The angle is after: %f \n", diffrenceInAngleToXAxis);
			

			bool rightSideOfCamera = true;
			if (diffrenceInAngleToXAxis >= 90 || diffrenceInAngleToXAxis <= -90 ) {
				rightSideOfCamera = false;


				if (diffrenceInAngleToXAxis >= 90) {
					xQuadrentModifier = -1;

				}
				else {
					zQuadrentModifier = -1;
				}
			}

			
			if (diffrenceInAngleToXAxis > 180 || diffrenceInAngleToXAxis < -180) {
				//diffrenceInAngleToXAxis = 179;
			}
			printf("The angle is: %f \n", diffrenceInAngleToXAxis);
			//printf("Fucking blaanks \n");
			printf("The player rotation is: %d \n", player->rotation);
			
			diffrenceInAngleToXAxis = std::fmod(diffrenceInAngleToXAxis, 90);
			
			//printf("The current camera rotation is: %d \n", camera->cameraRotation);

			if (diffrenceInAngleToXAxis < 0.1) {
				//diffrenceInAngleToXAxis = 0.1;
			}

			

			double objectDistance = DistanceCalculator(camera->xPos, camera->zPos, objectx, objectz);
			
			if (rightSideOfCamera == true) {
				distanceVertical = sin(diffrenceInAngleToXAxis * pi / 180) * objectDistance; //opp = sin(angle) * hyo
				distanceHorizontal = cos(diffrenceInAngleToXAxis * pi / 180) * objectDistance;
			}
			else {
				printf("Left side of screen \n");
				distanceVertical = cos(diffrenceInAngleToXAxis * pi / 180) * objectDistance; //adj = opp(angle) * hyo
				distanceHorizontal = sin(diffrenceInAngleToXAxis * pi / 180) * objectDistance;
			}



			//printf("The relative y distance is: %f \n", distanceVertical);

			object->SetFlatSquarePosition(screenMidWidth+ distanceHorizontal * xQuadrentModifier, screenMidHeight + -distanceVertical * zQuadrentModifier,
											object->GetWidth(), object->GetHeight(), camera->cameraRotation);

			if ((objectRotation%180) == playerDirection) {
				//Facing the square straight on

				int objectComparison;
				int objectComparisonAlt;
				int cameraComparison;
				int cameraComparisonAlt;

				
				if (playerDirection == 0) {
					objectComparison = object->GetXPos();
					objectComparisonAlt = object->GetZPos();
					cameraComparison = camera->xPos;
					cameraComparisonAlt = camera->zPos;
					distanceHorizontal = object->GetZPos() - camera->zPos;
				}
				else {
					objectComparison = object->GetZPos();
					objectComparisonAlt = object->GetXPos();
					cameraComparison = camera->zPos;
					cameraComparisonAlt = camera->xPos;
					distanceVertical = object->GetXPos() - camera->xPos;
				}

			}
			else {

			}

			int triangles = object->GetTriangles().size();
			//object->SetFlatSquarePosition(200, 100, 200, 100);
			for (int i = 0; i < triangles; i++) {
				SDL_RenderGeometry(renderer, NULL, (object->GetTriangles()[i].triangle), 3, NULL, 0);				
			}
		}



		//SDL_RenderGeometry(renderer, NULL, vert, 3, NULL, 0);
	}


private:
	SDL_Renderer* renderer;
	int screenWidth;
	int screenMidWidth;
	int screenHight;
	int screenMidHeight;
	double pi = 3.14;
	float worldToScreenX;
	float worldToScreenY;
	double radianToDegree = 180.0 / M_PI;
	float scalingFactor;
};





