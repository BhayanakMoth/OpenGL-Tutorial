#version 330 
layout (location = 0) in vec3 vertPos;
layout (location = 1) out vec3 vertices; 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){

	gl_Position = projection*view*model*vec4(vertPos,1);
	vertices = vertPos;

}
