varying vec2 v_uv;
uniform float u_time2;
uniform float u_timeinf;

uniform int option;

void main()
{
	float x = v_uv.x;
	float y = v_uv.y;
	vec3 final_color;

	float pass = mod(floor((u_timeinf-0.5)/5),3);

	if (pass == 0) {

		vec3 color_red = vec3(0.5*u_time2,0.0, 0.0); //vec3(1.0, 0.0, 0.0) for static color
		vec3 color_blue = vec3(0.0, 0.0, 0.5*u_time2); //vec3(0.0, 0.0, 1.0) for static color

		final_color =  x*color_red + (1-x)*color_blue;

	} else if (pass == 1) {

		vec3 color_white = vec3(1.0); //vec3(1.0) for static color
		vec3 color_black = vec3(0); //vec3(0.0) for static color
		float dist = distance(v_uv, vec2(0.5));

		final_color = mix(color_black, color_white, 1.5*dist);
		
		//vec3 color_white = vec3(0.01*u_time2); //vec3(1.0) for static color
		//vec3 color_black = vec3(u_time2); //vec3(0.0) for static color
		//float dist = distance(v_uv, vec2(0.5));
		//final_color = mix(color_white, color_black, 1.5*dist);
	}
	else if (pass == 2) {

		vec3 color_black = vec3(0); //vec3(0.0) for static color
		vec3 color_red = vec3(1.0 ,0.0, 0.0);
		vec3 color_blue = vec3(0.0, 0.0, 1.0);

		vec3 red_lines = mix(color_black,color_red,sin(x*120));
		//vec3 red_lines = step(mod(x,0.05),0.01)*color_red;
		vec3 blue_lines = mix(color_black,color_blue,sin(y*120));
		//vec3 blue_lines = step(mod(y,0.05),0.01)*color_blue;
		final_color = red_lines + blue_lines;

	}
	

	gl_FragColor = vec4(final_color, 1.0);

}
