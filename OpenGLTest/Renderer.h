#pragma once
#include "Renderable.h"
#include <GL/glew.h>
#include "Shader.h"
class Renderer
{
public:
	unsigned int Stride = 0;
	void Render(Renderable obj,Shader shader)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);
		obj.buffer.Bind();
		Stride += sizeof(float) * 3;
		
		if(obj.isIndexed)
			obj.index.Bind();
		if (obj.hasTexture) {
			obj.texture.Bind();
			Stride += sizeof(float) * 2;
		}
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, (void*)0);
		if (obj.hasTexture)
		{
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, Stride, (void*)(sizeof(float) * 3));
		}

		
	}
};