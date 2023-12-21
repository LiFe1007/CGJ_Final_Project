// STEP 7 - Combining all together.
// Copyright (c) 2015-23 by Carlos Martinho

#version 330 core

uniform mat4 ModelMatrix;
layout(std140) uniform Camera 
{	mat4 ViewMatrix;
	mat4 ProjectionMatrix;	};

uniform float Time0_1;
const float PI = 3.14159265358979323846264;

in vec3 inPosition;
in vec3 inNormal;

out vec3 exNormal;


void main(void)
{
	exNormal = inNormal;

	vec4 mcPosition = vec4(inPosition, 1.0);

    mcPosition.y = (
    	sin(mcPosition.x * 2.0 + Time0_1 * 2.0 * PI) +
    	sin(mcPosition.x * 0.5 + Time0_1 * 3.0 * PI)
    ) * 0.5;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * mcPosition;
}
