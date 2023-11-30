#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 texCoord;

//Temp for move back and forth
uniform mat4 texMove;

void main()
{
   gl_Position = texMove * vec4(aPos.xyz, 1.0); //'swizzling' is cool
   vertexColor = aColor; //sets vertex color to the 'other' vec3 defined in the vertices data 
   texCoord = aTexCoord;
}