#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 newColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int shininess;
uniform float ambientStrength;
uniform float specularStrength;
uniform float diffuseStrength;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    vec3 VertexPos = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    
    //float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - VertexPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;
    
    vec3 viewDir = normalize(viewPos - VertexPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;      

    newColor = (ambient + diffuse + specular) * objectColor;
}