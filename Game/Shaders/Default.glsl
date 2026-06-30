#define vertex

#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_UV;

uniform mat4 u_Transform;
uniform mat4 u_Proj;
uniform mat4 u_View;

out vec4 v_Color;
out vec2 v_UV;

void main()
{
    //gl_Position = u_CamTransform * u_View * u_CamTransform * vec4(aPosition, 1.0f);
    v_UV = a_UV;
    v_Color = a_Color;
    gl_Position = u_Proj * u_View * u_Transform * vec4(a_Position, 1.0f);
}

#define fragment

#version 330 core

in vec4 v_Color;
in vec2 v_UV;

out vec4 FragColor;

void main()
{
    FragColor = v_Color;
}
