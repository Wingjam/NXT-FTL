#include "follower.h"
#include <iostream>
#include <fstream>

using namespace nxtftl;

int main()
{
	follower follow{ 10 };
	follow.Run();
}

// TODO JP
// creer thread qui lit sur un vector. lit les positions et le output sur un fichier)
// indice de liste que felix est en train de remplir(int)
// le main donne le nombre de liste total
