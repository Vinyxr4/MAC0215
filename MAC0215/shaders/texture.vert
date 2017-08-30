attribute vec3 position;
attribute vec3 texCoord;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

varying vec4 tex;

void main (void) {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    tex = vec4 (texCoord, 1.0);

    gl_Position = chain * vec4 (position, 1.0);
}
