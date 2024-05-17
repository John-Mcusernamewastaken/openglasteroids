#pragma once

#include "WorldObject.hpp"
#include "windows.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"


WorldObject::WorldObject()
{
	OutputDebugStringW(L"FATAL: called invalid constructor MyWorldThing()\n");
	exit(1);
}
WorldObject::WorldObject(Model* model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, vector<tag>* tags)
{
	this->model = model;
	this->position = position;
	this->angle = angle;
	this->scale = scale;
	this->projectionLocation = projectionLocation;
	this->viewLocation = viewLocation;
	this->modelLocation = modelLocation;
	this->tags = tags;
	this->markedForDelete = false;
	boundingBox = NULL;
	boundingBoxAngle = glm::vec3(-1,-1,-1);
}

WorldObject::~WorldObject()
{
	delete tags;
}

void WorldObject::Draw()
{
	/*
		22.5.22
		rotation works correctly
		translation does nothing but doesn't cause drawing errors
		projection breaks everything

		24.5.22
		it's working!
	*/
	//calculate matrices
		//THESE ARE IN REVERSE ORDER FOR A REASON. BECAUSE OPENGL USES COLUMN-MAJOR MATRICES.
		//projection
			//init with identity
			glm::mat4 projectionMatrix = glm::ortho(
				-1.0f, 1.0f,	//left-right
				-1.0f, 1.0f,	//bottom-top
				1.0f, 100.0f	//near-far
			);
			//glm::mat4 projectionMatrix = glm::perspective(75.0f, 1.0f, 1.0f, 100.0f);
		//view
			//init with identity
			glm::mat4 viewMatrix(1.0f);
		//model
			//init with identity
			glm::mat4 modelMatrix(1.0f);
			//translate
			modelMatrix = glm::translate(modelMatrix, position);
			//rotate
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle.x), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle.y), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle.z), glm::vec3(0, 0, 1));
			//scale
			modelMatrix = glm::scale(modelMatrix, scale);
	//pass matrices to shader
		glad_glUniformMatrix4fv(
			projectionLocation,
			1,
			GL_FALSE,
			glm::value_ptr(projectionMatrix)
		);
		glad_glUniformMatrix4fv(
			viewLocation,
			1,
			GL_FALSE,
			glm::value_ptr(viewMatrix)
		);
		glad_glUniformMatrix4fv(
			modelLocation,
			1,
			GL_FALSE,
			glm::value_ptr(modelMatrix)
		);
	//draw
	model->Draw();
}

glm::vec3* WorldObject::getBoundingBox()
{
	if (angle!=boundingBoxAngle) { //recalculate the bounding box
		if (boundingBox != NULL) //delete any existing bounding box
		{
			delete[] boundingBox;
			boundingBox = NULL;
		}
		//construct the rotation matrix
		glm::mat4 rotationMatrix = glm::mat4(1.0); //create identity matrix
		rotationMatrix = glm::scale(rotationMatrix, this->scale);
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle.x), glm::vec3(1,0,0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle.y), glm::vec3(0,1,0));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle.z), glm::vec3(0,0,1));

		//rotate the verts
		boundingBox = new glm::vec3[8];
		//calc bounding box. get the min and maximum x, y, & z coordinates of all vertices in the all meshes
		for (char i = 0;i < 8;i++)
		{
			//rotate & scale the vert (we'll translate/reposition the final bounding box afterward)
			boundingBox[i] = glm::vec3(rotationMatrix * model->boundingBox[i]);
			boundingBox[i].x += position.x;
			boundingBox[i].y += position.y;
			boundingBox[i].z += position.z;
		}
	}
	return boundingBox;
}