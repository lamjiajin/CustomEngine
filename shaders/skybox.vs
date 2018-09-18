#version 330 core

uniform mat4 viewMat;

out vec3 texCoord;
//screen coords
const vec2 vert[4] =  vec2[4](
							vec2(-1.0f,  -1.0f),
							vec2(1.0f,  -1.0f),
							vec2(-1.0f,  1.0f),
							vec2(1.0f,  1.0f)
						);
void main(void)
{
    //@TODO
    /*  Render background using a full-screen quad defined as the far plane in NDC frame.

        This quad texture coordinates are defined in world-frame as the 4 back corners 
        of a 2x2x2 box symmetric about the world origin.
    */
    /*  ... */
	
	texCoord = mat3(transpose(viewMat)) * vec3(vert[gl_VertexID], -1.f);
    gl_Position  =  vec4(vert[gl_VertexID], 1.f, 1.f);
}