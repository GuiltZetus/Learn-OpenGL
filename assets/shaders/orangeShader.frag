#version 330 core

in vec3 ourPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 fragPos;
in vec3 fragPos_viewSpace;

in vec3 Normal_viewSpace;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float MixRate = 0.2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightPos_viewSpace;
uniform vec3 viewPos;

uniform mat4 view;

void main() {
  vec3 normalVector_viewSpace = normalize(Normal_viewSpace);
  vec3 normalVector = normalize(Normal);
  vec3 lightDirection = normalize(lightPos - fragPos);

  float diffTheta = max(dot(normalVector, lightDirection), 0.0f);
  vec3 diffuse = diffTheta * lightColor;

  vec3 lightPos_viewSpace = vec3(view * vec4(lightPos, 1.0f));
  vec3 lightDirection_viewSpace = normalize(lightPos - fragPos_viewSpace);
  vec3 viewPos_viewSpace = vec3(0.0, 0.0, 0.0);
  vec3 viewDirection_viewSpace = normalize(viewPos_viewSpace - fragPos_viewSpace);
  vec3 reflectionDirection_viewSpace = reflect(-lightDirection_viewSpace, normalVector_viewSpace);
  float spec_viewSpace = pow(max(dot(viewDirection_viewSpace, reflectionDirection_viewSpace), 0.0), 32);

  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectionDirection = reflect(-lightDirection, normalVector);
  float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);

  float ambientMagnitude = 0.1;
  vec3 ambient = ambientMagnitude * lightColor;

  float specularMagnitude = 0.5;
  vec3 specular = spec * specularMagnitude * lightColor;
  vec3 specular_viewSpace = spec_viewSpace * specularMagnitude * lightColor;

  vec3 finalColor = (ambient + specular) * objectColor;

  FragColor = vec4(finalColor, 1.0f);
}
