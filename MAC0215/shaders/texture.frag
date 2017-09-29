
//precision highp float;

uniform sampler2D texture;
varying vec2 tex;
uniform float gamma;

void main(void) {
    vec4 t = texture2D(texture, tex);

    vec4 color = vec4 (0, 0.0, 0, 1);

    //float gamma = 0.05;

    float alpha = smoothstep (0.5 - gamma, 0.5 + gamma, t.r);

    gl_FragColor = vec4(1, 1, 1 , alpha) * color;
    //gl_FragColor = vec4(1, 1, 1 , t.r) * color;
}
