#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <set>
#include <map>

using namespace std;

//Info para experimentos
int total_nodes = 0;

const int INF = 10e6; // Valor para indicar que no hubo solución.
const int UNDEFINED = -1;
int beneficio_opt = -INF;
bool poda_optimalidad_1;
bool poda_optimalidad_2;
bool poda_factibilidad;
vector<int> beneficio_acum_1;	//en la posicióm i contiene la suma acumulada de beneficio en el intervalo [i,n] 
vector<int> beneficio_acum_2;

struct Negocio {
	int beneficio;
	int contagio;
};

// Información de la instancia a resolver.
int cant_negocios, contagio_limite;
vector<Negocio> negocios;
/*
i = posicion del comercio sobre el que queremos decidir
w = contagio acumulado en el intervalo [0,i)
k = beneficio acumulado en el intervalo [0,i)
*/
int FB(int i, int contagio, int beneficio){
	total_nodes++;

	if(i >= cant_negocios){
		if(contagio <= contagio_limite){
			return beneficio;
		}else{
			return -INF;
		}
	}

	int uso_negocio = FB(i + 2, contagio + negocios[i].contagio, beneficio + negocios[i].beneficio);
	int no_uso_negocio = FB(i + 1, contagio, beneficio);

	return max(uso_negocio, no_uso_negocio);
}

int BT(int i, int contagio, int beneficio){
	total_nodes++;

	if (i >= cant_negocios){
		if (contagio <= contagio_limite){
			beneficio_opt = max(beneficio, beneficio_opt);
			return beneficio;
		}
		else{
			return -INF;
		}
	}

	//poda por factibilidad 
	if(poda_factibilidad && contagio > contagio_limite){
		return -INF;
	}
	
	//poda por optimalidad
	if(poda_optimalidad_1 && (beneficio_opt-beneficio) > beneficio_acum_1[i]){
		return -INF;
	}

	int uso_negocio = BT(i + 2, contagio + negocios[i].contagio, beneficio + negocios[i].beneficio);
	int no_uso_negocio = BT(i + 1, contagio, beneficio);

	return max(uso_negocio,no_uso_negocio);
}

vector<vector<int>> cache;

int DP(int i, int contagio){
	total_nodes++;

	if(i >= cant_negocios){
		return 0;
	}

	if(cache[i][contagio] != UNDEFINED){
		return cache[i][contagio];
	}

	int uso_negocio= -INF;
	int no_uso_negocio= -INF;

	if(contagio >= negocios[i].contagio)
		uso_negocio = DP(i+2, contagio - negocios[i].contagio) + negocios[i].beneficio;

	no_uso_negocio = DP(i+1, contagio);

	cache[i][contagio] = max(uso_negocio, no_uso_negocio);

	return cache[i][contagio];
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
		{"FB", "Fuerza Bruta"}, 
		{"BT", "Backtracking con podas"},
		{"BT-F", "Backtracking con poda por factibilidad"},
		{"BT-O1", "Backtracking con poda por optimalidad 1"},
		{"BT-O2", "Backtracking con poda por optimalidad 2"},
		{"DP", "Programacion dinámica"}};

	// Verificar que el algoritmo pedido exista.
	if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end())
	{
		if(argc>=2)
			cerr << "Algoritmo no encontrado: " << argv[1] << endl;
			
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algoritmos_implementados) 
			cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
		return 0;
	}
	string algoritmo = argv[1];

    // Leemos el input.
    cin >> cant_negocios >> contagio_limite;
    negocios.assign(cant_negocios,{});
	
    for (int i = 0; i < cant_negocios; ++i){
		Negocio negocio;
		cin >> negocio.beneficio >> negocio.contagio;

		negocios[i] = negocio;
	}

	//inicializamos el vector de beneficio acumulado 1
	beneficio_acum_1.assign(cant_negocios,0);
	beneficio_acum_1[cant_negocios-1] = negocios[cant_negocios-1].beneficio;
	for(int i=cant_negocios-2; i>=0 ;i--)
		beneficio_acum_1[i] = beneficio_acum_1[i+1] + negocios[i].beneficio;
	
	//inicializamos el vector de beneficio acumulado 2
	if(cant_negocios%2==1){
		beneficio_acum_2.assign(cant_negocios + 1, 0);
		beneficio_acum_2[cant_negocios] = negocios[cant_negocios-1].beneficio;
		beneficio_acum_2[cant_negocios - 1] = negocios[cant_negocios - 1].beneficio;
	}
	else{
		beneficio_acum_2.assign(cant_negocios, 0);
		int max_beneficio = max(negocios[cant_negocios - 2].beneficio,negocios[cant_negocios - 1].beneficio);
		beneficio_acum_2[cant_negocios - 2] = max_beneficio;
		beneficio_acum_2[cant_negocios - 1] = max_beneficio;
	}
	 
	for (int i = beneficio_acum_2.size()-1-3; i >= 0; i-=2){
		int max_beneficio = max(beneficio_acum_2[i + 2], beneficio_acum_2[i + 3]);
		beneficio_acum_2[i] = max_beneficio;
		beneficio_acum_2[i+1] = max_beneficio;
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
		beneficio_opt = -INF;
		poda_optimalidad_1 = poda_optimalidad_2 = poda_factibilidad = true;
		optimum = BT(0, 0, 0);
	}
	else if (algoritmo == "BT-F")
	{
		beneficio_opt = -INF;
		poda_optimalidad_1 = false;
		poda_optimalidad_2 = false;
		poda_factibilidad = true;
		optimum = BT(0, 0, 0);
	}
	else if (algoritmo == "BT-O1")
	{
		beneficio_opt = -INF;
		poda_optimalidad_1 = true;
		poda_optimalidad_2 = false;
		poda_factibilidad = false;
		optimum = BT(0, 0, 0);
	}
	else if (algoritmo == "BT-O2")
	{
		beneficio_opt = -INF;
		poda_optimalidad_1 = false;
		poda_optimalidad_2 = true;
		poda_factibilidad = false;
		optimum = BT(0, 0, 0);
	}
	else if (algoritmo == "DP")
	{
		cache.assign(cant_negocios, vector<int>(contagio_limite+1, UNDEFINED));
		optimum = DP(0, contagio_limite);
	}

	auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();

	// Imprimimos el tiempo de ejecución por stderr.
	clog << total_time << endl;
	clog << total_nodes << endl;
    // Imprimimos el resultado por stdout.
    cout << (optimum == INF ? -1 : optimum) << endl;
    return 0;
}