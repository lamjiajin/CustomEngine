#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
/*  For the sphere, we don't need other vertex attributes */

/* model to view */
uniform mat4 mvMat;
uniform mat4 nmvMat;
uniform mat4 projMat;

out vec3 view, normal;

void main(void)
{
    //@TODO
    /*  
		Compute the vertex position, normal and view direction in view frame.
        For reflection/refraction, we assume the sphere "camera" is at 
        the center of its frame, (0, 0, 0).
        So the view direction is actually the vertex position.
    */
	view = vec3(mvMat * vec4(pos,1));
	normal = vec3(nmvMat * vec4(nrm,1));
	gl_Position = projMat * mvMat * vec4(pos,1); 
    /*  ... */
}