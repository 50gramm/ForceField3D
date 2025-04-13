#version 330 core
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
uniform Material uMaterial2;

struct PointLight
{
	vec4 position;

	float constant;
	float linear;
	float quadratic;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
#define MAX_LIGHTS_NUM 4
uniform PointLight uPointLights[MAX_LIGHTS_NUM];
uniform int uNumLights;

uniform vec3 uEyePos;
uniform mat4 uModelMatrix;
uniform sampler2D uTextureSampler;
uniform float uTime;
varying vec2 vTexCoord;
varying vec3 vModelPos;
varying vec3 vNormal;
varying float vTangential;
varying float vTangentialNormalized;


vec4 calcColor(PointLight light, Material material, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position.xyz - light.position.w * fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = (reflect(lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), sqrt(material.shininess));

	float attenuation = 1.0;
	if(light.position.w != 0.0)
	{
		float distance = length(light.position.xyz - fragPos);
		attenuation = light.constant
			+ light.linear / distance
			+ light.quadratic / (distance * distance);
	}

	return (light.ambient * material.ambient
		+ light.diffuse * diff * material.diffuse
		+ light.specular * spec * material.specular)
		* attenuation;
}


void main()
{
	Material material = uMaterial;
	Material material2 = uMaterial2;
	if(material.hasTexture != 0)
	{
		vec4 texel = texture2D(uTextureSampler, vTexCoord);
		material.ambient *= texel;
		material.diffuse *= texel;
		material.specular *= texel;

		material2.ambient *= texel;
		material2.diffuse *= texel;
		material2.specular *= texel;
	}

	vec3 norm = normalize(vNormal * mat3(uModelMatrix));
	vec3 viewDir = normalize(vModelPos - uEyePos);

	float q = vTangentialNormalized;//sin(10*vTangential - 2*uTime);
	material.ambient = q*material.ambient + (1.0-q)*material2.ambient;
	material.diffuse = q*material.diffuse + (1.0-q)*material2.diffuse;
	material.specular = q*material.specular + (1.0-q)*material2.specular;
	material.shininess = q*material.shininess + (1.0-q)*material2.shininess;

	vec4 lightColor = vec4(0,0,0,0);
	#define applyLight(i)	if(i < uNumLights) lightColor += calcColor(uPointLights[i], material, norm, viewDir, vModelPos);
	applyLight(0)
	applyLight(1)
	applyLight(2)
	applyLight(3)

	gl_FragColor = vec4(lightColor.xyz, material.ambient.w);
}
