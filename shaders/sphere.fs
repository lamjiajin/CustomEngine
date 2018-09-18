#version 330 core

uniform samplerCube texCube;

uniform mat4 viewMat;

uniform int sphereRef;

in vec3 view, normal;


out vec4 fragColor;


void main(void)
{
    //@TODO
    /*  Compute reflected/refracted vectors in view frame for higher accuracy, 
        then transform them back to world frame for texture sampling.

        If sphereRef = 0 then there's only reflection.
        If sphereRef = 1 then there's only refraction.
        If sphereRef = 2 then there are both reflection and refraction.
        In that case, assume that reflection accounts for 70% of the color.
		
		The refractive index for the sphere is 1.5. For simplicity, we also
		assume that each ray going into the sphere is refracted only once.
    */
    /*  ... */

    /* normalize the normal first */
    vec3 n = normalize(normal);

    /* normalize the view vector */
    vec3 viewN = normalize(view);

    vec3 reflection;
    vec3 refraction;

    if(sphereRef == 0)
    {
        reflection = mat3(transpose(viewMat)) * reflect(view , n); 
        fragColor = texture(texCube, reflection);
        return;
    }
    else if(sphereRef == 1)
    {
        refraction = mat3(transpose(viewMat)) * refract(viewN , n , 1.0/1.5);
        fragColor = texture(texCube, refraction);
        return;
    }
    else if(sphereRef == 2)
    {
       reflection = mat3(transpose(viewMat)) * reflect(view , n);
       refraction = mat3(transpose(viewMat)) * refract(viewN , n ,1.0/ 1.5);
       fragColor = 0.7f * texture(texCube, reflection) + 0.3f * texture(texCube, refraction);
       return;
    }
}