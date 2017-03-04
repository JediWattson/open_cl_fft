#include <CL/cl.h>

#include "Fournier_one.h"
#include "Brot.h"
#include <fstream>
#include <istream>

#define __NO_STD_VECTOR // Use cl::vector instead of STL version

int main(void){

	Fournier_one f1;
	Brot b;
	f1.FFT_1();
	b.bset();

	return 0;
}
