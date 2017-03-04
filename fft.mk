all:
	g++ -L/opt/AMDAPPSDK-3.0/lib/x86_64/ -I/opt/AMDAPPSDK-3.0/include/ run.c Driver_fft.cpp Fournier_one.cpp Brot.cpp -o t.o -lOpenCL
