varying vec2 v_uv;
uniform float u_time2;
uniform float u_timeinf;

uniform int option;

void main()
{
	float x = v_uv.x;
	float y = v_uv.y;
	vec3 final_color;

	if (option == 0) {

		vec3 color_red = vec3(0.5*u_time2,0.0, 0.0); //vec3(1.0, 0.0, 0.0) for static color
		vec3 color_blue = vec3(0.0, 0.0, 0.5*u_time2); //vec3(0.0, 0.0, 1.0) for static color

		final_color =  x*color_red + (1-x)*color_blue;

	} else if (option == 1) {

		vec3 color_white = vec3(0.01*u_time2); //vec3(1.0) for static color
		vec3 color_black = vec3(u_time2); //vec3(0.0) for static color
		float dist = distance(v_uv, vec2(0.5));

		final_color = mix(color_black, color_white, 1.5*dist);

	}
	

	gl_FragColor = vec4(final_color, 1.0);

}
