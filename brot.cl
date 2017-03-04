__kernel void fracta(__global float *input, __global float *output, int pivot, int len)
{
	int ind = get_global_id(0);

	for(int i = pivot; i < pivot + len; i = i+2){
		float swapa = input[i + ind]*input[i + ind];
		float swapb = input[i + ind + 1]*input[i + ind + 1];
		float swapc = 0;
		for(int e = 0; e < 1000; e++){swapc =  swapa*swapa + swapb*swapb + swapc;}
		output[i/2 + ind] = swapc; // printf("%d %1f\n", i/2, output[i/2 + ind]);
	}		
}
