#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D texture;
uniform vec2 u_resolution;
uniform float u_time;

float getA(float time) {
    // Calculate a value between 0 and 9 based on time
    return floor(mod(time * 10.0, 10.0));
}

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution.xy;
    vec4 color = texture2D(texture, st);
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float a = getA(u_time);
    float lastDigit = floor(mod(gl_FragCoord.x, 10.0));
    if (a < 5.0 &&lastDigit >= a && lastDigit <= mod(a+5.0, 10.0)) {
        // If the last digit is between a and (a+5)%10, gray the pixel
        color *= 0.5;
    }
    else if(a >= 5.0 && (lastDigit >= a || lastDigit <= mod(a+5.0, 10.0))) {
        // If the last digit is between a and (a+5)%10, gray the pixel
        color *= 0.5;
    }

    gl_FragColor = color * pixel;
}
