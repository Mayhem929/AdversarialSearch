#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec3 u_mouse;
uniform float u_time;
uniform sampler2D texture;
uniform vec4 sfmlColor;

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main() {
    vec2 st = gl_FragCoord.st/u_resolution;
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float intensity = (pixel.r + pixel.g + pixel.b) / 3.0;
    vec4 rpixel = vec4(intensity, intensity, intensity, pixel.a);
    //float hue = abs(sin(u_time));
    float hue = fract(u_time * 1.0) ;
    //float hue = 0.5 + 0.5 * cos(u_time * 2.0);
    vec3 color = hsb2rgb(vec3(hue,1.0,1.0));

    // Determine the distance from the center of the sprite
    float dist = distance(st, vec2(0.5));
    // If the distance is less than 0.5 (i.e., the center of the sprite), use the original color
    gl_FragColor = vec4(color,1.0) * rpixel * sfmlColor;
}