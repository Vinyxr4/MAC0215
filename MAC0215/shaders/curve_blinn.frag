#version 150

in vec2 tex;
uniform float gamma;
uniform int pass;

out vec4 fcolor;

void main(void) {
    vec4 color = vec4 (0, 0.0, 0, 1);

    float test = tex.s*tex.s - tex.t;

    if (pass == 2 && test > 0)
        discard;

    //float alpha = smoothstep (0.5 - gamma, 0.5 + gamma, t.r);

    //gl_FragColor = vec4(1, 1, 1 , alpha) * color;
    fcolor = color;
}
