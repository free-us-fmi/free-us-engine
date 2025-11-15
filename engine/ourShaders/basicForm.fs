#version 330 core

uniform vec2 iRsolution;
uniform float iTme;
uniform float iFrame ;


struct object{
    int type;
    vec3 position;
    vec3 rotation;//euler angles //unghiuri pe axe in radiani
    vec3 scale;
};


uniform int no_objects;
uniform object objects[16];

struct light{
    vec3 direction;
    vec3 color;
};

uniform light lights[4];
uniform int no_lights;
vec2 iResolution=vec2(1900.f,1800.f);
float iTime=3;

out vec4 FragColor;


//------------------------------------------------
//    BASIC SHAPES
//------------------------------------------------
// -------- SDFs --------

//MAPPING
//Ellipsoid = 0
//Rectangular Prism = 1
//EllipticCone=2 
//EllipticCappedCylinder=3
//Pyramid=4
//Tetrahedron=5
//=6


//0
float sdEllipsoid( vec3 p, vec3 r )
{
  float k0 = length(p/r);
  float k1 = length(p/(r*r));
  return k0*(k0-1.0)/k1;
}
//1
float sdRectangularPrism( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}
//2
float sdEllipticCone(vec3 p, vec3 size)
{
    vec2 r=size.xy;
    float h=size.z;
    // Normalize xz by the ellipse radii
    vec2 k = p.xz / r;

    // Project point into 2D cone space
    float qx = length(k);
    float qy = p.y;

    // q = apex ? base vector in this 2D space
    // Apex at (0,h), base at (1,0)
    vec2 q = vec2(h/r.y, -1.0);

    // w = point in same space
    vec2 w = vec2(qx, qy);

    // Two distances from Inigo's canonical cone SDF
    vec2 a = w - q * clamp(dot(w, q) / dot(q, q), 0.0, 1.0);
    vec2 b = w - q * vec2(clamp(w.x / q.x, 0.0, 1.0), 1.0);

    float d = min(dot(a,a), dot(b,b));

    // Sign to know if inside or outside
    float s = max(
        q.y * (w.x * q.y - w.y * q.x),
        q.y * (w.y - q.y)
    );

    // Undo elliptical scaling in xz
    float scale = min(r.x, r.y);

    return sqrt(d) * sign(s) * scale;
}


//3
float sdEllipticCappedCylinder(vec3 p, vec3 scale)
{
    // Normalize xz to unit circle using ellipse radii
    vec2 d = abs(vec2(p.x / scale.x, p.z / scale.y)) - vec2(1.0, 0.0);

    // Apply height (scale.z) along y-axis
    d.y = abs(p.y) - scale.z;

    // Standard SDF combination for capped shapes
    return min(max(d.x,d.y), 0.0) + length(max(d, 0.0));
}


//4
float sdPyramid(vec3 p, vec3 scale)
{
    vec3 q = p / scale;
    q.xz = abs(q.xz);
    if (q.z > q.x) q.xz = q.zx;

    // Shift to center pyramid base at origin
    q.xz -= 0.5;

    // Compute helper values for SDF
    float m2 = 1.0 + 0.25; // unit pyramid: height^2 + 0.25
    vec3 r = vec3(q.z, q.y - 0.5*q.x, q.x + 0.5*q.y);

    float s = max(-r.x, 0.0);
    float t = clamp((r.y - 0.5*q.z)/(m2 + 0.25), 0.0, 1.0);

    float a = m2*(r.x + s)*(r.x + s) + r.y*r.y;
    float b = m2*(r.x + 0.5*t)*(r.x + 0.5*t) + (r.y - m2*t)*(r.y - m2*t);

    float d2 = min(r.y, -r.x*m2 - r.y*0.5) > 0.0 ? 0.0 : min(a, b);

    // Scale back to actual pyramid size
    float sdf = sqrt((d2 + r.z*r.z)/m2) * sign(max(r.z, -q.y));

    // Multiply by height scale to get correct SDF magnitude
    return sdf * scale.z;
}


//5
float sdTetrahedron(vec3 p, vec3 scale)
{
    // Normalize point to unit tetrahedron
    vec3 q = p / scale;

    // Normals for the four faces of a unit tetrahedron centered at origin
    const float sqrt3 = 1.7320508;
    vec3 n1 = normalize(vec3( 1.0, 1.0, 1.0));
    vec3 n2 = normalize(vec3(-1.0, 1.0, 1.0));
    vec3 n3 = normalize(vec3( 1.0,-1.0, 1.0));
    vec3 n4 = normalize(vec3( 1.0, 1.0,-1.0));

    // Signed distances to the planes
    float d1 = dot(q, n1) - 1.0;
    float d2 = dot(q, n2) - 1.0;
    float d3 = dot(q, n3) - 1.0;
    float d4 = dot(q, n4) - 1.0;

    // Max of face distances
    float sdf = max(max(d1, d2), max(d3, d4));

    // Scale back to actual size
    return sdf * min(scale.x, min(scale.y, scale.z));
}

//6



//HELPER FUNCTIONS
mat2 Rot(float a){
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

//rotation,scale,postion
float distObject(vec3 abspos,object obj){
    vec3 pos = obj.position;
    pos=-pos+abspos;
    int type=obj.type;
    vec3 rotation=obj.rotation;
    vec3 scale=obj.scale;

    pos.yz*=Rot(rotation.x);
    pos.xz*=Rot(rotation.y);
    pos.xy*=Rot(rotation.z);
    switch(type){
    case 0:
        return sdEllipsoid(pos,scale);
        
    case 1:
        return sdRectangularPrism(pos,scale);
        
    case 2:
        return sdEllipticCone(pos,scale);
    case 3:
        return sdEllipticCappedCylinder(pos,scale);
    case 4:
        return sdPyramid(pos,scale);
    //case 5:
        //return sdTetrahedron(pos,scale);
    }

}

// --------- Scene SDF (rotation applied) ---------
float mapScene(vec3 pos)
{
    
    float d;
    
    if(no_objects>0){
        d=distObject(pos,objects[0]);
        
        for(int i=1;i<no_objects;i++){
              float sd=distObject(pos,objects[i]);
            d=min(d,sd);
        }
    }
    else d=-1;
    return d;
}

// -------- Lighting / Normal --------
vec3 getNormal(vec3 p)
{
    const float e = 0.001;
    return normalize(vec3(
        mapScene(p + vec3(e,0,0)) - mapScene(p - vec3(e,0,0)),
        mapScene(p + vec3(0,e,0)) - mapScene(p - vec3(0,e,0)),
        mapScene(p + vec3(0,0,e)) - mapScene(p - vec3(0,0,e))
    ));
}

// -------- Raymarch --------
float raymarch(vec3 ro, vec3 rd)
{
    float t = 0.0;
    for(int i = 0; i < 120; ++i)
    {
        vec3 p = ro + rd * t;
        float d = mapScene(p);
        if(d < 0.01) return t;
        t += d;
        if(t > 20.0) break;
    }
    return -1.f;
}

// -------- Main --------
void main()
{

    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = (fragCoord - 0.5 * iResolution.xy) / iResolution.y;

    // Camera setup
    vec3 ro = vec3(0.0, 0.7, 8.0);
    vec3 rd = normalize(vec3(uv, -1.3));

    float tHit = raymarch(ro, rd);

    if(tHit > 0.0)
    {

        vec3 lightPos = vec3(2.0, 3.0, 2.0);
        vec3 p = ro + rd * tHit;
        vec3 n = getNormal(p);
        //vec3 n = normalize(lightPos - p);

        // ambient
        vec3 ambient = vec3(0.1, 0.18, 0.3);

        vec3 diffuseColor = vec3(0.0);
        diffuseColor += vec3(0.4, 0.7, 1.0) * 0.2;
        diffuseColor += vec3(0.3, 0.5, 0.8) * 0.2;
        diffuseColor += vec3(0.2, 0.4, 0.6) * 0.2;
        diffuseColor += vec3(0.1, 0.3, 0.5) * 0.2;
        diffuseColor += vec3(0.0, 0.2, 0.4) * 0.2;

        // final lighting accumulator
        vec3 col = ambient;

        for(int i = 0; i < no_lights; i++)
        {
            // dir lights: direction *towards* the surface
            vec3 l = normalize(-lights[i].direction);

            float diff = max(dot(n,l), 0.0);
            col += diff * lights[i].color;
        }

        // tone mapping
       // col = pow(col, vec3(1.0/2.2));
        
        FragColor = vec4(col, 1.0);
    }
    else
    {
        FragColor = vec4(0.5, 0.02, 0.03, 1.0);
    }
}
