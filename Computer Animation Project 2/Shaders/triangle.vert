#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraMatrices {
    mat4 view;
    mat4 proj;
	vec3 pos;
} cam;

layout(set = 1, binding = 1) uniform ObjectMatrix {
    mat4 model;
} obj;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragPosition;
layout(location = 3) out vec3 viewPosition;

out gl_PerVertex {
    vec4 gl_Position;
};

float getSign(float f) {
	return f < 0 ? -1 : 1;
}

void main() {
	vec4 worldPos = obj.model * vec4(inPosition, 1.0);

	fragPosition = worldPos.xyz;

	vec4 viewPos = cam.view * worldPos;
	viewPos.z *= -1;

	gl_Position = cam.proj * viewPos;
	fragTexCoord = inTexCoord;
    
	vec4 outNormal = obj.model * vec4(inNormal, 0);

	//float negativeScale = getSign(obj.model[0][0] * obj.model[1][1] * obj.model[2][2]);
    
	fragNormal = outNormal.xyz;// * negativeScale;

	viewPosition = cam.pos;
}