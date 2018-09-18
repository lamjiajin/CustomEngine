#version 330 core

uniform sampler2D colorTex;     /*  Base color texture */
uniform sampler2D normalTex;    /*  Normal texture for normal mapping */
uniform sampler2D bumpTex;      /*  Bump texture for bump mapping */

in vec2 uvCoord;

uniform bool lightOn;
uniform int numLights;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform int specularPower;

/*  These could be in view space or tangent space */
in vec3 lightDir[10];
in vec3 viewDir;
in vec3 normal;

uniform bool normalMappingOn;   /*  whether normal mapping is on */
uniform bool parallaxMappingOn; /*  whether parallax mapping is on */


out vec4 fragColor;


void main(void)
{
    fragColor = vec4(texture(colorTex, uvCoord).rgb, 1.0);
    if (!lightOn)
    {
        return;
    }

    vec4 light = ambient;

    //@TODO
    /*  If normal mapping is off, then use the interpolated normal from vertex shader.
        Otherwise, sample the normal from normal map.

        If parallax mapping is off, then uvCoord should be used for all the texture sampling.
        Otherwise, we should use the adjusted uvCoord.
        Adjusted uvCoord is set to be uvCoord + h * viewDir.xy,
        where h is height from bump map * 0.15 - 0.005.

        For lighting, Blinn-Phong model should be used.
    */


    vec3 n_normal;
    if (!normalMappingOn)
        n_normal= vec3(normalize(normal));
    else
        n_normal = normalize(vec3(texture2D(normalTex, uvCoord).rgb) * 2.f - 1.f);

    // normalize view dir
    vec3 viewVF_ = normalize(viewDir);
    // parallax mapping, get new uv
    if (parallaxMappingOn)
    {
        float height = texture(bumpTex, uvCoord).r * 0.15 - 0.005;
        vec2 uvCoordNew = uvCoord + height * viewVF_.xy;
        fragColor = vec4(texture(colorTex, uvCoordNew).rgb, 1.0);
    }

     for (int i = 0; i < numLights; ++i)
    {
        vec3 lightVF_ = normalize(lightDir[i]);
        vec3 h = normalize(viewVF_+lightVF_);
        float ldotn = max(dot(n_normal,lightVF_),0);
        float hdotn = max(dot(h,n_normal),0);
        hdotn = pow(hdotn,specularPower);
        light += ldotn*diffuse + hdotn*specular;
    }





    /*  ... */

    fragColor = fragColor * light;
}