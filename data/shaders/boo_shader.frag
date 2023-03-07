#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec3 u_mouse;
uniform float u_time;
uniform sampler2D texture;
uniform vec4 sfmlColor;

void main() {
    //vec2 st = gl_FragCoord.st/u_resolution;
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float intensity = (pixel.r + pixel.g + pixel.b) / 3.0;
    //vec4 rpixel = vec4(intensity, intensity, intensity, pixel.a);
    //float hue = abs(sin(u_time));
    //float hue = fract(u_time * 1.0) ;
    //float hue = 0.5 + 0.5 * cos(u_time * 2.0);
    //vec3 color = hsb2rgb(vec3(hue,1.0,1.0));
    gl_FragColor = vec4(pixel.r, pixel.g, pixel.b, pixel.a * 0.5) * sfmlColor;
}