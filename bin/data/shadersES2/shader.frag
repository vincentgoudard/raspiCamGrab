
precision highp float;

uniform vec2 resolution;
varying vec2 texCoordVarying;
uniform sampler2D tex0;
uniform vec4 vHSVC;
uniform vec3 RGBgains;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    //vec4 textureColor = texture2D(tex0, (gl_FragCoord.xy)/resolution.xy);
    vec4 textureColor = texture2D(tex0, texCoordVarying);
    vec3 fragRGB = textureColor.rgb;
    vec3 fragHSV = rgb2hsv(fragRGB).xyz;
    fragHSV.x += mod(vHSVC.x, 1.0);
    fragHSV.yz *= vHSVC.yz;
    //fragHSV.yz = clamp(vHSVC.yz, 0., 1.0);

    fragRGB = hsv2rgb(fragHSV);
    
    fragRGB = RGBgains * ((fragRGB - 0.5) * vHSVC.w + 0.5);
    
    gl_FragColor = vec4(fragRGB, textureColor.w);
} 
