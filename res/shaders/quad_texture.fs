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

	}

	else if (option == 4) {
	
	}
	

	gl_FragColor = final_res;

}
