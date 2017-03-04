#include <CL/cl.h>
#include <fstream>
#include "Driver_fft.h"

float* Driver_fft::flt_data;
size_t Driver_fft::pic_size;
int Driver_fft::imgL;
int Driver_fft::scale;
int Driver_fft::is;

Driver_fft::Driver_fft(){
	clGetPlatformIDs(1, &platform_id, &num_of_platforms);
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id,
		   	&num_of_devices);
	raw_data = (unsigned char*)malloc(MAX_SOURCE_SIZE);
	flt_data = (float*)malloc(MAX_SOURCE_SIZE);
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	context = clCreateContext(NULL, 1 , &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	imgL = 512;
	scale = 64;
	is = (imgL*scale)*3;
}


void Driver_fft::init_float(){

	/*load picture into an array called data*/
    	FILE* f = fopen("22.bmp", "rb");
    	pic_size = fread(raw_data, sizeof(unsigned char), MAX_SOURCE_SIZE, f);
    	fclose(f);
	int diff = pic_size - (imgL*imgL*3);
	pic_size -= diff;


	/*load the program from file, this breaks it into the string by string and gives the function a number of strings*/
	FILE* fp = fopen("rf.cl", "r");
	if(!fp){fprintf(stderr, "Failed to load kern.\n"); exit(1);}

	/*sets the size of strings and all the strings*/
	source_size = fread(source_str, 1 , MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/*creates object program and builds it, if error returns the specific issue to the buffer which gets initialized*/
	program = clCreateProgramWithSource(context, 1 , (const char **)&source_str, 
						&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, "", NULL, NULL);

	if(ret < 0){
		char buffer[10240];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
		fprintf(stderr, "CL Compilation:\n%s", buffer);
	}

	/*creates object kernel and initializes with the function following*/
	kernel = clCreateKernel(program, "raw_flt", &ret);

	ibuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
				 sizeof(unsigned char)*pic_size, NULL, &ret);
	obuffer = clCreateBuffer(context, CL_MEM_READ_WRITE,
				 sizeof(float)*pic_size, NULL, &ret);

	ret = clSetKernelArg(kernel, 0, sizeof(ibuffer), &ibuffer);
	ret = clSetKernelArg(kernel, 1, sizeof(obuffer), &obuffer);
	ret = clSetKernelArg(kernel, 2, sizeof(diff), &diff);
	ret = clSetKernelArg(kernel, 4, sizeof(is), &is);

	size_t global_work_size[1] = {1};

	for(int x = 0; x < (imgL/scale); x++){
		int p = is*x;
		ret = clSetKernelArg(kernel, 3, sizeof(p), &(p));
		ret = clEnqueueWriteBuffer(command_queue, ibuffer, CL_TRUE, sizeof(unsigned char)*p, 
				  	   sizeof(unsigned char)*is, raw_data, 0, NULL, NULL);
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
					     global_work_size, NULL,
					     0, NULL, NULL);	
	}

	ret = clEnqueueReadBuffer(command_queue, obuffer, CL_TRUE, 0,
				  sizeof(float)*pic_size, flt_data, 0, NULL,NULL);
}
