#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform bool border;
uniform bool hovered;
uniform vec4 rect;
float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.98798, 4.1414))) * 43758.5453);
}
//float rand(vec2 co);
float dist(vec3 pos1, vec3 pos2);
float dist(vec2 pos1, vec2 pos2);
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.000 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

void main() {
    vec2 st =1.3e7*gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    
    float radius=dist(u_mouse, gl_FragCoord.xy);
    vec4 color = vec4(0.0);
    float rg=snoise(st)*.5+.5;
	color = vec4(rg,rg,rg,atan(7.0/radius));

    gl_FragColor = color;
}

//float rand(vec2 co){
  //  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 42758.5453);
//}
                      
float dist(vec3 pos1, vec3 pos2){
    vec3 dist=pow(vec3(pos1.x-pos2.x,pos1.y-pos2.y,pos1.z-pos2.z),vec3(2,2,2));
    return sqrt(dist.x+dist.y+dist.z);
}
float dist(vec2 pos1, vec2 pos2){
    vec2 dist=pow(vec2(pos1.x-pos2.x,pos1.y-pos2.y),vec2(2,2));
    return sqrt(dist.x+dist.y);
}