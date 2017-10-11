#version 150

//precision highp float;

uniform sampler2D texture;
varying vec2 tex;
uniform float gamma;

in vec4 pixel_color;
out vec4 fcolor;

void main(void) {
    vec4 t = texture2D(texture, tex);

    vec4 color = vec4 (0, 0.0, 0, 1);

    float alpha = smoothstep (0.5 - gamma, 0.5 + gamma, t.r);

    fcolor = vec4(1, 1, 1 , alpha) * color;
}
