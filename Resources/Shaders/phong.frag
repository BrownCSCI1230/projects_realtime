#version 330 core
in vec4 camSpace_pos;
in vec4 camSpace_norm;

in vec2 uv;

uniform sampler2D objTexture;
uniform bool texUsed;

uniform mat4 model, view, projection;
uniform float ka;
uniform float kd;
uniform float ks;
uniform vec4 obj_ambient_color;
uniform vec4 obj_diffuse_color;
uniform vec4 obj_specular_color;
uniform float shininess;

uniform int lightType[8];
uniform vec4 lightColor[8];
uniform vec3 lightFunction[8];
uniform vec4 camSpace_lightPos[8];
uniform vec4 camSpace_lightDir[8];

uniform int numLights;

out vec4 fragColor;

vec4 getToLight(int lightIndex) {
    int LIGHT_POINT = 0;
    int LIGHT_DIRECTIONAL = 1;
    int LIGHT_SPOT = 2;
    int LIGHT_AREA = 3;

    if (lightType[lightIndex] == LIGHT_POINT) {
        return normalize((camSpace_lightPos[lightIndex]) - camSpace_pos);
    }
    else if (lightType[lightIndex] == LIGHT_DIRECTIONAL) {
        return normalize(-camSpace_lightDir[lightIndex]);
    }
    else if (lightType[lightIndex] == LIGHT_SPOT) {
        return normalize(camSpace_lightPos[lightIndex] - camSpace_pos);
    }
    else if (lightType[lightIndex] == LIGHT_AREA) {
        return normalize(camSpace_lightPos[lightIndex] - camSpace_pos);
    }

    return vec4(0);
}

float attenuationFactor(int lightIndex) {
    int LIGHT_POINT = 0;
    int LIGHT_DIRECTIONAL = 1;
    int LIGHT_SPOT = 2;
    int LIGHT_AREA = 3;

    if (lightType[lightIndex] == LIGHT_POINT) {
        vec3 coeffs = lightFunction[lightIndex];
        float d = length(camSpace_lightPos[lightIndex] - camSpace_pos);
        return 1.0 / (coeffs.x + coeffs.y * d + coeffs.z * d * d);
    }
    else if (lightType[lightIndex] == LIGHT_SPOT) {
        return 1;
    }
    else if (lightType[lightIndex] == LIGHT_AREA) {
        return 1;
    }

    return 1;
}

float computeDiffuseIntensity(vec3 camSpace_toLight) {
    //simple dot product to get diffuse intensity
    return max(dot(camSpace_toLight,
                   normalize(vec3(camSpace_norm))),
              0);
}

float computeSpecularIntensity(vec3 camSpace_toLight, vec3 camSpace_toEye) {
    // guard against pow weirdness when exponent is 0
    if (shininess < 0.0001) {
        return 0;
    }

    //reflect toLight
    vec3 camSpace_toLightReflected = reflect(-camSpace_toLight, normalize(vec3(camSpace_norm)));

    //Compute specular intensity using toEye, reflected light, and shininess
    return pow(max(dot(camSpace_toLightReflected,
                       camSpace_toEye),
                   0),
               shininess);
}

void main() {
    // Declare ambient, diffuse, and specular terms
    vec3 ambi = ka * vec3(obj_ambient_color);
    vec3 diff = vec3(0.0);
    vec3 spec = vec3(0.0);


    // Compute toEye Vector for specular intensity computation;
    vec4 camSpace_toEye = normalize(vec4(0,0,0,1) - camSpace_pos);


    // Compute per-light diffuse and specular contribution
    for(int i = 0; i<numLights; i+= 1){

        // get direction vector to light based on light type
        vec4 camSpace_toLight = getToLight(i);

        float diffuse_intensity = computeDiffuseIntensity(vec3(camSpace_toLight));
        float specular_intensity = computeSpecularIntensity(vec3(camSpace_toLight), vec3(camSpace_toEye));

        float att = attenuationFactor(i);


        diff = diff + diffuse_intensity * vec3(lightColor[i]) * att;
        spec = spec + specular_intensity * vec3(lightColor[i]) * att;
    }

    diff = diff * vec3(kd) * vec3(obj_diffuse_color);
    spec = spec * vec3(ks) * vec3(obj_specular_color);



    vec3 tempColor = clamp(ambi + diff + spec,0,1);

    if (texUsed) {
        vec3 texColor = vec3(texture(objTexture, uv));
        fragColor = vec4(tempColor * texColor, 1.0);
    } else {
        fragColor = vec4(tempColor, 1.0);
    }
}
