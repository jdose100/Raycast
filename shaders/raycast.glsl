@vs vertex
in vec2 position;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}

@end // vertext shader

@fs fragment
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}

@end // fragment shader

@program raycast vertex fragment