#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <omp.h>
#include <time.h>
#include <device_launch_parameters.h>

using namespace std;
using namespace std::chrono;

__global__ void dotProductKernel(double *vector1, double *vector2, double *dotProduct, int size) {
    int i = threadIdx.x + blockDim.x * blockIdx.x;
    if (i < size) {
        dotProduct[i] = vector1[i] * vector2[i];
    }
}

void advancedCalculate(vector<double>& vector1, vector<double>& vector2){
    int size = vector1.size();
    double *dev_vector1 = nullptr, *dev_vector2 = nullptr, *dev_dotProduct = nullptr;
    double *dotProduct = new double[size];
    register double sum = 0;
    register double temp = 0;

    // 分配存储空间
    cudaMalloc((void **)&dev_vector1, size * sizeof(double));
    cudaMalloc((void **)&dev_vector2, size * sizeof(double));
    cudaMalloc((void **)&dev_dotProduct, size * sizeof(double));

    // 将输入向量拷贝到GPU全局内存中
    cudaMemcpy(dev_vector1, vector1.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_vector2, vector2.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    // 设置线程块大小
    int blockSize = 256;
    int numBlocks = (size + blockSize - 1) / blockSize;

    // 调用GPU核函数进行并行计算
    dotProductKernel<<<numBlocks, blockSize>>>(dev_vector1, dev_vector2, dev_dotProduct, size);

    // 将计算结果拷贝回主机内存中
    cudaMemcpy(dotProduct, dev_dotProduct, size * sizeof(double), cudaMemcpyDeviceToHost);

    // 进行求和操作
    for(int i = 0; i < size; i++){
        sum += dotProduct[i];
    }

    // 释放GPU内存
    cudaFree(dev_vector1);
    cudaFree(dev_vector2);
    cudaFree(dev_dotProduct);

    //输出结果
    cout << "结果为: " << sum << endl;
}
