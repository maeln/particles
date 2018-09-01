#version 430 core

#define GRAVITY 9.81
#define GRAVITY_DIRECTION vec4(0.0, -1.0, 0.0, 1.0)

layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout (std140, binding = 0) buffer PositionBuffer
{
    vec4 positions[];
};

layout (std140, binding = 1) buffer VelocityBuffer
{
    vec4 velocities[];
};

layout (std430, binding = 2) buffer TtlBuffer
{
	float ttls[];
};

uniform float dt;
uniform float speed;

float random(vec2 st) 
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);
}

void main(void) 
{
	uint id = gl_GlobalInvocationID.x;
	vec4 pos = positions[id];
	vec4 vel = velocities[id] + GRAVITY_DIRECTION*(GRAVITY*0.001);
	float ttl = ttls[id];

	float nttl = ttl - dt;
	vec4 npos = pos + vel * dt;
	
	if(nttl <= 0.0) 
	{
		npos = vec4(0.0, 0.0, 0.0, 1.0);
		nttl = 2.5;
		vel = vec4(random(vel.yx) - random(vel.xy), random(vel.yz), random(vel.zx)-random(vel.xz), 1.0);
	}
	
	
	//positions[id] = vec3(float(id) / 128.0, 0, 0);
	positions[id] = npos;
	velocities[id] = vel;
	ttls[id] = nttl;
}

