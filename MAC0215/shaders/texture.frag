uniform sampler2D texture;
varying vec4 tex;

void main(void) {
    vec4 t = texture2D(texture, tex.st);

    vec4 color = vec4 (0, 0, 0, 1);

    float alpha = smoothstep (0.5, 0.5, t.r);

    gl_FragColor = vec4(1, 1, 1 , alpha) * color;
    //gl_FragColor = vec4(1, 1, 1 , t.r) * color;
}
