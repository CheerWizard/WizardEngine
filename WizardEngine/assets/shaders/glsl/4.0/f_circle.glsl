#version 400

in vec4 f_color;
in flat int f_id;
in vec2 f_uv;

out vec4 fragment;

struct Circle {
    float thickness;
    float fade;
};

uniform Circle circle[128];

void main() {
    Circle c = circle[f_id];
    float distance = 1.0 - length(f_uv);
    float thickness = c.thickness;
    float fade = c.fade;
    float alpha = smoothstep(0.0, fade, distance);
    alpha *= smoothstep(thickness + fade, thickness, distance);

    if (alpha == 0.0) {
        // discard transparent side
        discard;
    }

    fragment = f_color;
    fragment.a *= alpha;
}