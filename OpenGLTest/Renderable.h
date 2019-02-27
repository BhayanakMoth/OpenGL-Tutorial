#pragma once
#include<glm/gtx/transform.hpp>
#include<vector>
#include "VertexBuffer.hpp"
#include "IndexBuffer.h"
#include "Texture.h"
class Renderable
{

public:
	glm::mat4 model;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	bool isIndexed = false;
	VertexBuffer buffer;
	IndexBuffer   index;
	Texture texture;
	bool hasTexture = false;
	bool hasNormals = false;
	Renderable(glm::vec3 pos, bool indexed)
	{
		model = glm::translate(model, pos);
		isIndexed = indexed;
	}
	void SetVertexBuffer(VertexBuffer buff)
	{
		buffer = buff;
	}
	void SetIndexBuffer(IndexBuffer buff)
	{
		index = buff;
	}
	void SetTexture(Texture tex)
	{
		texture = tex;
	}
	
};
