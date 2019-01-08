#include <iostream>
#include <vector>

//using namespace std;

int testFunc(std::vector<int> param) {
	param.push_back(23);

	return 1;
}

int main() {
	
	std::vector<int> abc = {1,2,3};

	testFunc(abc);

	std::cout << abc[3] << std::endl;

	return 0;
}