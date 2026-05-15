#version 330 core

// === UNIFORMS YOU MUST PROVIDE FROM YOUR APPLICATION ===
vec2  uResolution = vec2(1920, 1080);   // window size
uniform float iTime;         // time in seconds
vec4  uMouse = vec4(0.f, 0.f, 0.f, 0.f);        // x,y = pos, z = pressed?
uniform sampler2D uChannel0; // replaces iChannel0
uniform sampler2D uChannel1; // replaces iChannel1

out vec4 FragColor;


// =========================================================
// === BEGIN: CODE PORTED FROM SHADERTOY ===================
// =========================================================

#define RAY_STEPS 150
#define BRIGHTNESS 1.2
#define GAMMA 1.4
#define SATURATION .65
#define detail .001

#define WAVES
#define BORDER

float t;
float edge = 0.0;
float det  = 0.0;

const vec3 origin = vec3(-1., .7, 0.);

// -------------------------
mat2 rot(float a)
{
    float c = cos(a), s = sin(a);
    return mat2(c, s, -s, c);
}

// -------------------------
vec4 formula(vec4 p)
{
    p.xz = abs(p.xz + 1.) - abs(p.xz - 1.) - p.xz;
    p.y -= .25;
    p.xy *= rot(radians(35.));
    p = p * 2.0 / clamp(dot(p.xyz, p.xyz), .2, 1.);
    return p;
}

// -------------------------
float de(vec3 pos)
{
#ifdef WAVES
    pos.y += sin(pos.z - t * 6.) * .15;
#endif

    vec3 tpos = pos;
    tpos.z = abs(3. - mod(tpos.z, 6.));
    vec4 p = vec4(tpos, 1.0);

    for (int i = 0; i < 4; i++)
        p = formula(p);

    float fr = (length(max(vec2(0.), p.yz - 1.5)) - 1.) / p.w;

    float ro = max(abs(pos.x + 1.) - .3, pos.y - .35);
    ro = max(ro, -max(abs(pos.x + 1.) - .1, pos.y - .5));

    pos.z = abs(.25 - mod(pos.z, .5));
    ro = max(ro, -max(abs(pos.z) - .2, pos.y - .3));
    ro = max(ro, -max(abs(pos.z) - .01, -pos.y + .32));

    return min(fr, ro);
}

// -------------------------
vec3 path(float ti)
{
    ti *= 1.5;
    return vec3(
        sin(ti),
        (1. - sin(ti * 2.)) * .5,
        -ti * 5.
    ) * .5;
}

// -------------------------
vec3 normal(vec3 p)
{
    vec3 e = vec3(0.0, det * 5., 0.0);

    float d1 = de(p - e.yxx), d2 = de(p + e.yxx);
    float d3 = de(p - e.xyx), d4 = de(p + e.xyx);
    float d5 = de(p - e.xxy), d6 = de(p + e.xxy);
    float d = de(p);

    edge =
        abs(d - 0.5 * (d2 + d1)) +
        abs(d - 0.5 * (d4 + d3)) +
        abs(d - 0.5 * (d6 + d5));

    edge = min(1., pow(edge, .55) * 15.);

    return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

// -------------------------
vec4 rainbow(vec2 p)
{
    float s = sin(p.x * 7.0 + t * 70.0) * 0.08;
    p.y += s;
    p.y *= 1.1;

    vec4 c;
    if (p.x > 0.0) c = vec4(0,0,0,0);
    else if (0./6.<p.y && p.y<1./6.) c=vec4(255,43,14,255)/255.;
    else if (1./6.<p.y && p.y<2./6.) c=vec4(255,168,6,255)/255.;
    else if (2./6.<p.y && p.y<3./6.) c=vec4(255,244,0,255)/255.;
    else if (3./6.<p.y && p.y<4./6.) c=vec4(51,234,5,255)/255.;
    else if (4./6.<p.y && p.y<5./6.) c=vec4(8,163,255,255)/255.;
    else if (5./6.<p.y && p.y<6./6.) c=vec4(122,85,255,255)/255.;
    else c = vec4(0,0,0,0);

    c.a *= .8 - min(.8, abs(p.x * .08));
    c.rgb = mix(c.rgb, vec3(length(c.rgb)), .15);
    return c;
}

// -------------------------
vec4 nyan(vec2 p)
{
    vec2 uv = p * vec2(0.4, 1.0);

    float ns = 3.0;
    float nt = iTime * ns;
    nt -= mod(nt, 240.0/256.0/6.0);
    nt = mod(nt, 240.0/256.0);

    float ny = mod(iTime * ns, 1.0);
    ny -= mod(ny, 0.75);
    ny *= -0.05;

    vec2 tc = vec2(uv.x/3.0 + 210.0/256.0 - nt + 0.05, .5 - uv.y - ny);
    vec4 color = texture(uChannel1, tc);

    if (uv.x < -0.3) color.a = 0.;
    if (uv.x > 0.2)  color.a = 0.;

    return color;
}

// -------------------------
vec3 raymarch(vec3 from, vec3 dir)
{
    edge = 0.;
    float d = 100.;
    float totdist = 0.;
    vec3 p;

    for (int i = 0; i < RAY_STEPS; i++)
    {
        if (d > det && totdist < 25.)
        {
            p = from + totdist * dir;
            d = de(p);
            det = detail * exp(.13 * totdist);
            totdist += d;
        }
    }

    p -= (det - d) * dir;
    vec3 norm = normal(p);

    vec3 col = (1. - abs(norm)) * max(0., 1. - edge * .8);
    totdist = clamp(totdist, 0., 26.);

    // Background sky & sun
    float sunsize = 7. - max(0., texture(uChannel0, vec2(.6, .2)).x) * 5.;
    float an = atan(dir.x, dir.y) + iTime * 1.5;

    float s  = pow(clamp(1. - length(dir.xy) * sunsize
             - abs(.2 - mod(an, .4)), 0., 1.), .1);

    float sb = pow(clamp(1. - length(dir.xy) * (sunsize - .2)
             - abs(.2 - mod(an, .4)), 0., 1.), .1);

    float sg = pow(clamp(1. - length(dir.xy) * (sunsize - 4.5)
             - .5 * abs(.2 - mod(an, .4)), 0., 1.), 3.);

    float y = mix(.45, 1.2, pow(smoothstep(0., 1., .75 - dir.y), 2.))
            * (1. - sb * .5);

    vec3 backg = vec3(0.5,0.,1.) * ((1.-s)*(1.-sg)*y
               + (1.-sb)*sg*vec3(1.,.8,0.15)*3.);

    backg += vec3(1.,.9,.1)*s;
    backg = max(backg, sg*vec3(1.,.9,.5));

    col = mix(vec3(1.,.9,.3), col, exp(-.004 * totdist * totdist));

    if (totdist > 25.) col = backg;

    col = pow(col, vec3(GAMMA)) * BRIGHTNESS;
    col = mix(vec3(length(col)), col, SATURATION);
    col *= vec3(1., .9, .85);

#ifdef NYAN
    dir.yx *= rot(dir.x);
    vec2 ncatpos = (dir.xy + vec2(-3. + mod(-t, 6.), -.27));
    vec4 ncat = nyan(ncatpos * 5.);
    vec4 rain = rainbow(ncatpos * 10. + vec2(.8, .5));

    if (totdist > 8.) col = mix(col, max(vec3(.2), rain.rgb), rain.a * .9);
    if (totdist > 8.) col = mix(col, max(vec3(.2), ncat.rgb), ncat.a * .9);
#endif

    return col;
}

// -------------------------
vec3 moveCamera(inout vec3 dir)
{
    vec3 go = path(t);
    vec3 adv = path(t + .7);
    vec3 advec = normalize(adv - go);

    float an = adv.x - go.x;
    an *= min(1., abs(adv.z - go.z)) * sign(adv.z - go.z) * .7;
    dir.xy *= rot(an);

    an = advec.y * 1.7;
    dir.yz *= rot(an);

    an = atan(advec.x, advec.z);
    dir.xz *= rot(an);

    return go;
}

// -------------------------
void main()
{
    t = iTime * .5;

    vec2 uv = gl_FragCoord.xy / uResolution * 2. - 1.;
    vec2 oriuv = uv;

    uv.y *= uResolution.y / uResolution.x;

    vec2 mouse = (uMouse.xy / uResolution - .5) * 3.;
    if (uMouse.z < 1.) mouse = vec2(0., -0.05);

    float fov = .9 - max(0., .7 - iTime * .3);

    vec3 dir = normalize(vec3(uv * fov, 1.));
    dir.yz *= rot(mouse.y);
    dir.xz *= rot(mouse.x);

    vec3 from = origin + moveCamera(dir);

    vec3 color = raymarch(from, dir);

#ifdef BORDER
    color = mix(vec3(0.), color,
        pow(max(0., .95 - length(oriuv * oriuv * oriuv * vec2(1.05, 1.1))), .3));
#endif

    FragColor = vec4(color, 1.0);
}
