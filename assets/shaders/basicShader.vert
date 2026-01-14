#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 ourColor;
out vec3 ourPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 fragPos;
out vec3 fragPos_viewSpace;

out vec3 Normal_viewSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  Normal = mat3(transpose(inverse(model))) * aNormal;
  Normal_viewSpace = mat3(transpose(inverse(view * model))) * aNormal;
  fragPos = vec3(model * vec4(aPos, 1.0f));
  fragPos_viewSpace = vec3(view * vec4(fragPos, 1.0f));
}
