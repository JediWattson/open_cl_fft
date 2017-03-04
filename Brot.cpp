#include <CL/cl.h>
#include "Driver_fft.h"
#include "Brot.h"

#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <iostream>
#include <iterator>

float* Brot::bset(){

	/*load the program from file, this breaks it into the string by string and gives the function a number of strings*/
	FILE* fp = fopen("./brot.cl", "r");
	if(!fp){fprintf(stderr, "Failed to load kern.\n"); exit(1);}

	/*sets the size of strings and all the strings*/
	source_size = fread(source_str, 1 , MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/*creates object program and builds it, if error returns the specific issue to the buffer which gets initialized*/
	program = clCreateProgramWithSource(context, 1 , 
					(const char **)&source_str, 
					&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, "", NULL, NULL);
	if(ret < 0){
		char buffer[10240];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
		fprintf(stderr, "CL Compilation:\n%s", buffer);
	}

	/*creates object kernel and initializes with the function following*/

	kernel = clCreateKernel(program, "fracta", &ret);

	ibuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
				 sizeof(float)*pic_size*2, NULL, &ret);

	obuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
				 sizeof(float)*pic_size/3, NULL, &ret);

	ret = clSetKernelArg(kernel, 0, sizeof(ibuffer), &ibuffer);
	ret = clSetKernelArg(kernel, 1, sizeof(obuffer), &obuffer);

	size_t global_work_size[1] = {1};
	is*=2;
	ret = clSetKernelArg(kernel, 3, sizeof(is), &is);

	for(int x = 0; x < (imgL/scale); x++){
		int p = is*x;
		ret = clSetKernelArg(kernel, 2, sizeof(p), &(p));
		ret = clEnqueueWriteBuffer(command_queue, ibuffer, CL_TRUE, sizeof(float)*p, 
				  	   sizeof(float)*2*is, flt_data, 0, NULL, NULL);
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
					     global_work_size, NULL,
					     0, NULL, NULL);
	}

	ret = clEnqueueReadBuffer(command_queue, obuffer, CL_TRUE, 0, 
				  sizeof(float)*(pic_size)/3, flt_data, 0, NULL,NULL);

	for(int i = 0; i < pic_size/3; i++){printf("%d .. %1f\n", i, flt_data[i]);}

	char buffer[44];
    	FILE* f = fopen("aaa.txt", "w");
	for(long i = 0; i < (pic_size/3); i++){
		if(i%imgL == 0 && i > 0)
			sprintf(buffer, " ,%1f\n", flt_data[i]);	
		else
			sprintf(buffer, " ,%1f", flt_data[i]);
		fputs (buffer, f);
	}
}

