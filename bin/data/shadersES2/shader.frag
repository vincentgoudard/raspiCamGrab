
precision highp float;

uniform vec2 resolution;
uniform sampler2D tex0;

//varying vec2 texCoordVarying;
//uniform float mouseX;
//uniform float mouseY;
uniform vec4 rgbaGains;

void main()
{
 //   gl_FragColor = texture2D(tex0, texCoordVarying);
    //vec3 color = texture2D(tex0, (gl_FragCoord.xy + vec2(mouseX,0))/resolution.xy).rgb;
    //gl_FragColor = vec4(0.5,0.7,(mouseY/resolution.y),1)*texture2D(tex0, (gl_FragCoord.xy + vec2(mouseX,0))/resolution.xy);
    //gl_FragColor = rgbaGains*texture2D(tex0, (gl_FragCoord.xy)/resolution.xy);
    //gl_FragColor.rgb = color;

    gl_FragColor = rgbaGains*texture2D(tex0, (gl_FragCoord.xy)/resolution.xy);
    
    //gl_FragColor = 0.1*texture2D(tex0, (gl_FragCoord.xy)/resolution.xy);
    
}
