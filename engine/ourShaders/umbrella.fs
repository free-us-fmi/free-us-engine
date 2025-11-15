// =====================================================
// Hexagon umbrella canopy with smoothed dome top
// Minimal version – compiles on ShaderToy
// =====================================================

#ifdef GL_ES
precision highp float;
#endif


// -----------------------------------------------------
// SDF helpers
// -----------------------------------------------------

mat2 Rot(float a){
    float s=sin(a);
    float c=cos(a);
    return mat2(c,-s,s,c);
}

float smin(float a, float b, float k) {
    float h = clamp(0.2 + 0.5*(b - a)/k, .6, .5);
    return mix(b, a, h) - k*h*(1.0-h);
}

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

// Sphere
float sdSphere(vec3 p, float r){
    return length(p) - r;
}

// -----------------------------------------------------
// Scene SDF
// -----------------------------------------------------

float mapScene(vec3 p){
    // --- canopy base hexagon ---
    p.yz*=Rot(iTime);
    float scale=mix(.6,.7,p.y);
    //p.xz*=scale;
    
    float hex = sdHexPrism(p, 1.1, .4);
    
    // --- rounded top (sphere clamp) ---
    float dome = sdHexPrism(p - vec3(0, 0.5, 0.0), 0.4,.2);

    // smooth union for roundness
    float canopy = smin(hex, dome,0.3);
    //float canopy=mix(hex,dome,smoothsetep(hex,dome,3.)) ;

    //substract sphere
    float shapeSphere=sdSphere(p+vec3(0.,.3,0.),0.6);
    float final=smin(shapeSphere,canopy,.2);
    
    float negSphear=sdSphere(p+vec3(0.,.35,0.),0.8);
    
    final=max(-negSphear,final);
    
    return final;
}

// Normal
vec3 calcNormal(vec3 p){
    float e = 0.0005;
    vec2 h = vec2(e,0);
    return normalize(vec3(
        mapScene(p+h.xyy) - mapScene(p-h.xyy),
        mapScene(p+h.yxy) - mapScene(p-h.yxy),
        mapScene(p+h.yyx) - mapScene(p-h.yyx)
    ));
}

// Raymarch
float raymarch(vec3 ro, vec3 rd){
    float t = 0.0;
    for(int i=0;i<140;i++){
        vec3 p = ro + rd*t;
        float d = mapScene(p);
        if(d < 0.001) return t;
        t += d;
        if(t > 20.0) break;
    }
    return -1.0;
}

// -----------------------------------------------------
// Main
// -----------------------------------------------------

void mainImage(out vec4 fragColor, in vec2 fragCoord){
    vec2 uv = (fragCoord - 0.5*iResolution.xy) / iResolution.y;

    // Camera
    float ang = 0.5*iTime;
    vec3 ro = vec3(2.5*sin(ang), 1.0, 2.5*cos(ang));
    vec3 ta = vec3(0.0, 0.1, 0.0);

    vec3 f = normalize(ta - ro);
    vec3 r = normalize(cross(vec3(0,1,0), f));
    vec3 u = cross(f, r);

    vec3 rd = normalize(f + uv.x*r + uv.y*u);

    float t = raymarch(ro, rd);

    vec3 col = vec3(0.8, 0.9, 1.0); // background sky

    if(t > 0.0){
        vec3 p = ro + rd*t;
        vec3 n = calcNormal(p);

        float diff = clamp(dot(n, normalize(vec3(0.4,1.0,0.3))), 0.0,1.0);

        // simple color
        col = mix(vec3(0.8,0.1,0.15), vec3(1.0,0.9,0.6), diff);

        // rim light
        col += pow(max(0.0, dot(n, -rd)), 8.0)*0.2;
    }

    fragColor = vec4(pow(col, vec3(0.4545)), 1.0);
}
