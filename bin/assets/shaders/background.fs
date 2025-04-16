precision highp float;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	int hasTexture;
};
uniform Material uMaterial;
//uniform vec4 uColor;

uniform mat4 uModelMatrix;
varying vec2 vTexCoord;


void main()
{
	vec4 color = uMaterial.ambient;
	color.xy = vTexCoord.xy;
	float d = (pow(vTexCoord.x-0.5, 2.0) + pow(vTexCoord.y-0.5, 2.0)) * 2.0;
	d = max(0.0, 1.0 - d);
	color.xyz = vec3(0.5,0.5,0.5) * (1.0-d) + color.xyz * d;
	gl_FragColor = color;
}
