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

	void SetAll(const std::vector<Triangle>& objectTriangles, int xPos, int zPos, int yPos, int rotation, int width, int height) {

		this->objectTriangles = objectTriangles;


		this->xPos = xPos;
		this->zPos = zPos;
		this->yPos = yPos;
		this->rotation = rotation;
		this->height = height;
		this->width = width;
	}

	void SetFlatSquarePosition(int xPos, int yPos, int width, int height) {
		int halfWidth = width / 2;
		int halfHeight = height / 2;
		objectTriangles[0].triangle[0].position.x = xPos - halfWidth; objectTriangles[0].triangle[0].position.y = yPos - halfHeight;
		objectTriangles[0].triangle[1].position.x = xPos - halfWidth; objectTriangles[0].triangle[1].position.y = yPos + halfHeight;
		objectTriangles[0].triangle[2].position.x = xPos + halfWidth; objectTriangles[0].triangle[2].position.y = yPos + halfHeight;

		objectTriangles[1].triangle[0].position.x = xPos - halfWidth; objectTriangles[1].triangle[0].position.y = yPos - halfHeight;
		objectTriangles[1].triangle[1].position.x = xPos + halfWidth; objectTriangles[1].triangle[1].position.y = yPos - halfHeight;
		objectTriangles[1].triangle[2].position.x = xPos + halfWidth; objectTriangles[1].triangle[2].position.y = yPos +halfHeight;
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
	int xPos = 250;
	int zPos = 500;
	int yPos = 0;
	int rotation = 0; //In degrees, clockwise around the y axis
};

struct CameraData {
	int xOffset = 0;
	int yOffset = 400;
	int zOffset = -200;
	int xPos = 0;
	int zPos = 0;
	int yPos = 0;
	int cameraRotation = 75;//Rotation angle from z or x axis depending on y rotation
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
			int triangles = object->GetTriangles().size();

			int objectRotation = object->GetRotation();

			if (player->rotation % 90 != 0) {
				//do rotated things
			}

			int distance = 0;
			if ((objectRotation&180) == playerDirection) {
				//Facing the square straight on

				int objectComparison;
				int objectComparisonAlt;
				int cameraComparison;
				int cameraComparisonAlt;
				int maxLimit;

				
				if (playerDirection == 0) {
					objectComparison = object->GetXPos();
					objectComparisonAlt = object->GetZPos();
					cameraComparison = camera->xPos;
					cameraComparisonAlt = camera->zPos;
					distance = object->GetZPos() - camera->zPos;
				}
				else {
					objectComparison = object->GetZPos();
					objectComparisonAlt = object->GetXPos();
					cameraComparison = camera->zPos;
					cameraComparisonAlt = camera->xPos;
					distance = object->GetXPos() - camera->xPos;
				}

				//calulate top side
				int topEdgexDistance = objectComparisonAlt - cameraComparisonAlt;
				double topEdgeyDistance = (double)camera->yPos - ((double)object->GetYPos() + (double)object->GetHeight()/2);
				double oppOverAdjacent = (double)topEdgexDistance / topEdgeyDistance;
				double subtractAngle = std::atan(oppOverAdjacent);
				subtractAngle *= radianToDegree;
				double realAngle = camera->halfFov - (subtractAngle - camera->cameraDeadAngle);
				//double realAngle = 45 - subtractAngle;
				
				double lineDistance = DistanceCalculator(cameraComparisonAlt, camera->yPos, objectComparison, object->GetYPos());
				double realAngleTan = std::tan(realAngle* (M_PI/180));
			 
				double relativeYPositionFromCenter = realAngleTan * lineDistance;
				
				
				int topLeftVetexScale = lineDistance * 0.0005 * 90;
				printf("THe debug distance is:%d \n", topLeftVetexScale);


				int renderObjectXPos = (int)(screenMidWidth + (objectComparison - cameraComparison));
				object->SetFlatSquarePosition(renderObjectXPos, 400, object->GetWidth() * (1.0f - scalingFactor * (distance-1)*0.001f), object->GetHeight() * (1.0f - scalingFactor * (distance - 1) * 0.001f));
				//object->SetLeftEdge(300, screenMidHeight + relativeYPositionFromCenter, 0, 200);
				//object->SetTopEdge(500, 200 , 500, 0);
				//object->SetRightEdge(500, screenMidHeight + relativeYPositionFromCenter, 0, 200);
				//object->SetBottomtEdge(300, 300,500,0);
				object->SetTopLeftVertex((object->GetXPos() - object->GetWidth()/2) * topLeftVetexScale, (screenMidHeight + relativeYPositionFromCenter) * topLeftVetexScale);
				object->SetTopRightVertex(500, screenMidHeight + relativeYPositionFromCenter);
				object->SetBottomRightVertex(600, 500);
				object->SetBottomLeftVertex(50, 550);

			}
			else {
				// square is to the side
			}

			//object->SetFlatSquarePosition(200, 100, 200, 100);
			for (int i = 0; i < triangles; i++) {
				if (distance != 0) {
					SDL_RenderGeometry(renderer, NULL, (object->GetTriangles()[i].triangle), 3, NULL, 0);
				}
				
				//printf("triangles point x:%f  y:%f \n", object->GetTriangles()[0].triangle[i].position.x, object->objectTriangles[0].triangle[i].position.y);

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

	float worldToScreenX;
	float worldToScreenY;
	double radianToDegree = 180.0 / M_PI;
	float scalingFactor;
};





