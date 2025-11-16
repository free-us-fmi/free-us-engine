#version 330 core

vec2 iResolution=vec2(1900.,1800.);
uniform float iTime;

out vec4 fragColor;

//HELPER
vec3 palette(float h){
    return 0.5+0.5*sin(6.2831*(h+vec3(0.0,0.33,0.66)));
}

float hash(float n){ return fract(sin(n)*43758.5453); }

float smin(float a, float b, float k) {
    float h = clamp(0.2 + 0.5*(b - a)/k, .6, .5);
    return mix(b, a, h) - k*h*(1.0-h);
}

mat2 Rot(float a){
    float s=sin(a);
    float c=cos(a);
    return mat2(c,-s,s,c);
}

//FORMS
float sdSphere(vec3 p, float r){ return length(p)-r; }
float sdPlane(vec3 p){ return p.y; }

// 2D hexagon SDF
float sdHexagon(vec2 p, float r) {
    p = abs(p);
    return max(dot(p, vec2(0.5, 0.8660254)), p.x) - r;
}

// Hexagonal prism
float sdHexPrism(vec3 p, float r, float h){
    float d = sdHexagon(p.xz, r);
    float dy = abs(p.y) - h;
    return max(d, dy);
}

// -----------------------------------------------------
// Scene SDF
// -----------------------------------------------------

float sdUmbrella(vec3 p){
    float scale=mix(.6,.7,p.y);

    float hex = sdHexPrism(p, 1.1, .4);

    float dome = sdHexPrism(p - vec3(0, 0.5, 0.0), 0.4,.2);

    float canopy = smin(hex, dome,0.3);
    //float canopy=mix(hex,dome,smoothstep(hex,dome,3.)) ;

    float shapeSphere=sdSphere(p+vec3(0.,.3,0.),0.6);
    float final=smin(shapeSphere,canopy,.2);

    float negSphear=sdSphere(p+vec3(0.,.35,0.),0.8);

    final=max(-negSphear,final);

    return final;
}

// Scene distance function
//------------------------------------------------------

const int UMB_W = 4;
const int UMB_H = 4;
const float UMB_SPACING = 1.7;

float umbh(int i) {
    return abs(sin(iTime*float(i)/4.) * 2.123);
}

float map(vec3 p, out int matID)
{
    float d = 1e9;
    matID = 0;

    float g = p.y + 1.0;
    if (g < d) { d = g; matID = 1; }

    for (int y=0; y<UMB_H; y++)
    for (int x=0; x<UMB_W; x++)
    {
        int idx = x + y * UMB_W;

        vec3 pos = vec3(
            float(x - UMB_W/2) * UMB_SPACING,
            1.3 + 0.2 * umbh(idx),
            float(y - UMB_H/2) * UMB_SPACING
        );

        float s = sdUmbrella(p - pos);

        if (s < d) { d = s; matID = 2 + idx; }
    }

    return d;
}

vec3 getNormal(vec3 p){
    float e=0.001;
    int m;

    // vec2 k = vec2(1,-1);  // INVALID swizzling in your original ? fixed
    // replace with actual vec3 offsets:
    vec3 o1 = vec3( 1, 1,  -1);
    vec3 o2 = vec3(-1, -1,  1);
    vec3 o3 = vec3(-1,  1, -1);
    vec3 o4 = vec3( 1, -1, -1);

    return normalize(
        o1 * map(p + o1*e, m) +
        o2 * map(p + o2*e, m) +
        o3 * map(p + o3*e, m) +
        o4 * map(p + o4*e, m)
    );
}

vec3 shade(vec3 ro, vec3 rd)
{
    float t=0.0;
    int matID=0;
    vec3 col=vec3(0.0);

    for (int i=0;i<120;i++){
        vec3 p=ro+rd*t;
        float d=map(p,matID);
        if (d<0.001 || t>20.0) break;
        t+=d*0.8;
    }

    vec3 p=ro+rd*t;
    if (t>20.0) return vec3(0.02,0.03,0.06);

    vec3 n=getNormal(p);
    vec3 lightDir=normalize(vec3(0.4,1.0,0.3));
    float diff=max(dot(n,lightDir)*2.,.0);
    float spec=pow(max(dot(reflect(-lightDir,n),-rd),0.0),32.0)*2.2;

    if (matID==1){
        float refl = pow(max(0.0, n.y),0.5);
        col = mix(vec3(0.5,0.5,0.6), vec3(0.1,0.12,0.15), refl);
        col += 0.7*spec;
    }
    else {
        float h = fract(float(matID)*0.17);
        vec3 base = palette(h);
        float pulse = 0.5 + 0.5*sin(iTime*2.0+h*6.28);
        col = base*(0.4+0.6*diff*pulse) + 0.2*spec;
    }

    return col;
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = (fragCoord - 0.5*iResolution.xy)/iResolution.y;

    vec3 ro = vec3(-.2, 2., 9.0);
    vec3 rd = normalize(vec3(uv, -1.2));
    float angle=-0.01;
    rd.yz = mat2(cos(angle), -sin(angle), sin(angle), cos(angle)) * rd.yz;

    vec3 col = shade(ro, rd);

    fragColor = vec4(col,1.0);
}
