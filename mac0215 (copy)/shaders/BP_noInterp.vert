# version 120

attribute vec3 position;
attribute vec3 vNormal;

uniform vec3 ambientProduct;
uniform vec3 diffuseProduct;
uniform vec3 specularProduct;

uniform float shininess;

uniform vec4 lightPosition;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

varying vec4 fColor;

void main() {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    vec3 pos = vec4 (chain * vec4(position, 1.0)).xyz;

    vec3 E = normalize( pos );
    vec3 N = normalize(vec4 (chain * vec4(vNormal, 1.0))).xyz;

    vec3 L = normalize( lightPosition.xyz );
    vec3 H = normalize( L + E );

    vec4 ambient = vec4(ambientProduct, 1.0);
    vec4 diffuse = vec4(diffuseProduct, 1.0) * max( dot(L, N), 0.0 );
    vec4 specular = vec4(specularProduct, 1.0) * pow( max(0.0, dot(N, H)), shininess );

    fColor = ambient + diffuse + specular;
    fColor.a = 1.0;

    gl_Position = chain * vec4(position, 1.0);
}
