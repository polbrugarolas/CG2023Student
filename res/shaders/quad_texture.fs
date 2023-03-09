varying vec2 v_uv;
uniform float u_time2;
uniform float u_timeinf;

uniform int option;
uniform sampler2D fruits;

void main()
{
	float x = v_uv.x;
	float y = v_uv.y;
	vec4 final_res;
	vec4 image = texture2D(fruits,v_uv);

	float pass = mod(floor((u_timeinf-0.5)/5),3);
	
	if (option == 0) {

		final_res = image;

	} else if (option == 1) {
		float total = (image.x+image.y+image.z)/3;
		image.x = total;
		image.y = total;
		image.z = total;
		final_res = image;
	}
	else if (option == 2) {
		image.x = 1.0 - image.x;
		image.y = 1.0 - image.y;
		image.z = 1.0 - image.z;
		final_res = image;
	}
	else if (option == 3) {
		float i_x = image.x;
		float i_y = image.y;
		float i_z = image.z;
		image.x = 1.0 - i_z;
		image.y = i_x;
		image.z = i_y/2;
		final_res = image;
	}
	else if (option == 4) {
		float total = (image.x+image.y+image.z)/3;
		image.x = floor(total+0.6);
		image.y = floor(total+0.6);
		image.z = floor(total+0.6);
		final_res = image;
	}
	else if (option == 5) {
		vec4 image = (texture2D(fruits,v_uv+vec2(-0.005))+texture2D(fruits,v_uv+vec2(0.005))+texture2D(fruits,v_uv+vec2(-0.005,0))+texture2D(fruits,v_uv+vec2(0,-0.005))+texture2D(fruits,v_uv+vec2(0.005,0))+texture2D(fruits,v_uv+vec2(0,0.005)))/6;

		final_res = image;
	}
	else if (option == 6) {
		vec3 color_white = vec3(1.0);
		vec3 color_black = vec3(0);
		float dist = distance(v_uv, vec2(0.5));
		vec3 final_color = mix(color_black, color_white, 1.5*dist);
		image.rgb = image.rgb - final_color;
		final_res = image;
	}
	

	gl_FragColor = final_res;

}
