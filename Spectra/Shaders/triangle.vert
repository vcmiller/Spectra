#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	vec4 viewPos = ubo.view * ubo.model * vec4(inPosition, 1.0);
	viewPos.z *= -1;

    gl_Position = ubo.proj * viewPos;
    fragTexCoord = inTexCoord;
}