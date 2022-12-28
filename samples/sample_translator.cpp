#include "translator.h"
#include <iostream>

int main() {
	Lexeme a("Jesus1+2");
	std::cout << a.all << " " << a.numbers << " " << a.operators << " " << a.variables << std::endl;

	return 0;
}