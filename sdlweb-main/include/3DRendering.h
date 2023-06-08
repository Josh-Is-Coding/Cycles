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
		printf("the number of triangles is %f \n", objectTriangles[0].triangle->position.y);

		this->objectTriangles = objectTriangles;

		printf("pushing p %f\n", this->objectTriangles.size());

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
	int yOffset = 50;
	int zOffset = -200;
	int xPos = 0;
	int zPos = 0;
	int yPos = 0;
	int cameraRotation = 75;//Rotation angle from z or x axis depending on y rotation
};


class WorldObjectRenderer {
public:
	WorldObjectRenderer() = default;

	WorldObjectRenderer(SDL_Renderer* renderer, int screenWidth, int screenHight, int fov) {
		this->renderer = renderer;
		this->screenHight = screenHight;
		this->screenMidHeight = screenHight / 2;
		this->screenWidth = screenWidth;
		this->screenMidWidth = screenWidth / 2;
		this->fov = ((double)fov)/2;
		this->tanFov = (int)(tan(this->fov));

		worldToScreenX = 500 / screenWidth;
		scalingFactor = 0.5f;
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
				int cameraComparison;
				int maxLimit;

				
				if (playerDirection == 0) {
					objectComparison = object->GetXPos();
					cameraComparison = camera->xPos;
					distance = object->GetZPos() - camera->zPos;
				}
				else {
					objectComparison = object->GetZPos();
					cameraComparison = camera->zPos;
					distance = object->GetXPos() - camera->xPos;
				}

				int renderObjectXPos = (int)(screenMidWidth + (objectComparison - cameraComparison));
				object->SetFlatSquarePosition(renderObjectXPos, 400, object->GetWidth() * (1.0f - scalingFactor * (distance-1)*0.001f), object->GetHeight() * (1.0f - scalingFactor * (distance - 1) * 0.001f));
				object->SetLeftEdge(100, 200, 0, 200);
				object->SetTopEdge(300, 200 , 600, 0);
				object->SetRightEdge(500, 200, 0, 400);
				object->SetBottomtEdge(300, 300,500,0);
			}
			else {
				// square is to the side
			}

			//object->SetFlatSquarePosition(200, 100, 200, 100);
			for (int i = 0; i < triangles; i++) {
				printf("distance is:%d \n", distance);
				if (distance >= 0) {
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
	double fov;
	float worldToScreenX;
	float worldToScreenY;
	int tanFov;

	float scalingFactor;
};





