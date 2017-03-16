#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 2, binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightDirWorld = normalize(vec3(1, -1, 1));
    float diffuse = clamp(dot(normalize(fragNormal), -lightDirWorld), 0, 1);

    outColor = texture(texSampler, fragTexCoord) * diffuse;
}