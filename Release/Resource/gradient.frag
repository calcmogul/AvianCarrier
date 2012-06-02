uniform float width;
uniform float height;

void main() {
    vec4 pixel = gl_FragColor;
    float x = gl_FragCoord.y / height;
    float y = gl_FragCoord.x / width;

    gl_FragColor.r = 0.4 * ( 1 - abs( x - 0.5 ) ) * ( 1 - abs( y - 0.5 ) );
    gl_FragColor.g = 0.4 * ( 1 - abs( x - 0.5 ) ) * ( 1 - abs( y - 0.5 ) );
    gl_FragColor.b = 0.4 * ( 1 - abs( x - 0.5 ) ) * ( 1 - abs( y - 0.5 ) );

    gl_FragColor.a = 1.0;
}