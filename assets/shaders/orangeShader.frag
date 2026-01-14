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
uniform vec3 viewPos;

uniform mat4 view;

void main() {
  vec3 normalVector = normalize(Normal);
  vec3 lightDirection = normalize(lightPos - fragPos);

  float diffTheta = max(dot(normalVector, lightDirection), 0.0f);
  vec3 diffuse = diffTheta * lightColor;

  vec3 viewDirection = normalize(viewPos - fragPos);
  vec3 reflectionDirection = reflect(-lightDirection, normalVector);
  float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);

  float ambientMagnitude = 0.1;
  vec3 ambient = ambientMagnitude * lightColor;

  float specularMagnitude = 0.5;
  vec3 specular = spec * specularMagnitude * lightColor;

  vec3 finalColor = (ambient + diffuse + specular) * objectColor;

  FragColor = vec4(finalColor, 1.0f);
}
