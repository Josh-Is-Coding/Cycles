#pragma once
#include <SDL2/SDL.h>
#include<vector>

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

	void SetYpos() {

	}

	int GetXPos() {
		return xPos;
	}

	int GetYPos() {
		return yPos;
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
	int xPos;
	int yPos;
	int rotation = 0; //In degrees, clockwise around the y axis
};


class WorldObjectRenderer {
public:
	WorldObjectRenderer() = default;

	WorldObjectRenderer(SDL_Renderer* renderer, int screenWidth, int screenHight) {
		this->renderer = renderer;
		this->screenHight = screenHight;
		this->screenWidth = screenWidth;
	}

	void renderObject(SDL_Renderer* renderer, ObjectData* object, PlayerData* player) {
		int triangles = object->GetTriangles().size();
		int objectX = object->GetXPos();
		int objectY = object->GetYPos();
		int objectRotation = object->GetRotation();
		object->SetFlatSquarePosition(100, 100, 200, 100);
		for (int i = 0; i < triangles; i++) {
			printf("ths runns \n");
			SDL_RenderGeometry(renderer, NULL, (object->GetTriangles()[i].triangle), 3, NULL, 0);
			//printf("triangles point x:%f  y:%f \n", object->GetTriangles()[0].triangle[i].position.x, object->objectTriangles[0].triangle[i].position.y);

		}


		//SDL_RenderGeometry(renderer, NULL, vert, 3, NULL, 0);
	}


private:
	SDL_Renderer* renderer;
	int screenWidth;
	int screenHight;
};





