#include<iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>

using namespace std;


double calc(const char *num1, const char *operation, const char *num2) {

	if (std::string(operation) == "+")
	{
		return atof(num1) + atof(num2);
	}

	if (std::string(operation) == "-")
	{
		return atof(num1) - atof(num2);
	}

	if (std::string(operation) == "*")
	{
		return atof(num1) * atof(num2);
	}

	if (std::string(operation) == "/")
	{
		return atof (num1) / atof(num2);
	}

	std::cerr << "undefined operator \"" << operation << "\"" << std::endl;
}

int main(int argc, char** argv) 
{
	if (argc == 4) 
	{
		if (atof(argv[3]) == 0 && std::string(argv[2]) == "/")
			std::cerr << "divizion by zero" << std::endl;
		else
			std::cout << calc(argv[1], argv[2], argv[3]) << std::endl;
	}

	else if (argc == 3)
	{
		std::ifstream in(argv[1]);
		std::ofstream out(argv[2]);

		if (in.is_open() && out.is_open())
	    {
	    	string num1, num2, operation;

	        while (in >> num1 >> operation >> num2)
	        {
	            if (atof(num2.c_str()) == 0 && std::string(operation.c_str()) == "/") {
	            	std::cerr << "divizion by zero" << std::endl;
	            	out << num1 + operation + num2 << " = " << "divizion by zero" << endl;
	            }

	            else {
	            	std::cout << calc(num1.c_str(), operation.c_str(), num2.c_str()) << std::endl;
	            	out << num1 + operation + num2 << " = " << calc(num1.c_str(), operation.c_str(), num2.c_str()) << endl;
	            }
	        }
	    }
		
		in.close();
		out.close();
	}

	else 
	{
		std::cerr << ">> error. no numbers to calculate" << std::endl;
		return -1;
	}

	return 0;
}