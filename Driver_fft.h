#ifndef DRIVER_FFT
#define DRIVER_FFT
#define MAX_SOURCE_SIZE (0x100000)

class Driver_fft{
	private:
		unsigned char* raw_data;
		cl_uint num_of_platforms;
		cl_uint num_of_devices;
		cl_platform_id platform_id;

	protected:		
		cl_kernel kernel;
		cl_device_id device_id;
		cl_context context;
		cl_program program;
		cl_command_queue command_queue;
		cl_mem ibuffer;
		cl_mem obuffer;
		int ret;
		char *source_str;
		size_t source_size;

		static float* flt_data;
		static size_t pic_size;
		static int imgL;
		static int scale;
		static int is;

		void init_float();
		Driver_fft();
};



#endif
