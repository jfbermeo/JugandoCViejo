///Librerias
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include "arrayDinamico.h"
#include "arrayMemoriaParcial.h"



///Defino caracteristicas (constantes)
//defino los clientes
#define n 11 // numero de nodos en el sistema
#define m 10 // numero de clientes
#define maxnodos 4
#define numIter 3
#define IterTabuList 3 //tamaño de la lista tabu
#define DemaMin 10 // defino la demanda maxima

//Factores de emision
#define FacEmi1 0.7232
#define FacEmi2 0.8158
#define FacEmi3 0.9083
#define FacEmi4 1.0009
#define FacEmi5 1.0841


	///declaracion Variables globales

	///Defino la matriz ahorro
	double ahorro [n][n];

///llenar la lista
unsigned long lista [m*(m-1)][3];

///defino la matriz de distancias
int distancia [n][n];

///Codigo del proveedor
int codigo [n];

///demanda
unsigned long demanda [n];

//////volumen
unsigned long volumen [n];

///*****Datos de entrada******///////////
///capacidad Vehiculo
unsigned long cv=24000;

/////Capacidad en m3 (volumen)
unsigned long vv=100;

///Se crea un tipo de dato Memoria Parcial
arrayMemoriaParcial listadoNuevo;


using namespace std;

///Funcion para seleccionar aleaoriamente las rutas
int random(int min,int max)
{
	Sleep(400);
	srand(time(NULL));
	return  (rand ()%max)+ min;
}

///Funcion para calcular el ahorro, Solucion inicial ALgoritmo de C&W
void CalcularAhorro()


{
	cout << "Los ahorros son:" " " "\n"<<endl;
	for (int i=0;i<=m ;i++)
	{
		for  (int j=0;j<=m ;j++)
		{
			ahorro [i][j]= /*distancia [0][i]+*/distancia [0][i]- distancia [i][j];
			//)ahorro [i][j]= distancia.ObtenerDistancia(0,j)- distancia.ObtenerDistancia(i,j);
			if  (ahorro [i][j] < 0 || i==j)
			{
				ahorro [i][j] =0;
			}
			cout<<"\t"<< ahorro [i][j];

		}
		cout<<endl;
	}
	cout<<"\n";
}

///Funcion para llenar la lista de ahorros
void LlenarLista()
{

	int poslist=0;

	for(int i=1;i<=m;i++)
	{
		for  (int j=1;j<=m;j++)
		{
			if (i!=j)
			{
				lista [poslist] [0] = ahorro [i][j];///ahorro
				lista [poslist] [1] = i;///Origen
				lista [poslist] [2] = j;///Destino
				poslist++;
			}
		}
	}
}

///Ordenamiento burbuja de la lista ahorros de mayor a menor
void OrdenamientoMetodoBurbuja()
{
	int temp[3];
	for(int poslist=1;poslist<=(m*(m-1));poslist++)
	{
		for (int list=0;list<(m*(m-1))-poslist;list++)

			if (lista [list][0] < lista[list+1][0])
		{
			temp [0] = lista [list][0];///0=ahorro
			temp [1] = lista [list][1];///1=origen
			temp [2] = lista [list][2];///2=destino
			lista [list][0] = lista[list+1][0];
			lista [list][1] = lista[list+1][1];
			lista [list][2] = lista[list+1][2];
			lista [list+1][0] = temp [0];
			lista [list+1][1] = temp [1];
			lista [list+1][2] = temp [2];
		}
	}
}

///Se muestra la lista ordenada
void MostrarListaOrdenada()
{
	cout<<"El ahorro ordenado de mayor a menor es el siguiente: "  " ";

	cout<<"\n";
	cout<<"a"<<" "<<"i"<<" "<<"j"<<endl;
	for(int poslist=0;poslist<(m*(m-1));poslist++){
		cout<<lista [poslist][0]<<" "<< lista [poslist][1]<<" "<<lista [poslist][2]<<endl;
	}
}

///Se define la matriz memo parcial, con los datos del problema
void Memo_Parcial ()
{
	int dis_ini;

	for (int i=1;i<=m;i++)
	{

		dis_ini=/*distancia[0][i] +*/ distancia[0][i];
		//is_ini= distancia.ObtenerDistancia(0,i) //distancia[0][i];
		MemoriaParcial obj;

		obj.Num_Ruta = i;
		obj.CodigoProve = codigo [i];
		obj.carga = demanda [i]; // si este vehiculo, no tiene la capacidad de cargar toda la demanda carga = lo que alcance??
		obj.espacio = volumen [i];
		obj.distancia=dis_ini;
		obj.Num_Clientes=1;
		obj.ex1=i;
		obj.ex2=i;
		obj.secuencia.AsignarCantidad(1);
		obj.secuencia.AsignarValor(0,i);

		listadoNuevo.agregarNuevo(obj);


	}


	return ;


}
///Se muestra la matriz memo parcial
void Mostrar_Memo_Parcial ()
{
	cout<<"La Matriz de memoria parcial: "  " ";

	cout<<"\n";

		cout<< "-\tcarga\tvol\tdist\tclien\tsec" <<endl;

	for (int i=0;i<listadoNuevo.tamano;i++)
	{

		listadoNuevo.imprimirObjeto(i);


		cout<<endl;
		cout<<"\n";
	}


	int sumaDistanciaOriginal=listadoNuevo.CalcularDistancia(false);// calculo las distancia, de como esta, antes de cambiarla
	cout<<"distancia Original"<<" "<<sumaDistanciaOriginal<<"\n"<<endl;
	// cout<<"Las emisiones son:"<<"  "<<sumaDistanciaOriginal * 0.90<<"  "<<"KGCO2"<<endl;


}


int calcularDistancia(int x,int y)
{
	return distancia[x][y];
}


//Es la funcion mas importante, se define el cuerpo del codigo
void ActualizarListado(int  nNodoOrigen,int nNodoDestino,int ahorro)
{
	if(nNodoOrigen==nNodoDestino)
	{
		return;
	}

	int pos= 1;

	for(int i=0;i<listadoNuevo.tamano;i++)
	{
		//cout<<"i="<<i<<endl;
		MemoriaParcial objTmp=listadoNuevo.obtenerRuta(i);

		//cout<<"numRuta"<<objTmp.Num_Ruta<<endl;

		if(objTmp.Num_Ruta!=nNodoOrigen && objTmp.Num_Ruta!=nNodoDestino)
		{
			//listadoTmp[pos]=objTmp;
			pos++;
			//cout<<"no se va a unir esta ruta"<<endl;
		}
		else
		{
			if(objTmp.Num_Ruta==nNodoOrigen)
			{
				//cout <<"Voy a Buscar nodo Destino"<<endl;l
				//Buscar  NodoFrom
				for(int j=1;j<listadoNuevo.tamano;j++)
				{
					MemoriaParcial objTmpDestino=listadoNuevo.obtenerRuta(j);
					if(objTmpDestino.Num_Ruta==nNodoDestino)
					{
						//	cout <<"Encontre Nodo Destino"<<endl;

						//cout<<"imprimir"<<objTmpDestino.Num_Ruta<<" "<<objTmpDestino.carga<<" "<<objTmpDestino.distancia;


						//Mostrar_Memo_Parcial();
						//cout<<"---------Origen" <<nNodoOrigen<<" destino"<<nNodoDestino;

						if(objTmp.Num_Ruta!=objTmpDestino.Num_Ruta && // Origen destinos sean DISTINTOS
						   objTmp.carga + objTmpDestino.carga<=cv  && // la suma de las CARGAS no superen la capacidad del vehiculo
						   objTmp.espacio + objTmpDestino.espacio<=vv &&  // la suma del VOLUMEN no superen la capacidad del vehiculo
						   objTmp.Num_Clientes+objTmpDestino.Num_Clientes <= maxnodos )//&& // los numeros de cliente no debe superar el NUMERO MAXIMO DE NODOS
                           // objTmp.distancia + objTmpDestino.distancia <= distancia [0][i] +3000 )  //Numero maximo de kilometros a desviarse
						{

							if (objTmp.carga <=DemaMin || objTmpDestino.carga <=DemaMin ) // Si la carga es menor a  demanda minima (DemMin) no se une el nodo
							{
                                  if (objTmp.carga <=DemaMin){
                                  cout<<"La carga del nodo"<<"  "<<objTmp.Num_Ruta<<"  "<<" va por el centro de consolidacion "<< " " <<endl;
                                  }

                                    if(objTmpDestino.carga <=DemaMin){
                                     cout<<"La carga del nodo"<<"  "<<objTmpDestino.Num_Ruta<<"  "<<" va por el centro de consolidacion "<< " " <<endl;
                                    }
							}

							else
							{

                                {
                                     listadoNuevo.unirRutas(objTmp,objTmpDestino,ahorro);
                                    listadoNuevo.eliminar(objTmpDestino.Num_Ruta);
                                }


							}

						}

						pos++;
					}

				}
			}

		}


	}


}



///Se definen restricciones del codigo
void Solucion()
{
	for(int i=0;i<(m*(m-1));i++)
	{
		int origen=lista[i][1];///origen
		int destino=lista[i][2];///destino
		int ahorro=lista[i][0];///ahorro

		if(ahorro<=0)
		{
			return;//salir
		}


		MemoriaParcial nodoOrigen=listadoNuevo.cargarMemoParcial(origen);//cargo solo extremos
		MemoriaParcial nodoDestino=listadoNuevo.cargarMemoParcial(destino);//cargo solo extremos


		if(nodoOrigen.Num_Ruta==0  ||    nodoDestino.Num_Ruta==0 )
		{
			//  cout<<"no voy a unir  "<<origen<< "con "<<destino<<" por que uno de los 2, no es extremo"<<endl;
			continue;
		}

		if(nodoDestino.carga + nodoOrigen.carga<=cv && nodoDestino.espacio + nodoOrigen.espacio<=vv && nodoOrigen.Num_Ruta!=nodoDestino.Num_Ruta)
		{
			if
				((destino==nodoDestino.ex1 || destino==nodoDestino.ex2)
				 &&
				 (origen==nodoOrigen.ex1 || origen==nodoOrigen.ex2))


			{
				////////ANALIZAR BIEN ESTA CONDICION////////////
				if (nodoOrigen.Num_Clientes+nodoDestino.Num_Clientes <= maxnodos) {
					ActualizarListado(nodoOrigen.Num_Ruta,nodoDestino.Num_Ruta,ahorro);
				}
			}
		}
	}

}


/// Intensificacion - Ordenamiento de la secuencia en la solucion  Metodo burbuja

void OrdenamientoDeSecuencia()
{

	for (int i=0;i<listadoNuevo.tamano;i++)
	{
		int temp;

		for(int var1=1;var1<=listadoNuevo.listado[i].secuencia.tamano;var1++)
		{
			for (int var2=0;var2<listadoNuevo.listado[i].secuencia.tamano-var1;var2++)
			{
				if(distancia[0][listadoNuevo.listado[i].secuencia.listadoDinamico[var2]]<distancia[0][listadoNuevo.listado[i].secuencia.listadoDinamico[var2+1]])
				{
					temp=listadoNuevo.listado[i].secuencia.listadoDinamico[var2];
					listadoNuevo.listado[i].secuencia.listadoDinamico[var2] = listadoNuevo.listado[i].secuencia.listadoDinamico[var2+1];
					listadoNuevo.listado[i].secuencia.listadoDinamico[var2+1] = temp;
				}
			}
		}

	}
}

///Se corre antes de lambdaInterchange shift (1,0)
void Lambda()
{

	MemoriaParcial ruta1;
	MemoriaParcial ruta2;

	int nruta;

	nruta=listadoNuevo.tamano;
	///cuantas rutas tienen menos de 4 nodo

	int numRutaEficaces=0;
	for(int i=0;i<nruta;i++)
	{
		MemoriaParcial masNodos=listadoNuevo.obtenerRuta(i);

		if(masNodos.Num_Clientes <4)
		{
			numRutaEficaces++;
		}
	}
	cout<<"Las rutas eficaces con menos de cuatro nodos son:"<<" "<<numRutaEficaces<<"\n"<<endl;

	if(numRutaEficaces<=1)
	{
		cout<<"No cumple para esta funcion"<<"\n"<<endl;
		cout<<numRutaEficaces<<endl;
		return;
	}
	///Cuantas rutas tienen un nodo.
	int numRutaEficaces1=0;
	for(int i=0;i<nruta;i++)
	{
		MemoriaParcial masNodos=listadoNuevo.obtenerRuta(i);

		if(masNodos.Num_Clientes ==1)
		{
			numRutaEficaces1++;
		}
	}
	cout<<"Las rutas eficaces con un nodo son :"<<" "<<numRutaEficaces1<<"\n"<<endl;

	if(numRutaEficaces1==0)
	{
		cout<<"No cumple para esta funcion"<<"\n"<<endl;
		cout<<numRutaEficaces1<<"\n"<<endl;
		return;
	}

	int iRuta1,iRuta2;// indice ruta 1, indice ruta2


	for (int i=1;i<numIter;i++)
	{
		///Eligiendo la ruta1
		do{
			iRuta1=random(0,nruta);
			ruta1=listadoNuevo.obtenerRuta(iRuta1);

		}
		while (ruta1.Num_Clientes<=1 || ruta1.Num_Clientes>=maxnodos);

		//eligiendo la ruta2
		do{

			iRuta2= random(0,nruta);
			ruta2=listadoNuevo.obtenerRuta(iRuta2);

		}
		while (ruta2.Num_Clientes!= 1 || ruta1.Num_Ruta==ruta2.Num_Ruta);

		//cout<<"Uno " << ruta1.Num_Ruta << " tiene " << ruta1.Num_Clientes<< " Con " << ruta2.Num_Ruta << " tiene " << ruta2.Num_Clientes<<endl;

		ActualizarListado(ruta1.Num_Ruta,ruta2.Num_Ruta,0);





		//ruta1.secuencia.unirListado(ruta2.secuencia);
	}
	int sumaDistanciaOriginal=listadoNuevo.CalcularDistancia(false);// calculo las distancia, de como esta, antes de cambiarla
	cout<<"distanciaOriginal"<<"\n"<<endl;
	cout<<sumaDistanciaOriginal<<"\n"<<endl;
}


///Se aplica la mejora  - Intercambio de Lambda
void lambdaInterchange ()
{

	MemoriaParcial ruta1;
	MemoriaParcial ruta2;

	int nruta;

	nruta=listadoNuevo.tamano;

	int listaTabu[nruta][n];

	for(int i=0;i<nruta;i++)
	{
		listaTabu[i][0]=0;
		listaTabu[i][1]=0;
	}

	///cuantas rutas tienen mas de 1 nodos

	int numRutaEficaces=0;
	for(int i=0;i<nruta;i++)
	{
		MemoriaParcial masNodos=listadoNuevo.obtenerRuta(i);

		if(masNodos.Num_Clientes >1)
		{
			numRutaEficaces++;

		}
	}
	cout<<"Las rutas eficaces son:"<<" "<<numRutaEficaces<<endl;

	if(numRutaEficaces<=1)
	{
		cout<<"No cumple para esta funcion"<<endl;
		cout<<numRutaEficaces<<endl;
		return;
	}

	int iRuta1,iRuta2;// indice ruta 1, indice ruta2


	for (int i=1;i<numIter;i++)
	{
		///Eligiendo la ruta1
		do{

			//Resto numero de iteracciones a todos los nodos
			for(int i=0;i<nruta;i++)
			{
				if(listaTabu[i][1]>0)
				{
					cout<<"LT"<<listaTabu[i][0]<<" "<<listaTabu[i][1]-1<<endl;
					listaTabu[i][1]=listaTabu[i][1]-1;
				}
			}

			iRuta1=random(0,nruta);
			ruta1=listadoNuevo.obtenerRuta(iRuta1);

			//***  cout<<"Seleccion indice R1 "<<" "<<""<< iRuta1<<endl;
		}
		while (ruta1.Num_Clientes<2 || listaTabu[iRuta1][1] );
		cout<<"R1"<<" "<<ruta1.Num_Ruta<<endl;
		listaTabu[iRuta1][0]=ruta1.Num_Ruta;
		listaTabu[iRuta1][1]=4;

		///eligiendo la ruta2

		do{

			//Resto numero de iteracciones a todos los nodos

			for(int i=0;i<nruta;i++)
			{
				if(listaTabu[i][1]>0)
				{
					cout<<"LT "<<listaTabu[i][0]<<" "<<listaTabu[i][1]-1<<endl;
					listaTabu[i][1]=listaTabu[i][1]-1;
				}
			}

			iRuta2= random(0,nruta);
			ruta2=listadoNuevo.obtenerRuta(iRuta2);
			//***		cout<<"Seleccion indice R2"<<" "<< iRuta2<<endl;
		}
		while (ruta2.Num_Clientes<2 || ruta1.Num_Ruta==ruta2.Num_Ruta || listaTabu[iRuta2][1]>0);
		listaTabu[iRuta2][0]=ruta2.Num_Ruta;
		listaTabu[iRuta2][1]=4;
		//////////////////Aqui comienza a validar el cambio de los nodos//// Pero solo verifica la distancia y es necesario que verifique la capacidad del camion y el volumen
		//cambio (1,0)
		//ruta1.unirListado(ruta2.secuencia);

		cout<<"R2"<<" "<<ruta2.Num_Ruta<<endl;

		// punto 2

		int nodoR1=random(0,ruta1.Num_Clientes);
		int nodoR2 =random(0,ruta2.Num_Clientes);


		int sumaDistanciaOriginal=listadoNuevo.CalcularDistancia(false);// calculo las distancia, de como esta, antes de cambiarla

		cout<<"distancia Original"<<"  "<<sumaDistanciaOriginal<<endl;

		cout<<"intercambia"<<"  "<<ruta1.Num_Ruta<<"-"<<nodoR1<<" -"<<ruta2.Num_Ruta<<"-"<<nodoR2<<endl;

		int rutaTmp=ruta1.secuencia.listadoDinamico[nodoR1];
		listadoNuevo.listado[iRuta1].secuencia.listadoDinamico[nodoR1]=ruta2.secuencia.listadoDinamico[nodoR2];
		listadoNuevo.listado[iRuta2].secuencia.listadoDinamico[nodoR2]=rutaTmp;

		//punto3

		int sumaDistanciaDespuesDelCambio=listadoNuevo.CalcularDistancia(true);//ahora sumo, las distancias luego de intercambiar los nodos

		cout<<"distancia Nueva"<<"  "<<sumaDistanciaDespuesDelCambio<<endl;

		if(sumaDistanciaDespuesDelCambio>sumaDistanciaOriginal|| ruta1.carga >cv || ruta2.carga>cv)//si la sumatoria de la distancia nueva es mayor a la original,ó si la carga de las rutas es mayor a ala capicidad del vehiculo vuelvo a dejar los nodos como estaban, por que el cambio no es eficaz
		{
			rutaTmp=ruta1.secuencia.listadoDinamico[nodoR1];
			listadoNuevo.listado[iRuta1].secuencia.listadoDinamico[nodoR1]=ruta2.secuencia.listadoDinamico[nodoR2];
			listadoNuevo.listado[iRuta2].secuencia.listadoDinamico[nodoR2]=rutaTmp;
			cout<<"No Cambia"<<endl;

			listadoNuevo.CalcularDistancia(true);
		}
		else// si hay un ahorro
		{
			listadoNuevo.ActualizarDistancia();//Actualizamos el valor de la distancia
		}
	}
	cout<<"No se aplica Criterio de aspiracion"<<" "<<"\n"<<endl;
}


//Calculo de emisiones de CO2

void contaminacion () {

	//Variable para almacenar el resultado
	float x=0;


	for(int i=0;i<listadoNuevo.tamano;i++)
	{
		if ( listadoNuevo.listado[i].carga  >= 0 && listadoNuevo.listado[i].carga <=5000){
			x= x + listadoNuevo.listado [i].distancia * FacEmi1;
		}
		else {
			if ( listadoNuevo.listado[i].carga  >=5001 && listadoNuevo.listado[i].carga <=10000){
				x= x + listadoNuevo.listado [i].distancia * FacEmi2;
			}

			else    {
				if  ( listadoNuevo.listado[i].carga  >=10001 && listadoNuevo.listado[i].carga <=15000){
					x= x+ listadoNuevo.listado [i].distancia * FacEmi3;
				}
				else {
					if( listadoNuevo.listado[i].carga  >=15001 && listadoNuevo.listado[i].carga <=20000){
						x= x+ listadoNuevo.listado [i].distancia * FacEmi4;
					}
					else {
						if (listadoNuevo.listado[i].carga >=20001 && listadoNuevo.listado[i].carga<=24000) {
							x= x+ listadoNuevo.listado [i].distancia * FacEmi5;}
					}
				}
			}

		}



	}
	cout<<"Las emisiones son"<<" "<<x<<endl;
}
/*
int mostrarNombreProveedor(int indice)

{
return codigo[indice] ;
}

*/



int main() {

	///Defino las caracterisiticas de mi problema
	int i;
	int j;

	///Leer la demanda.txt
	i=0;
	ifstream adem ("demanda1.txt");
	while  (!adem.eof()&& i<=m) {
		adem >>demanda [i];
		//cout<<nodo [i];
		i++;
	}
	adem.close ();



	//Leer el volumen de la demanda .txt
	i=0;
	ifstream avol ("V1.txt");
	while  (!avol.eof()&& i<=m) {
		avol >>volumen [i];
		//cout<<nodo [i];
		i++;
	}
	avol.close ();

	///Leer la distancia
	i=0, j=0;
	ifstream adis ("kilometros.txt");
	while  (!adis.eof()&& i<=m && j<=m) {
		adis >>distancia [i][j];
		i++;
		//Recorrido de la matriz
		if (i==n){
			i=0; j++;
		}
	}
	adis.close ();

	///Leer el codigo del proveedor.txt
	i=0;
	ifstream acod ("C1.txt");
	while  (!acod.eof()&& i<=m) {
		acod >>codigo [i];
		//cout<<"Los proveedores son"<<codigo[i]<<endl;
		i++;


	}
	acod.close ();

	/// Llamado de funciones

	CalcularAhorro();
	LlenarLista();
	OrdenamientoMetodoBurbuja();
	MostrarListaOrdenada();
	Memo_Parcial ();
	Mostrar_Memo_Parcial ();
	Solucion();
	Mostrar_Memo_Parcial ();
	Lambda();
	Mostrar_Memo_Parcial ();
	lambdaInterchange ();
	Mostrar_Memo_Parcial ();
	OrdenamientoDeSecuencia ();
	Mostrar_Memo_Parcial ();
	contaminacion();

	return 0;

}
