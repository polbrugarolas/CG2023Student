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
	
	if (option == 0) {

		vec3 color_red = vec3(0.5*u_time2,0.0, 0.0); //vec3(1.0, 0.0, 0.0) for static color
		vec3 color_blue = vec3(0.0, 0.0, 0.5*u_time2); //vec3(0.0, 0.0, 1.0) for static color

		final_color =  x*color_red + (1-x)*color_blue;

	} else if (option == 1) {

		vec3 color_white = vec3(1.0);
		vec3 color_black = vec3(0);
		float dist = distance(v_uv, vec2(0.5));

		final_color = mix(color_black, color_white, 1.5*dist);
		
		//vec3 color_white = vec3(0.01*u_time2); //vec3(1.0) for static color
		//vec3 color_black = vec3(u_time2); //vec3(0.0) for static color
		//float dist = distance(v_uv, vec2(0.5));
		//final_color = mix(color_white, color_black, 1.5*dist); //mix(color_black, color_white, 1.5*dist)
	}
	else if (option == 2) {

		vec3 color_black = vec3(0);
		vec3 color_red = vec3(1.0 ,0.0, 0.0);
		vec3 color_blue = vec3(0.0, 0.0, 1.0);

		vec3 red_lines = mix(color_black,color_red,sin(x*120));
		//vec3 red_lines = step(mod(x,0.05),0.01)*color_red;
		vec3 blue_lines = mix(color_black,color_blue,sin(y*120));
		//vec3 blue_lines = step(mod(y,0.05),0.01)*color_blue;
		final_color = red_lines + blue_lines;

	}
	else if (option == 3) {

		vec3 color_black = vec3(0);
		vec3 color_red = vec3(x, 0.0, 0.0);
		vec3 color_green = vec3(0.0, y, 0.0);

		vec3 x_lines = mix(color_black, color_red, floor(x*15)/15);
		vec3 y_lines = mix(color_black, color_green, floor(y*15)/15);

		final_color = x_lines + y_lines;
	}

	else if (option == 4) {
		vec3 color_black = vec3(0);
		vec3 color_green_black = vec3(0.0, y, 0.0);
		vec3 color_black_green = vec3(0.0, (1-y), 0.0);
		
		float factor_trans = mod(u_time2,4);
		float factor = step(0.5+0.25*sin(factor_trans*x*2*3.1416), y);
		final_color =  mix(color_green_black,color_black_green, factor);
	}
	else if (option == 5) {
		vec3 color_black = vec3(0);
		vec3 color_white = vec3(1.0);

		float sum = floor(x*30) + floor(y*30);
		float even = step(1, mod(sum, 2));
		
		final_color = mix(color_white, color_black, even);
	}
	

	gl_FragColor = vec4(final_color, 1.0);

}
