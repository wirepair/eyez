#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

uniform vec2 u_eye_location; // TODO: implement & use
uniform vec2 u_pupil_size;   // TODO: implement & use

//	<https://www.shadertoy.com/view/4dS3Wd>
//	By Morgan McGuire @morgan3d, http://graphicscodex.com
float hash(float n)
{ 
	return fract(sin(n) * 1e4); 
}

float hash(vec2 p) 
{ 
	return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); 
}

float noise(vec2 x) {
	vec2 i = floor(x);
	vec2 f = fract(x);

	// Four corners in 2D of a tile
	float a = hash(i);
	float b = hash(i + vec2(1.0, 0.0));
	float c = hash(i + vec2(0.0, 1.0));
	float d = hash(i + vec2(1.0, 1.0));

	// Simple 2D lerp using smoothstep envelope between the values.
	// return vec3(mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
	//			mix(c, d, smoothstep(0.0, 1.0, f.x)),
	//			smoothstep(0.0, 1.0, f.y)));

	// Same code, with the clamps in smoothstep and common subexpressions
	// optimized away.
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

const mat2 m = mat2(0.8, 0.6, -0.6, -0.8);

float fbm(vec2 p)
{
	float f = 0.0;
	f += 0.5000*noise(p); p *=m*2.02;
	f += 0.2500*noise(p); p *=m*2.03;
	f += 0.1250*noise(p); p *=m*2.01;
	f += 0.0625*noise(p); p *=m*2.04;	
	f /= 0.9375;
	return f;
}

void main() 
{
	vec2 q = gl_FragCoord.xy/u_resolution.xy;
	vec2 p = -1.0 + 2.0*q;
	p.x *= u_resolution.x/u_resolution.y;

	float r = sqrt(dot(p,p));
	float a = atan(p.y, p.x);
	vec3 col = vec3(1.0);

	

	if (r < 0.5)
	{
		col = vec3(0.0, 0.3, 0.4); // base blue
		
		float f = fbm(5.0*p);
		col = mix(col, vec3(0.2, 0.5, 0.4), f); // mix with greenish

		// size of center of iris
		f = 1.0 - smoothstep(0.2, 0.3, r);
		col = mix(col, vec3(0.9, 0.6, 0.2), f); // orange-ish center

		a += 0.2*fbm(5.0*p);

		// scelera
		f = smoothstep(0.3, 1.0, fbm(vec2(12.0*r,20.0*a)));
		col = mix(col, vec3(1.0), f);

		f = smoothstep(0.4, 1.6, fbm(vec2(10.0*r,15.0*a)));
		col *= 1.20 - f;

		f = smoothstep(0.4, 0.9, r);
		col *= 1.0 - 0.5*f;

		// pupil

		// pupil dialation
		float ss = 0.7 + 0.7*sin(10.0*u_time);
		float anim = 1.0 + 0.1*ss*clamp(1.0-r,0.0,1.0);
		r *= anim;

		// size
		f = smoothstep(0.1, 0.2, r);
		col *= f;

		//float ss = 0.5 + 0.5*sin(10.0*u_time);
		//float anim = 1.0 + 0.1*ss*clamp(0.5-r,0.0,1.0);
		

		// reflection
		//f = 1.0 - smoothstep(0.0, 0.175, length(p - vec2(0.1,0.1)));
		//col += vec3(f);

		// fix alias
		f = smoothstep(0.4, 0.8, r);
		col = mix(col, vec3(1.0), f);

	}

	gl_FragColor = vec4(col,1.0);
}