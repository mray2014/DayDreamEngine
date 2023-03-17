
#define VERT_LOCATION_OFFSET 3

struct DirectionalLight{
	vec3 lightDir;
	float pad;
	vec3 lightColor;
	float pad2;
};

struct PointLight{
	vec3 lightPos;
	vec3 lightColor;
	float size;
};