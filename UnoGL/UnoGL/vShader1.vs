#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vertexColor;

void main()
{
   gl_Position = vec4(aPos.xyz, 1.0); //'swizzling' is cool
   vertexColor = aColor; //sets vertex color to the 'other' vec3 defined in the vertices data 
}