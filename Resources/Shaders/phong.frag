#version 330 core
in vec4 camSpace_pos;
in vec4 camSpace_norm;

uniform mat4 model, view, projection;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float specularIntensity;
uniform vec4 objectColor;
uniform float shininess;

uniform int lightType[8];
uniform vec4 worldSpace_lightPos[8];
uniform vec4 worldSpace_lightDir[8];
uniform vec3 lightIntensity[8];

uniform int numLights;

out vec4 fragColor;

vec3 pointLight(int index){
    vec3 camSpace_norm_n = normalize(camSpace_norm.xyz);

    vec4 camSpace_toLight = view*worldSpace_lightPos[index] - camSpace_pos;
    vec3 camSpace_toLight_n = normalize(camSpace_toLight.xyz);

    vec3 camSpace_toEye_n = -normalize(camSpace_pos.xyz);
    vec3 camSpace_reflectedLight_n = reflect(-camSpace_toLight_n, camSpace_norm_n);

    //Diffuse
    float diffuseDot = max(dot(camSpace_toLight_n, camSpace_norm_n), 0);

    //Specular
    float specularDot = max(dot(camSpace_reflectedLight_n, camSpace_toEye_n), 0);

    return objectColor.xyz*(diffuseDot*diffuseIntensity+pow(specularDot, 5));

    if(shininess <= 0.0){
        return objectColor.xyz*(diffuseDot*diffuseIntensity);
    }

    return objectColor.xyz*(diffuseDot*diffuseIntensity+pow(specularDot, shininess)*specularIntensity);
}

vec3 directionalLight(int index){
    vec3 camSpace_norm_n = normalize(camSpace_norm.xyz);

    vec4 camSpace_lightDir = view*worldSpace_lightDir[index];
    vec3 camSpace_lightDir_n = normalize(camSpace_lightDir.xyz);

    vec3 camSpace_toEye_n = -normalize(camSpace_pos.xyz);
    vec3 camSpace_reflectedLight_n = reflect(camSpace_lightDir_n, camSpace_norm_n);

    //Diffuse
    float diffuseDot = max(dot(-camSpace_lightDir_n.xyz, camSpace_norm_n), 0);

    //Specular
    float specularDot = max(dot(camSpace_reflectedLight_n, camSpace_toEye_n), 0);

    if(shininess <= 0.0){
        return objectColor.xyz*(diffuseDot*diffuseIntensity);
    }

    return objectColor.xyz*((diffuseDot*diffuseIntensity)+(pow(specularDot, shininess)*specularIntensity));
}

vec3 attenuate(vec3 initialLightColor, int index){
    //No attenuation currently
    return initialLightColor*lightIntensity[index].x;
}

void main() {
    vec3 tempColor = objectColor.xyz*ambientIntensity;

   //Diffuse and Specular Terms

   for(int i = 0; i<numLights; i+= 1){
       if(lightType[i] == 0){
           //Directional Light
           tempColor += lightIntensity[i].x*directionalLight(i);
       }
       if(lightType[i] == 1){
           //Point Light
           vec3 pointContribution = pointLight(i);
           //Add attenuation
           tempColor += attenuate(pointContribution, i);
       }

       fragColor = vec4(tempColor, 1.0);
   }

}
