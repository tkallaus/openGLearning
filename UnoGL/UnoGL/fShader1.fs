#version 330 core

out vec4 FragColor;
in vec3 vertexColor; //the input from the above vertexShader, matching name and type
uniform float gloColor; //global uniform color

void main()
{
   //FragColor = vec4(vertexColor, 1.0);
   FragColor = vec4(vertexColor.x - gloColor, vertexColor.y + 1-gloColor, vertexColor.z - gloColor, 1.0); //cool new technique makes a wild oscillating color rectangle
}