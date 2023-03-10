// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
uniform sampler2D entity_texture;

void main()
{
	// Set the ouput color per pixel
	//vec3 color = normalize(v_world_normal);
	vec3 color = texture2D(entity_texture, v_uv).rgb;

	gl_FragColor = vec4( color, 1.0 );
}
