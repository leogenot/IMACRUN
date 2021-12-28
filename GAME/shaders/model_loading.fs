#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

struct PointLight {
    vec3 position;
    vec3 color;
};

#define NR_POINT_LIGHTS 5

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 dirLight;
uniform vec3 lightColor;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

// function prototypes
vec3 DirLightPhong(vec3 dirLight, vec3 normal, vec3 viewDir);
vec3 PointLightPhong(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // directional lighting
    vec3 result = DirLightPhong(dirLight, norm, viewDir);

    // point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += PointLightPhong(pointLights[i], norm, FragPos, viewDir);        
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 DirLightPhong(vec3 dirLight, vec3 normal, vec3 viewDir)
{    
    vec3 lightDir = normalize(-dirLight);

    // ambient
    vec3 ambient = lightColor * vec3(texture(material.texture_diffuse1, TexCoords));

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff * vec3(texture(material.texture_diffuse1, TexCoords));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * spec * vec3(texture(material.texture_specular1, TexCoords));

    // combine results
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 PointLightPhong(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // ambient
    vec3 ambient = light.color * vec3(texture(material.texture_diffuse1, TexCoords));

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * vec3(texture(material.texture_diffuse1, TexCoords));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * spec * vec3(texture(material.texture_specular1, TexCoords));

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);    
    
    // combine results
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}