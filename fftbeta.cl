__kernel void fft(__global float *input, __global float *output, int pivot, int len,  int N)
{
	barrier(CLK_LOCAL_MEM_FENCE);
	float PI = 3.14159265f;
	int ind = get_global_id(0);

	if(N == 1){
		int inr = pivot;
		int thr = pivot/3;
		int L = 512;
		int a;
		int b = log2((float)L*L) -1;
		int swap;
		int sum;

		while( inr < pivot + len)
		{
			a = 0;
			sum = 0;
			swap = thr;

			while(a <= b && swap > 0)
			{
				if(swap%2 == 1)
					sum += (int)pow(2, (float)(b - a));
				a++;
				swap = swap/2;

			}
			if(thr < L*L){
			output[ind + sum] = (input[ind + inr] + input[ind + inr++] + input[ind + inr++])/3;
			//printf("%d %1f\n", sum, input[ind + inr]);
			inr++;}
			else{output[ind + sum] = 0; inr += 3;}
			thr++;
		}


	}

	else{
		int radix = N/2;
		int k = pivot%(N/2);
		for(int y = pivot; y < pivot + len; y++){			
			float swapx = native_cos((float)(-2*PI*k*(y))/N)*input[ind + y];
			float swapy = native_sin((float)(-2*PI*k*(y + radix))/N)*input[ind + (y + radix)];
			output[ind + y] = swapx + swapy;
			output[ind + y + radix] = swapx - swapy;
			k++;			
			if(k >= N/2){
				radix = -radix;
				k = 0;
			}
		}
	}
}
