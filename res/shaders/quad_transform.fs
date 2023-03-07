varying vec2 v_uv;
uniform float u_time2;
uniform float u_timeinf;

uniform int option;
uniform sampler2D street;

void main()
{
	float x = v_uv.x;
	float y = v_uv.y;
	vec4 final_res;
	vec4 image = texture2D(street,v_uv);

	float pass = mod(floor((u_timeinf-0.5)/5),3);

	if (option == 0) {
		final_res = image;

	} else if (option == 1) {
		final_res = image;
	}
	else if (option == 2) {
		final_res = image;
	}
	gl_FragColor = final_res;

}
