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
layout(location = 2) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

void main() {

    vec3 lightDirWorld;
	float intensity;
	if (light.pos.w < 0.5) {
		lightDirWorld = light.pos.xyz;
		intensity = 1.0f;
	} else {
		lightDirWorld = fragPosition - light.pos.xyz;

		float dist2 = dot(lightDirWorld, lightDirWorld);
		float r2 = light.range * light.range;
		float ratio = dist2 / r2;
		ratio = clamp(ratio, 0, 1);
		intensity = 1.0 - ratio;
		intensity = intensity * intensity;
	}

	lightDirWorld = normalize(lightDirWorld);

	float dp = clamp(dot(normalize(fragNormal), -lightDirWorld), 0, 1);
    float diffuse = dp * intensity * light.intensity;

    outColor = texture(texSampler, fragTexCoord) * diffuse * vec4(light.color, 1.0);
}