vec4 QuaternionAdd(in vec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w + b.w;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

vec4 QuaternionSubtract(in vec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w - b.w;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

vec4 QuaternionMultiply(in nec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	
	return c;
}


vec4 QuaternionDivide(in vec4 a, in vec4 b)
{
	vec4 c;
	double s = b.w*b.w + b.x*b.x + b.y*b.y + b.z*b.z;
	
	c.w = (a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z) / s;
	c.x = (-a.w*b.x + a.x*b.w - a.y*b.z + a.z*b.y) / s;
	c.y = (-a.w*b.y + a.x*b.z + a.y*b.w - a.z*b.x) / s;
	c.z = (-a.w*b.z - a.x*b.y + a.y*b.x + a.z*b.w) / s;
	
	return c;
}

vec4 QuaternionExp(in vec4 a)
{
	vec4 c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double et = exp(q.w);
	double s = r == 0 ? 0 : et * sin(r) / r;
	
	q.w = et * cos(r);
	q.x = q.x * s;
	q.y = q.y * s;
	q.z = q.z * s;
	
	return c;
}

vec4 QuaternionLn(in vec4 a)
{
	vec4 c;
	double r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	double t = r == 0 ? 0 : atan(r, q.w) / r ;
	
	q.w = .5 * log(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	q.x = q.x * t;
	q.y = q.y * t;
	q.z = q.z * t;
	
	return c;
}

vec4 QuaternionPower(in vec4 q, in double p)
{
	return QuaternionExp(QuaternionMultiply(QuaternionLn(q),power));
}
