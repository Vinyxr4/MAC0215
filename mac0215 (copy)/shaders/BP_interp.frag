# version 120

varying vec3 N, E, L;

void main()
{
    vec4 col;

    vec3 N2 = normalize (N);
    vec3 E2 = normalize (E);
    vec3 H = normalize( L + E2 );

    vec4 ambient = vec4(vec3(0.2), 1.0);
    vec4 diffuse = vec4(vec3(0.3), 1.0) * max( dot(L, N2), 0.0 );
    vec4 specular = vec4(vec3(0.3), 1.0) * pow( max(dot(N2, H), 0.0), 5 );

    col = ambient + diffuse +specular;
    col.a = 1.0;

    gl_FragColor = col;
}

