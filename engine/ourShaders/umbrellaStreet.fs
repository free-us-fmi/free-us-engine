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


//----------------------------------------------------------
//Baston SDF
//----------------------------------------------------------
float sdCappedCylinder(vec3 p, float h, float r) 
{
    vec2 d = abs(vec2(length(p.xz), p.y)) - vec2(r, h);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdCappedTorus(vec3 p, vec2 sc, float ra, float rb)
{
    p.x = abs(p.x);
    float k = (sc.y*p.x>sc.x*p.y) ? dot(p.xy,sc) : length(p.xy);
    return sqrt(dot(p,p) + ra*ra - 2.0*ra*k) - rb;
}

float sdBaston(vec3 p)
{

    float s=0.3;
    vec3 q=p/s;

     // --- Flip upside down (180° around X axis) ---
    q.yz = mat2(-1.0, 0.0,0.0, -1.0) * q.yz;
               

    float torusHeight = 0.5;
    float torusRadius = 0.4;
    float torusThickness = 0.13;

    // ---- Torus ----
    vec3 pt = q - vec3(0.15, torusHeight+1.42, 0.0);
    float head = sdCappedTorus(pt, vec2(0.955, 0.3), torusRadius, torusThickness);

    // ---- Cylinder attached to side of torus ----
    float attachX = torusRadius + torusThickness; // position at rim
    vec3 ph = q - vec3(attachX, torusHeight, 0.0);
    float handle = sdCappedCylinder(ph, 1.4, 0.15);

    // ---- Smooth Union ----
    float k = 0.20;
    float h = clamp(0.5 + 0.5 * (handle - head) / k, 0.0, 1.0);
    
     float r=mix(handle, head, h) - k * h * (1.0 - h);

    return r*s;
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
    //
    //sus jos
    //last param -> fata spate
    vec3 pMoved = p - vec3(-0.17, 0.0, 0.05);
    float baston = sdBaston(pMoved);
    //float baston=sdBaston(p);

    return min(final,baston);
}

// ---------------------------------------------------------
// Scene Mapping
// ---------------------------------------------------------
const int UMB_W = 4;
const int UMB_H = 4;
const float UMB_SPACING = 1.5;

float umbh(int i) {
    return abs(sin(uTime * float(i) / 10.) * 2.123);
}

mat2 rotY(float a) {
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c);
}

//-------------------------back plan--------------------------------
float sdPlan(vec3 p) {
    vec3 size = vec3(10.0, 2.0, 0.05); // width (X), height (Y), thickness (Z)
    return sdBox(p - vec3(-0.5, 1.0, -11.0), size); // move it behind umbrellas
}
//------------------------------------------------------------------
//--------------------------------TEXTURE-----------------------------


//----------------------------------------

// ----------------------------------------
//MAP
//----------------------------------------

float mapScene(vec3 p, out int matID)
{
    float d = 1e9;
    matID = 0;

    // Ground / street
    float g = p.y + 1.0;
    if (g < d) { d = g; matID = 1; }

    // Right building
    vec3 pRight = p - vec3(-5.0, 0., 1.0); 
    pRight.xz = rotY(0.0) * pRight.xz;    
    float rightbuilding = sdBox(pRight, vec3(.5,3.,20.));
    if(rightbuilding < d) { d = rightbuilding; matID = 2; }

    // Left building
    vec3 pLeft = p - vec3(3.0, 0., 1.0); 
    pLeft.xz = rotY(-0.0) * pLeft.xz;   
    float leftbuilding = sdBox(pLeft, vec3(.5,3.,20.));
    if(leftbuilding < d) { d = leftbuilding; matID = 3; }

    // Back plan
    float plan = sdPlan(p); 
    if(plan < d) { d = plan; matID = 4; }

    // Umbrellas
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
        if (s < d) { d = s; matID = 5 + idx; }
    }

    return d;
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

float noise(vec3 p){
    return fract(sin(dot(p, vec3(12.9898,78.233,37.719)))*43758.5453);
}
//---------------------------------------------------------
//TEXTURE FOR BUILDING
//-------------------------------------------------------
vec3 buildingTexture(vec3 p)
{
    // Brick parameters
    float brickWidth = 0.6;
    float brickHeight = 0.3;
    float mortar = 0.03;
    vec3 brickColor = vec3(0.55,0.2,0.15);
    vec3 mortarColor = vec3(0.6,0.6,0.6);

    // Determine row
    float row = floor(p.y / brickHeight);

    // Stagger offset every other row
    float stagger = mod(row, 2.0) * 0.5 * brickWidth;

    // Global position with stagger
    float xGlobal = p.x + stagger;
    float yGlobal = p.y;

    // Compute distance to nearest vertical edge
    float xEdgeDist = mod(xGlobal, brickWidth);
    float yEdgeDist = mod(yGlobal, brickHeight);

    // Mask: 1 if inside brick, 0 if in mortar
    float maskX = step(mortar, xEdgeDist) * step(xEdgeDist, brickWidth - mortar);
    float maskY = step(mortar, yEdgeDist) * step(yEdgeDist, brickHeight - mortar);
    float mask = maskX * maskY;

    // Add slight color variation per brick
    float n = fract(sin(dot(floor(vec2(xGlobal/brickWidth, yGlobal/brickHeight)), vec2(12.9898,78.233))) * 43758.5453);
    vec3 variedBrick = brickColor * (0.8 + 0.2 * n);

    return mix(mortarColor, variedBrick, mask);
}






//---------------------------------
// SHADOW
//--------------------------
float softShadow(vec3 ro, vec3 rd, float mint, float maxt, float k)
{
    float res = 1.0;
    float t = mint;
    int tmpMatID; // <-- declare a variable
    for(int i=0; i<64; i++)
    {
        float h = mapScene(ro + rd*t, tmpMatID); // pass variable
        if(h < 0.001) return 0.0; // fully shadowed
        res = min(res, k*h/t);     // soft shadow factor
        t += clamp(h, 0.01, 0.2);
        if(t>maxt) break;
    }
    return clamp(res, 0.0, 1.0);
}

//------------------------------------
//pastel
//-----------------------------
vec3 pastel(vec3 c) {
    // Mix with white to lighten and soften the color
    return mix(c, vec3(1.0), 0.5); // 0.5 = amount of "pastelness"
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

    if (t>20.0) return vec3(0.7, 0.85, 1.0);

    vec3 p=ro+rd*t;
    vec3 n=getNormal(p);
    vec3 lightDir=normalize(vec3(0.4,1.0,0.3));
    float sh = softShadow(p + n*0.001, lightDir, 0.01, 10.0, 32.0);

    float diff = max(dot(n, lightDir)*2.0, 0.0) * sh;
    float spec=pow(max(dot(reflect(-lightDir,n),-rd),0.0),32.0)*2.2;

    vec3 col;

    if(matID==1) // ground / street
    {
        float groundMask = smoothstep(0.9, 1.0, n.y);
        vec3 streetColor = vec3(0.76, 0.70, 0.50);
        float grain = noise(p*5.0)*0.05;
        col = streetColor + grain;
    }
    else if(matID==2) // right building
    {
        vec3 pRightLocal = p - vec3(-5.0, 0., 1.0);
        pRightLocal.xz = rotY(0.0) * pRightLocal.xz;
        col = buildingTexture(pRightLocal);
        col *= diff + 0.2;
    }
    else if(matID==3) // left building
    {
        vec3 pLeftLocal = p - vec3(3.0, 0., 1.0);
        pLeftLocal.xz = rotY(-0.0) * pLeftLocal.xz;
        col = buildingTexture(pLeftLocal);
        col *= diff + 0.2;
    }

    else if(matID==4) // back plan
    {
        col = vec3(0.93, 0.85, 0.70);
        col *= diff + 0.2;
    }
    else if(matID>=5) // umbrellas
    {
        float h = fract(float(matID)*0.17);
        vec3 base = pastel(palette(h));
        float pulse = 0.5 + 0.5*sin(uTime*2.0+h*6.28);
        col = base*(0.4+0.6*diff*pulse) + 0.2*spec;
    }
    else
    {
        col = vec3(1.0,0.0,1.0); // fallback magenta
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

    // --- Add subtle fog ---
    float t = length(ro); // approximate distance along the ray
    float fogAmount = 1.0 - exp(-0.02 * t); // controls density, tweak 0.02
    vec3 fogColor = vec3(0.7, 0.85, 1.0);   // light blue fog
    col = mix(col, fogColor, fogAmount);

    FragColor = vec4(col,1.0);
}
