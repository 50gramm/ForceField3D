#version 330 core

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

	//float shininess;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
#define NR_POINT_LIGHTS 4  
uniform PointLight uPointLights[NR_POINT_LIGHTS];
uniform int uNumLights = 0;

uniform vec3 uEyePos;
uniform mat4 uModelMatrix;
uniform sampler2D uTextureSampler;
uniform float uTime = 0;
varying vec2 vTexCoord;
varying vec3 vModelPos;
varying vec3 vNormal;
varying float vTangential;
varying float vTangentialNormalized;
//varying vec4 lightPos = vec4(3,2,1,1);
//vec3 lightDir = normalize(vec3(3,2,1));


vec4 calcDirLight(PointLight light, Material material, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize((light.position.xyz));
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = (reflect(lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), sqrt(material.shininess));

	return light.ambient * material.ambient
		+ light.diffuse * diff * material.diffuse
		+ light.specular * spec * material.specular;
}


vec4 calcPointLight(PointLight light, Material material, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position.xyz - fragPos);
	
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = (reflect(lightDir, normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), sqrt(material.shininess));

	float distance = length(light.position.xyz - fragPos);
	float attenuation = 1.0 / (light.constant
		+ light.linear * distance
		+ light.quadratic * (distance * distance));    

	return (light.ambient * material.ambient
		+ light.diffuse * diff * material.diffuse
		+ light.specular * spec * material.specular)
		* attenuation;
}


void main()
{
	vec3 norm = normalize(vNormal * mat3(uModelMatrix));
	vec3 viewDir = normalize(vModelPos - uEyePos);

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

	float q = vTangentialNormalized;//sin(10*vTangential - 2*uTime);
	material.ambient = q*material.ambient + (1-q)*material2.ambient;
	material.diffuse = q*material.diffuse + (1-q)*material2.diffuse;
	material.specular = q*material.specular + (1-q)*material2.specular;
	material.shininess = q*material.shininess + (1-q)*material2.shininess;

	vec4 lightColor = vec4(0,0,0,0);
	for(int i=0; i<uNumLights; ++i)
	{
		if(uPointLights[i].position.w == 0.0)
			lightColor += calcDirLight(uPointLights[i], material, norm, viewDir);
		else
			lightColor += calcPointLight(uPointLights[i], material, norm, viewDir, vModelPos);
	}

	gl_FragColor = vec4(lightColor.xyz, uMaterial.ambient.w);
}
