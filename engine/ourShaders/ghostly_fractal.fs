#version 330 core

vec2  iResolution = vec2(1920, 1080);
float iTime = 1.f;

/*** --- SAFE SHADERTOY MACRO REPLACEMENTS --- ***/
// Shadertoy-style macro rewritten to a function
vec3 P(float z) {
    return vec3(
        (cos(z * 0.3) * 0.6) * 7.0,
        (cos(z * 0.1) * 0.8) * 6.0,
        z
    );
}

#define T (iTime * 0.1)

// rot(a) from Shadertoy was invalid for GL; rewrite:
mat2 rot(float a) {
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c);
}

/*** --- LOOK FUNCTION --- ***/
vec3 look(vec3 p) {
    float t = T * 6.0;
    return p - vec3(
        P(p.z).x + tanh(cos(t * 0.3) * 2.0) * 2.8,
        P(p.z).y + tanh(cos(t * 0.5) * 2.0) * 2.8,
        1.3 + T + tanh(cos(t * 0.15) * 1.9)
    );
}

/*** --- MAIN --- ***/
in vec2 uv;
out vec4 FragColor;

void main()
{
    vec2 u = uv * iResolution;

    float s = 0.002;
    float d = 0.0, i, j, l, w, shell;

    vec3 r  = vec3(iResolution,0.);
    vec3 p  = P(T);
    vec3 ro = p;
    vec3 q;

    vec3 Z = normalize(P(T + 3.0) - look(p) - p);
    vec3 X = normalize(vec3(Z.z, 0.0, -Z.x));

    // Fix invalid mat2 * vec3 ? use vec3(dir.xy, dir.z)
    vec3 dir = vec3(rot(sin(T) * 0.6) * ((u - r.xy * 0.5) / r.y), 1.0);

    vec3 D = mat3(-X, cross(X, Z), Z) * dir;

    vec4 o = vec4(0.0);

    for (i = 0.0; i < 99.0 && s > 0.001; i += 1.0) {

        p = ro + D * d;

        shell = 0.1 - length(p.xy - P(p.z).xy - d);

        q = p;

        p += cos(6.0*T + p.y + p.x + p.z) * 0.4;

        // FIX: vec3 swizzle p.zxy does not exist in GLSL ? use vec3(p.z, p.x, p.y)
        s = dot(abs(p - floor(p) - 0.5), vec3(0.45));

        p = q;
        p.y += cos(6.0*T + p.z) * 0.5;
        p.x += cos(5.0*T + p.z) * 0.6;
        p *= 2.0;

        for (j = 0.0, w = 2.0; j < 8.0; j += 1.0) {

            p.xy *= rot(T);
            p      = sin(p);
            p.xy *= rot(T * 3.5);
            p.xz *= rot(sin(T * 3.0) * 0.2);

            l = 2.11 / dot(p, p);
            p *= l;
            w *= l;
        }

        d += max(shell, min(s, length(p) / w));

        o.rgb += (cos(vec3(p.y, p.z, p.x) + 0.1) * 0.005) / (d * s);
    }

    o = tanh(o * exp(-d / 24.0));
    FragColor = o;
}
