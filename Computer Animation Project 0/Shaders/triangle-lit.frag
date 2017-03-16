#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 2, binding = 2) uniform sampler2D texSampler;
layout(set = 3, binding = 3) uniform LightInfo {
    vec4 pos;
    vec3 color;
    float intensity;
    float range;
} light;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightDirWorld = normalize(light.pos.xyz);
    float diffuse = clamp(dot(normalize(fragNormal), -lightDirWorld), 0, 1);

    outColor = texture(texSampler, fragTexCoord) * diffuse * vec4(light.color, 1.0);
}