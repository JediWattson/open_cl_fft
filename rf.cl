__kernel void raw_flt(__global unsigned char* r, __global float* f, int diff, int pivot, int len)
{
	int ind = get_global_id(0);
	for(int i = pivot; i <= pivot + (len-diff); i++){f[ind + i] = (float)r[ind + i+diff]/255.0;}
}
