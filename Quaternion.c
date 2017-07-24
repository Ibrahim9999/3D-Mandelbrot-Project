

vec4 QuaternionAdd(vec4 a, vec4 b)
{
	vec4 c;
	
	c.w = a.w + b.w;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

vec4 QuaternionSubtract(vec4 a, vec4 b)
{
	vec4 c;
	
	c.w = a.w - b.w;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

vec4 QuaternionMultiply(vec4 a, vec4 b)
{
	vec4 c;
	
	c.w = a.W*b.W - a.X*b.X - a.Y*b.Y - a.Z*b.Z;
	c.x = a.W*b.X + a.X*b.W + a.Y*b.Z - a.Z*b.Y;
	c.y = a.W*b.Y - a.X*b.Z + a.Y*b.W + a.Z*b.X;
	c.z = a.W*b.Z + a.X*b.Y - a.Y*b.X + a.Z*b.W;
	
	return c;
}


vec4 QuaternionDivide(vec4 a, vec4 b)
{
	vec4 c;
	double s = b.W*b.W + b.X*b.X + b.Y*b.Y + b.Z*b.Z;
	
	c.w = (a.W*b.W + a.X*b.X + a.Y*b.Y + a.Z*b.Z) / s;
	c.x = (-a.W*b.X + a.X*b.W - a.Y*b.Z + a.Z*b.Y) / s;
	c.y = (-a.W*b.Y + a.X*b.Z + a.Y*b.W - a.Z*b.X) / s;
	c.z = (-a.W*b.Z - a.X*b.Y + a.Y*b.X + a.Z*b.W) / s;
	
	return c;
}

vec4 QuaternionExp(vec4 a)
{
	vec4 c;
	double r = sqrt(q.X*q.X + q.Y*q.Y + q.Z*q.Z);
	double et = exp(q.W);
	double s = r == 0 ? 0 : et * sin(r) / r;
	
	q.W = et * cos(r);
	q.X = q.X * s;
	q.Y = q.Y * s;
	q.Z = q.Z * s;
	
	return c;
}

vec4 QuaternionLn(vec4 a)
{
	vec4 c;
	double r = sqrt(q.X*q.X + q.Y*q.Y + q.Z*q.Z);
	double t = r == 0 ? 0 : atan(r, q.W) / r ;
	
	q.W = .5 * log(q.W*q.W + q.X*q.X + q.Y*q.Y + q.Z*q.Z);
	q.X = q.X * t;
	q.Y = q.Y * t;
	q.Z = q.Z * t;
	
	return c;
}

vec4 QuaternionPower(vec4 q, double p)
{
	return QuaternionExp(QuaternionMultiply(QuaternionLn(q),power));
}