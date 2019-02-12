#version 330

uniform vec3 eye_pos;
uniform vec3 light_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec4 projected_position;
in vec4 transposed_normal;

out vec4 color;

struct pLight {
	vec3 position;
	vec3 ambiant;
	vec3 diffuse;
	vec3 specular;
};

vec3 refl(vec3 N, vec3 L)
{
	return 2.0 * dot(L, N) * N - L;
}

vec3 phong(vec3 hit, vec3 eye, vec3 N, pLight light, float ks)
{
	vec3 L = normalize(light.position - hit);
	vec3 V = normalize(eye - hit);
	vec3 R = refl(N, L);
	vec3 ambiant = light.ambiant;
	vec3 diffuse = max(dot(L, N), 0.0) * light.diffuse;
	vec3 specular = pow(max(dot(R, V), 0.0), ks) * light.specular;
	return ambiant + 0.5 * (diffuse + specular);
}

void main()
{
	pLight l1 = pLight(
		(vec4(light_pos, 1.0) * model * view).xyz,
		vec3(0.1), vec3(0.5), vec3(0.8));

	color = vec4(phong(projected_position.xyz, eye_pos, transposed_normal.xyz, l1, 1.0), 1.0);
}