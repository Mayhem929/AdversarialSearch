#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform sampler2D texture;

uniform vec3 colorA;
uniform vec3 colorB;

void main() {
    vec3 color = vec3(0.0);
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    float pct = abs(sin(u_time));

    // Mix uses pct (a value from 0-1) to
    // mix the two colors
    color = mix(colorA, colorB, pct);

    gl_FragColor = vec4(color,1.0) * pixel;
}
