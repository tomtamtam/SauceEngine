#define vertex

#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

uniform mat4 u_Transform;
uniform mat4 u_CamTransform;
uniform mat4 u_View;

void main()
{
    //gl_Position = u_CamTransform * u_View * u_CamTransform * vec4(aPosition, 1.0f);
    gl_Position = u_Transform * vec4(aPosition, 1.0f);
}

#define fragment

#version 330 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}
