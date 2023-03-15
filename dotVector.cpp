#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include<gmpxx.h>
#include <gmp.h>
#include <omp.h>
#include <time.h>

using namespace std;
using namespace std::chrono;

vector<double> generateVector(int n); 
void basicCalculate(vector<double> vector1, vector<double> vector2);
void gmpCalculate(vector<double>& vector1, vector<double>& vector2);

vector<double> generateVector(int n) {
    vector<double> nums(n);
    auto engine = default_random_engine(high_resolution_clock::now().time_since_epoch().count());
    auto distribution = uniform_real_distribution<double>(-10000, 10000);
    generate(nums.begin(), nums.end(), [&engine, &distribution](){ return distribution(engine); });
    return nums;
}

void basicCalculate(vector<double> vector1, vector<double> vector2){
     double sum = 0;

    for(int i = 0; i < vector1.size(); i++){
        sum += vector1[i] * vector2[i];     
    }
    cout << "结果为: " << sum << endl;
}

void advancedCalculate(vector<double>& vector1, vector<double>& vector2){
    int size = vector1.size();
    register double sum = 0;
    register double temp = 0;
    omp_set_num_threads(4); 
    #pragma omp parallel for reduction(+ : sum) //reduction(+ : sum)表示sum是一个共享变量，每个线程都有自己的sum，最后将所有线程的sum相加
    for(int i = 0; i < size; i++){
        temp = vector1[i] * vector2[i];
        sum += temp;
    }
    cout << "结果为: " << sum << endl;
}

void gmpCalculate(vector<double>& vector1, vector<double>& vector2){
    int size = vector1.size();

    mpf_t *vec1 = new mpf_t[size]; //申请内存
    mpf_t *vec2 = new mpf_t[size];

    omp_set_num_threads(4);

    #pragma omp parallel for //并行化循环
    for(int i = 0; i < size; i++){
        mpf_init_set_d(vec1[i], vector1[i]);
        mpf_init_set_d(vec2[i], vector2[i]);
    }
    mpf_t sum;
    mpf_t temp;
    mpf_init(sum);
    
    
    // #pragma omp parallel for reduction(+ : sum) //reduction(+ : sum)表示sum是一个共享变量，每个线程都有自己的sum，最后将所有线程的sum相加
    for(int i = 0; i < size; i++){
        mpf_init(temp);
        mpf_mul(temp,vec1[i], vec2[i]);     
        mpf_add(sum,sum,temp);
    }

    #pragma omp single  
    gmp_printf("结果为: %.10Ff\n", sum);
    mpf_clear(sum);

    #pragma omp parallel for
    for(int i = 0; i < size; i++){
        mpf_clear(vec1[i]);
        mpf_clear(vec2[i]);
    }
    delete[] vec1;
    delete[] vec2;
}


bool isInteger(const string& s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;
    
    char* end_pointer = nullptr;
    strtol(s.c_str(), &end_pointer, 10);
    
    return (*end_pointer == 0);
}


int main() {
bool continueFlag = true;
while(continueFlag){
    bool passFlag = false;
    int n = 0;
    vector<double> vector1;
    vector<double> vector2;
    string input;
    while(!passFlag){
        cout << "选择手动输入(1)还是自动生成(2): ";
        //先刷新缓冲区，否则会出现输入错误的情况
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> input;
        //接收cin，如果输入的不是整数，则让用户重新输入
        if(!isInteger(input)){
            cout << "输入错误，请重新输入" << endl;
            continue;
        }else{
            n = stoi(input);
        }
        if(n == 1){
            cout << "输入想生成的vector的长度: ";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin >> input;
            //如果n不是正整数，则让用户重新输入
            if(!isInteger(input)){
                cout << "输入错误，请重新输入" << endl;
                continue;
            }else{
                n = stoi(input);
            }
            vector1.resize(n);
            vector2.resize(n);
            cout << "输入第一个vector的元素: ";
            for(int i = 0; i < n; i++){
                cin >> vector1[i];
            }
            cout << "输入第二个vector的元素: ";
            for(int i = 0; i < n; i++){
                cin >> vector2[i];
            }
            passFlag = true;
        }
        else if(n == 2){
            cout << "输入想生成的vector的长度: ";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin >> n;
            //如果n不是正整数，则让用户重新输入
            if(n <= 0){
                cout << "输入错误，请重新输入" << endl;
                continue;
            }
            vector1 = generateVector(n);
            vector2 = generateVector(n);
            passFlag = true;
        }
        else{
            cout << "输入错误，请重新输入" << endl;
        }
    }

    //计算时间
    double start = omp_get_wtime( );
    advancedCalculate(vector1, vector2);
    double end = omp_get_wtime( );

    //打印用时
    std::cout << "用时： "<< float( end - start ) * 1000.0 << "ms" << std::endl; 
    //询问用户是否继续输入
    cout << "是否继续输入(1)或者退出(2): ";
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin >> n;
    if(n == 1){
        continueFlag = true;
    }
    else if(n == 2){
        continueFlag = false;
    }
 }
 return 0;
}
