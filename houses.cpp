#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <set>
#include <map>

using namespace std;

int INF = 10e6; // Valor para indicar que no hubo solución.

struct House {
	int beneficio;
	int contagio;
};

// Información de la instancia a resolver.
int n, W;
vector<House> houses;

int FB(int i, int w, int k)
{
	if(i >= n){
		if(w <= W){
			return k;
		}else{
			return -INF;
		}
	}

	int k1 = FB(i + 2, w + houses[i].contagio, k + houses[i].beneficio);
	int k2 = FB(i + 1, w, k);

	return max(k1, k2);
}

// Recibe por parámetro qué algoritmos utilizar para la ejecución separados por espacios.
// Imprime por clog la información de ejecución de los algoritmos.
// Imprime por cout el resultado de algun algoritmo ejecutado.
int main(int argc, char** argv)
{	
	cout << argc << endl;
	for(int i=0;i< argc;i++)
		cout<<argv[i]<<endl; 
	
	// Leemos el parametro que indica el algoritmo a ejecutar.
	map<string, string> algoritmos_implementados = {
		{"FB", "Fuerza Bruta"}, {"BT", "Backtracking con podas"}, {"BT-F", "Backtracking con poda por factibilidad"}, 
		{"BT-O", "Backtracking con poda por optimalidad"}, {"DP", "Programacion dinámica"}
	};

	// Verificar que el algoritmo pedido exista.
	if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end())
	{
		if(argc>=2)
			cerr << "Algoritmo no encontrado: " << argv[1] << endl;
			
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algoritmos_implementados) cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
		return 0;
	}
	string algoritmo = argv[1];

    // Leemos el input.
    cin >> n >> W;
    houses.assign(n,{});
	
    for (int i = 0; i < n; ++i){
		House house;
		cin >> house.beneficio >> house.contagio;

		houses[i] = house;
	}

    // Ejecutamos el algoritmo y obtenemos su tiempo de ejecución.
	int optimum;
	optimum = INF;
	auto start = chrono::steady_clock::now();

	if (algoritmo == "FB")
	{
		optimum = FB(0, 0, 0);
	}
	else if (algoritmo == "BT")
	{

	}
	else if (algoritmo == "DP")
	{

	}

	auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();

	// Imprimimos el tiempo de ejecución por stderr.
	clog << total_time << endl;

    // Imprimimos el resultado por stdout.
    cout << (optimum == INF ? -1 : optimum) << endl;
    return 0;
}