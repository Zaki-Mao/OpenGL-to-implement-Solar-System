//Mao Zunjie Wang Haocheng 18098531-i011-0019 18098537-i011-0112
#version 430

in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec2 tc;

out vec4 fragColor;

layout (binding=0) uniform sampler2D t;
layout (binding=1) uniform sampler2D n;
layout (binding=2) uniform sampler2D h;

struct PositionalLight
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
};

struct Material
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;	 
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform int sun;

vec3 CalcBumpedNormal()
{
	vec3 Normal = normalize(varyingNormal);
	return Normal;
	vec3 Tangent = normalize(varyingTangent);
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	vec3 Bitangent = cross(Tangent, Normal);
	vec3 BumpMapNormal = texture(n,tc).xyz;
	BumpMapNormal = BumpMapNormal * 2.0 - 1.0;
	mat3 TBN = mat3(Tangent, Bitangent, Normal);
	vec3 NewNormal = TBN * BumpMapNormal;
	NewNormal = normalize(NewNormal);
	return NewNormal;
}

void main(void)
{
		if(sun==1)
		{
			vec4 tcc = texture(t,tc);
			fragColor = tcc*1.3;
			return;
		}
// normalize the light, normal, and view vectors:
	vec3 L = normalize(varyingLightDir-varyingVertPos);
	vec3 V = normalize(-varyingVertPos);

	//vec3 N = CalcBumpedNormal();
	vec3 N = CalcBumpedNormal();

	// get the angle between the light and surface normal:
	float cosTheta = dot(L,N);
	
	// compute light reflection vector, with respect N:
	vec3 R = normalize(reflect(-L, N));
	
	// angle between the view vector and reflected light:
	float cosPhi = dot(V,R);
	//fragColor = vec4(N,1);
	//return;
	// compute ADS contributions (per pixel):
	vec4 tcc = texture(t,tc);
	fragColor =
	0.25*tcc +
		 2*tcc*(fragColor = globalAmbient * material.ambient
			+ light.ambient * material.ambient
			+ light.diffuse * material.diffuse * max(cosTheta,0.0)
			+ light.specular  * material.specular
				* pow(max(cosPhi,0.0), material.shininess));
}
