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

struct ObjectData {
	ObjectData() {
		xPos = 0;
		yPos = 0;
		rotation = 0;
	};

	ObjectData(std::vector<Triangle> objectTriangles , int xPos, int yPos, int rotation) {
		this->objectTriangles = objectTriangles;
		this->xPos = xPos;
		this->yPos = yPos;
		this->rotation = rotation;
	}

	void SetAll(const std::vector<Triangle>& objectTriangles, int xPos, int yPos, int rotation) {
		printf("the number of triangles is %f \n", objectTriangles[0].triangle->position.y);

		this->objectTriangles = objectTriangles;
		//for (int i = 0; i < objectTriangles.size(); i++) {
		//	this->objectTriangles.push_back(objectTriangles[i]);
		//	printf("adding new point \n");
		//}
		printf("pushing p %f\n", this->objectTriangles.size());
		//this->objectTriangles = objectTriangles;
		this->xPos = xPos;
		this->yPos = yPos;
		this->rotation = rotation;
	}


	std::vector<Triangle> objectTriangles;
	int xPos = 0;
	int yPos = 0;
	int rotation = 0; //In degrees, clockwise around the y axis
};

struct PlayerData {
	int xPos;
	int yPos;
	int rotation = 0; //In degrees, clockwise around the y axis
};


void renderObject(SDL_Renderer* renderer, ObjectData object /*, PlayerData* player*/) {
	int triangles = object.objectTriangles.size();
	for (int i = 0; i < 3; i++) {
		printf("ths runns \n");
		printf("triangles point x:%f  y:%f \n", object.objectTriangles[0].triangle[i].position.x, object.objectTriangles[0].triangle[i].position.y);
		
		//for (int j = 0; j < object->objectTriangles[i].triangle.size() - 1; j++) {
		//	SDL_RenderGeometry(renderer, NULL, &(object->objectTriangles[i].triangle[j]), 3, NULL, 0);
		//}
		
	}
	SDL_RenderGeometry(renderer, NULL, (object.objectTriangles[0].triangle), 3, NULL, 0);
	SDL_RenderGeometry(renderer, NULL, (object.objectTriangles[1].triangle), 3, NULL, 0);

	SDL_Vertex vert[3];
	SDL_Color c{ 175, 178, 255, 200 };
	// center
	vert[0].position.x = 100;
	vert[0].position.y = 100;
	vert[0].tex_coord.x = 0;
	vert[0].tex_coord.y = 0;
	vert[0].color = c;

	// left
	vert[1].position.x = 100;
	vert[1].position.y = 200;
	vert[1].tex_coord.x = 0;
	vert[1].tex_coord.y = 1;
	vert[1].color = c;

	// right 
	vert[2].position.x = 300;
	vert[2].position.y = 200;
	vert[2].tex_coord.x = 1;
	vert[2].tex_coord.y = 1;
	vert[2].color = c;

	//SDL_RenderGeometry(renderer, NULL, vert, 3, NULL, 0);
}

