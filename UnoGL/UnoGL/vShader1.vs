#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 texCoord;

//Temp for move back and forth
uniform float texMove;

void main()
{
   gl_Position = vec4(aPos.x + texMove, aPos.yz, 1.0); //'swizzling' is cool
   vertexColor = aColor; //sets vertex color to the 'other' vec3 defined in the vertices data 
   texCoord = aTexCoord;
}