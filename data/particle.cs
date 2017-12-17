#version 430 core

layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding = 0) buffer PositionBuffer
{
    vec3 positions[];
};

layout (std430, binding = 1) buffer VelocityBuffer
{
    vec3 velocities[];
};

layout (std430, binding = 2) buffer TtlBuffer
{
	float ttls[];
};

uniform float dt;
uniform float speed;

void main(void) 
{
	uint id = gl_GlobalInvocationID.x;
	vec3 pos = positions[id];
	vec3 vel = velocities[id];
	float ttl = ttls[id];
	
	float nttl = ttl - dt;
	vec3 npos = pos + vel * dt;
	/*
	if(nttl <= 0.0) 
	{
		npos = vec3(0.0, 0.0, 0.0);
		nttl = 2.5;
	}
	*/
	
	positions[id] = vec3(float(id) / 128.0, 0, 0);
	velocities[id] = vel;
	ttls[id] = ttl;
}

