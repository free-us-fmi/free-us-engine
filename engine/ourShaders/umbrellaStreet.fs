#version 450 core

out vec4 FragColor;

//uniform float uTime;
//uniform vec2 uResolution;

float uTime =1.f;
vec2 uResolution=vec2(1920.f,1080.f);
// ---------------------------------------------------------
// Helper
// ---------------------------------------------------------
vec3 palette(float h){
    return 0.5 + 0.5 * sin(6.2831 * (h + vec3(0.0, 0.33, 0.66)));
}

float hash(float n){ return fract(sin(n)*43758.5453); }

float smin(float a, float b, float k) {
    float h = clamp(0.2 + 0.5*(b - a)/k, 0.6, 0.5);
    return mix(b, a, h) - k*h*(1.0-h);
}

mat2 Rot(float a){
    float s=sin(a), c=cos(a);
    return mat2(c,-s,s,c);
}

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

// Box
float sdBox(vec3 p, vec3 b){
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

// ---------------------------------------------------------
// Umbrella SDF
// ---------------------------------------------------------
float sdUmbrella(vec3 p){
    float hex = sdHexPrism(p, 1.1, .4);
    float dome = sdHexPrism(p - vec3(0, 0.5, 0.0), 0.4,.2);
    float canopy = smin(hex, dome, 0.3);

    float shapeSphere = sdSphere(p+vec3(0.,.3,0.),0.6);
    float final = smin(shapeSphere,canopy,.2);

    float negSphere = sdSphere(p+vec3(0.,.35,0.),0.8);
    final = max(-negSphere, final);

    return final;
}

// ---------------------------------------------------------
// Scene Mapping
// ---------------------------------------------------------
const int UMB_W = 4;
const int UMB_H = 4;
const float UMB_SPACING = 2.5;

float umbh(int i) {
    return abs(sin(uTime * float(i) / 10.) * 2.123);
}

float mapScene(vec3 p, out int matID)
{
    float d = 1e9;
    matID = 0;

    float g = p.y + 1.0;
    if (g < d) { d = g; matID = 1; }

    float leftbuilding  = sdBox(p+vec3(3.5,0.,1.),vec3(.5,3.,10.));
    float rightbuilding = sdBox(p+vec3(-3.5,0.,1.),vec3(.5,3.,10.));

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

    d = min(d, rightbuilding);
    return min(d, leftbuilding);
}

vec3 getNormal(vec3 p){
    float e=0.001;
    int m;
    vec2 k=vec2(1,-1);
    return normalize(
        k.xyy*mapScene(p+k.xyy*e,m) +
        k.yyx*mapScene(p+k.yyx*e,m) +
        k.yxy*mapScene(p+k.yxy*e,m) +
        k.xxx*mapScene(p+k.xxx*e,m)
    );
}

// ---------------------------------------------------------
// Shading
// ---------------------------------------------------------
vec3 shade(vec3 ro, vec3 rd)
{
    float t=0.0;
    int matID=0;
    for (int i=0;i<120;i++){
        vec3 p=ro+rd*t;
        float d=mapScene(p,matID);
        if (d<0.001 || t>20.0) break;
        t+=d*0.8;
    }

    if (t>20.0) return vec3(0.80, 0.91, 0.95);

    vec3 p=ro+rd*t;
    vec3 n=getNormal(p);
    vec3 lightDir=normalize(vec3(0.4,1.0,0.3));

    float diff=max(dot(n,lightDir)*2.,.0);
    float spec=pow(max(dot(reflect(-lightDir,n),-rd),0.0),32.0)*2.2;

    vec3 col;
    if (matID==1){
        col = mix(vec3(0.5,0.5,0.6), vec3(0.1,0.12,0.15), .2);
        col += 0.7*spec;
    }
    else {
        float h = fract(float(matID)*0.17);
        vec3 base = palette(h);
        float pulse = 0.5 + 0.5*sin(uTime*2.0+h*6.28);
        col = base*(0.4+0.6*diff*pulse) + 0.2*spec;
    }
    return col;
}

// ---------------------------------------------------------
// Main
// ---------------------------------------------------------
void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5 * uResolution.xy) / uResolution.y;

    vec3 ro = vec3(.0, .7, 8.0);
    vec3 rd = normalize(vec3(uv, -1.3));

    float angle=-0.01;
    rd.yz = Rot(angle) * rd.yz;

    vec3 col = shade(ro, rd);
    FragColor = vec4(col,1.0);
}
