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

        vec2 center = vec2(0.5, 0.5);
        
        float angle = u_timeinf * 0.5;
        
        vec2 rotated_uv = vec2((v_uv.x - center.x) * cos(angle) - (v_uv.y - center.y) * sin(angle) + center.x, (v_uv.x - center.x) * sin(angle) + (v_uv.y - center.y) * cos(angle) + center.y);

        final_res = texture2D(street, rotated_uv);

	}
	else if (option == 2) {
		
		float pixel_size = 0.0001 + abs(0.03 * sin(u_time2 * 0.1+1.7));
        //float pixel_size = 0.1/(u_time2*10+1);
        
        vec2 rounded_uv = vec2(floor(v_uv.x / pixel_size) * pixel_size, floor(v_uv.y / pixel_size) * pixel_size);
     
        vec4 pixelated_color = vec4(0.0);
        int num_pixels = 0;
        for (float i = 0.0; i < pixel_size; i += 0.005) {
            for (float j = 0.0; j < pixel_size; j += 0.005) {
                pixelated_color += texture2D(street, rounded_uv + vec2(i, j));
                num_pixels++;
            }
        }
        pixelated_color /= float(num_pixels);
        
        final_res = pixelated_color;
	}

	gl_FragColor = final_res;

}
