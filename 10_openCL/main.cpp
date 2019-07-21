#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <vector>
#include <iostream>
#include <time.h>
#include <fstream>
#include <exception>

void randVec(int * vec, int size) {
	srand(time(NULL));

	int x;
	for (int i = 0; i < size; i++) {
		x = rand() % 100;
		vec[i] = x;
	}
}

void MultipleVectorHost(int * vec1,int * vec2,int* vecRes, int size) {
	SYSTEMTIME start, end;
	GetLocalTime(&start);

	std::vector<int> vecResult;
	for(int j = 0; j <1000; j++)
		for (int i = 0; i < size; i++)
			vecRes[i] = (vec1[i] * vec2[i]);

	GetLocalTime(&end);
	int dif = end.wMilliseconds - start.wMilliseconds;
	if (dif < 0)
		dif = 0;
	std::cout << "host time: " << dif << std::endl;
}

void MultipleVectorDevice(cl::Device device,int * vec1,int* vec2,int * _pVecResult, int _size) {
	SYSTEMTIME start, end;
	GetLocalTime(&start);
	
	std::cout <<"device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
	time_t begin = time(NULL);

	std::vector<cl::Device> contexDevice;
	contexDevice.push_back(device);
	cl::Context contex(contexDevice);
	cl::CommandQueue queue(contex, device);

	cl::Buffer inputVector1 = cl::Buffer(contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, _size * sizeof(int), vec1);
	cl::Buffer inputVector2 = cl::Buffer(contex, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, _size * sizeof(int), vec2);
	cl::Buffer outputVector1 = cl::Buffer(contex, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, _size * sizeof(int), _pVecResult);

	std::string kernel_code = 
		"void kernel mul(	__global const int* vec1,	"
		"__global const int* vec2,						"
		"__global int* vecRes,							"
		"int size){										"					
		"	int iJob = get_global_id(0);				"
		"	if (iJob >= size) return;					"
		"	vecRes[iJob] = vec1[iJob] * vec2[iJob];		"
		"}												";

	cl::Program::Sources sources(1,std::make_pair(kernel_code.c_str(), kernel_code.length() + 1) );

	cl::Program program(contex, sources);
	program.build(contexDevice);

	cl::Kernel kernel(program, "mul");

	int iArg = 0;
	kernel.setArg(iArg++, inputVector1);
	kernel.setArg(iArg++, inputVector2);
	kernel.setArg(iArg++, outputVector1);
	kernel.setArg(iArg++, _size);

	for (int iTest = 0; iTest < 1000; iTest++)
	{
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1024), cl::NDRange(32));
		queue.finish();
	}

	// Read buffer C into a local list
	queue.enqueueReadBuffer(outputVector1, CL_TRUE, 0, _size * sizeof(int), _pVecResult);

	GetLocalTime(&end);
	int dif = end.wMilliseconds - start.wMilliseconds;
	if (dif < 0) 
		dif = 0;
	std::cout << "time: " << dif << std::endl;
}

bool compare(int * mas1, int * mas2, int size) {
	for (int i = 0; i < size; i++)
		if (mas1[i] != mas2[i]) return false;
	return true;
}

void test(int lendth) {
	std::cout << "-------------------------------------\n";
	std::cout << "test lendth : " << lendth << std::endl;
	int * vec1 = new int[lendth]; 
	int * vec2 = new int[lendth];

	randVec(vec1, lendth);
	randVec(vec2, lendth);

	int * vec_for_host = new int[lendth];
	MultipleVectorHost(vec1, vec2, vec_for_host, lendth);

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	for (int idPlatform = 0; idPlatform < platforms.size(); idPlatform++) {
		std::vector<cl::Device> devices;
		platforms[idPlatform].getDevices(CL_DEVICE_TYPE_ALL, &devices);
		try {
			for (int idDevice = 0; idDevice < devices.size(); idDevice++) {
				int * vec_for_device = new int[lendth];
				MultipleVectorDevice(devices[idDevice], vec1, vec2, vec_for_device,lendth);
				if(compare(vec_for_host, vec_for_device,lendth))
					std::cout << "result by host is equal result by device" << std::endl << std::endl;
				else
					std::cout << "result by host is not equal result by device" << std::endl << std::endl;
				delete[] vec_for_device;
			}
		}
		catch (cl::Error e) {
			std::cout << e.what() << "(" << e.err() << ")" << std::endl;
		}
	}

	delete[] vec1;
	delete[] vec2;
	delete[] vec_for_host;
}

int main()
{
	test(512);
	test(1024);
	test(131072);

	std::cin.get();
	return 0;
}