#version 400

in vec3 direction;

uniform double step;
uniform int bail;
uniform vec3 camera;
uniform vec4 color;

bool mandelTest(in vec3 point) {
    vec3 v = point;
    vec3 c = point;

    for (int i = 0; i < bail; i++) {
        v = nextPoint(v, c, 2.0);
        if (v.x*v.x + v.y*v.y + v.z*v.z <= 2.0) return true;
    }
    return false;
}

vec3 nextPoint (vec3 v, vec3 c, double power){

	double x = 0;
	double y = 0;
	double z = 0;
	
	double xx = v.x * v.x;
	double yy = v.y * v.y;
	double zz = v.z * v.z;
	
	if (equals(power, 1.0))
		return v;
	if (equals(power, 2.0))
	{
		x = -(2*v.x*v.y * ( xx + yy - zz )) / (( xx + yy ) * ( xx + yy + +v.z*v.z));
		y = -x;
		z = (v.x*v.x-v.y*v.y) / (v.x*v.x-v.y*v.y);
		
		return vec3(x, y, z);
	}
	
	double rN = pow( sqrt( xx + yy + zz ), power );
	double nPhi =  power * atan( v.y, v.x ) ;
	double nTheta = power * acos( v.z / sqrt( xx + yy + zz ) );
	
	double sinNTheta = sin( nTheta);
	
	x = sinNTheta * cos(nPhi);
	y = sinNTheta * sin(nPhi);
	z = cos(nTheta);
	
	return vec3(rN*x + c.x, rN*y + c.y, rN*z + c.z);
}

int equals(double a, double b) {
    double epsilon = 0.000000001;
    return (abs(a-b) < epsilon);
}

void main() {    
    vec3 pos = camera;
    
    while (!mandelTest(pos))
        pos = pos + step*direction;

    outputF = color/length(pos-camera);
}
