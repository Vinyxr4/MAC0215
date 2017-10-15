#version 330
#extension GL_EXT_geometry_shader4: enable

#define sampling 4

layout(triangles) in;
layout(triangle_strip) out;
layout (max_vertices =  45) out;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

out vec4 pixel_color;

vec3 bezier_quadratic_eval (float t, vec3 control_p[3]) {
    vec3 p[3];
    p[0] = control_p[0];
    p[1] = control_p[1];
    p[2] = control_p[2];

    float one_minus = 1.0 - t;
    float one_minus_2 = one_minus * one_minus;
    float one_minus_t_2 = one_minus * t * 2.0;
    float t_2 = t * t;

    vec3 ans = one_minus_2 * p[0] + one_minus_t_2 * p[1] + t_2 * p[2];

    return ans/200;
}

void test () {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();

}

void main() {
    vec3 control_p[3];
    control_p[0] = gl_in[0].gl_Position.xyz;
    control_p[1] = gl_in[1].gl_Position.xyz;
    control_p[2] = gl_in[2].gl_Position.xyz;

    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    float delta = 1/float(sampling-1.0);

    vec3 sample = bezier_quadratic_eval (0, control_p);
    for (int i = 1; i < sampling; ++i) {
        float t = i * delta;
        gl_Position = vec4(sample.xyz,1.0);
        pixel_color = vec4 (0,0,0,1);
        EmitVertex();
        gl_Position = chain * vec4(-10, 0, 0,1.0);
        pixel_color = vec4 (0,0,0,1);
        EmitVertex();
        sample = bezier_quadratic_eval (t, control_p);
        gl_Position = vec4(sample.xyz,1.0);
        pixel_color = vec4 (0,0,0,1);
        EmitVertex();
    }
    EndPrimitive();
}

