#include <iostream>

int calcularDistancia(int x,int y);
int calcularContaminacion(int x,int y);

struct MemoriaParcial {
public:
    int Num_Ruta;
    int CodigoProve;
    int distancia;
    int contaminacion;
    unsigned long carga;
    unsigned long espacio;
    int Num_Clientes;
    int ex1;
    int ex2;
    arrayDinamico secuencia;


    int calcularDistanciaSecuencia()
    {

        int suma = 0;
        //int suma = calcularDistancia(0,secuencia.listadoDinamico[0]); // es la suma inicial que hay entre el 0, y el primer numero de la secuencia
        //std::cout<<"distancia Inicial " << secuencia.listadoDinamico[0] << " " << suma<< "\n";

        for(int i=0;i<Num_Clientes-1;i++)
        {
            int n1,n2;
            n1=secuencia.listadoDinamico[i];//1
            n2=secuencia.listadoDinamico[i+1];//3

            //std::cout<<"suma"<<n1<<" " <<n2<< " " << calcularDistancia(n1,n2);
            suma = suma + calcularDistancia(n1,n2);
        }
        suma+= calcularDistancia(secuencia.listadoDinamico[Num_Clientes-1],0)//se suma la distancia del ultimo nodo del listado, al nodo 0

        return suma;
    }


int calcularDistanciaContaminacion()
    {

        int suma = calcularContaminacion(0,secuencia.listadoDinamico[Num_Clientes-1]); // es la suma que hay entre el 0, y el ultimo numero de la secuencia
        //std::cout<<"distancia Inicial " << secuencia.listadoDinamico[0] << " " << suma<< "\n";

        for(int i=Num_Clientes-1;i>0;i--)
        {
            int n1,n2;
            n1=secuencia.listadoDinamico[i];
            n2=secuencia.listadoDinamico[i-1];

            //std::cout<<"suma"<<n1<<" " <<n2<< " " << calcularDistancia(n1,n2);
            suma = suma + calcularContaminacion(n1,n2);
        }

        return suma;
    }
};



struct arrayMemoriaParcial {
public:
    int tamano;
    MemoriaParcial *listado;

    //Agregar ruta al listado, aumenta el tamaño en 1, viejo->nuevo

    void agregarNuevo (MemoriaParcial objTmp){

        int _tamano=tamano+1;
        MemoriaParcial* listadoViejo= listado;

        listado=new MemoriaParcial[_tamano];
        //listado 1
        for(int i=0;i<tamano;i++)
        {
            listado[i]=listadoViejo[i];
        }
        listado[tamano]=objTmp;
        tamano =_tamano;


    }


    void eliminar(int NumeroRuta)    {

        int _tamano=tamano-1;
        MemoriaParcial* listadoViejo=listado;


        listado=new MemoriaParcial    [_tamano];
        //listado 1
        int Pos=0;
        for(int i=0;i<tamano;i++)
        {
            if( listadoViejo[i].Num_Ruta!=NumeroRuta)
            {
                listado[Pos]=listadoViejo[i];
                Pos++;
            }
        }

        tamano =_tamano;
    }


    void unirRutas(MemoriaParcial nodoOrigen, MemoriaParcial nodoDestino, int ahorros){

        for(int i=0;i<tamano;i++)
        {
            if(listado[i].Num_Ruta==nodoOrigen.Num_Ruta)
            {
                listado[i].carga=nodoDestino.carga+nodoOrigen.carga;
                listado[i].CodigoProve==nodoOrigen.CodigoProve;
                listado[i].espacio=nodoDestino.espacio+nodoOrigen.espacio;
                listado[i].distancia=nodoOrigen.distancia+nodoDestino.distancia-ahorros;
                listado[i].Num_Clientes=nodoDestino.Num_Clientes+nodoOrigen.Num_Clientes;
                listado[i].ex2=nodoDestino.ex1;
                listado[i].secuencia.unirListado(nodoDestino.secuencia);
                return;
            }
        }
    }

    void imprimirObjeto (int posicion)    {

    //    std::cout<<listado[posicion].Num_Ruta;
    //    std::cout<<"\t"<<listado[posicion].CodigoProve;
        std::cout<<"\t" <<listado[posicion].carga;
        std::cout<<"\t" <<listado[posicion].espacio;
        std::cout<< "\t" <<listado[posicion].distancia;
        std::cout<< "\t" <<listado[posicion].Num_Clientes;
        //std::cout<< "\t" <<listado[posicion].ex1;
        //std::cout<< "\t" <<listado[posicion].ex2;
        std::cout<< "\t";
        listado[posicion].secuencia.mostrarListado();
    }


    int CalcularDistancia(bool noRecalcula)
    {
        int suma = 0;
        for(int i=0;i<tamano;i++)
        {
            MemoriaParcial obj = listado[i];
            if(noRecalcula==true)
            {
                suma = suma + obj.calcularDistanciaSecuencia();
                std::cout<<"\t" <<"AAAAAAA"<< " "<< obj.calcularDistanciaSecuencia();
            }else

            {
                suma = suma + obj.distancia;
                 std::cout<<"\t" <<"AAAAAAA"<< " "<< obj.calcularDistanciaSecuencia();
            }
        }

        return suma;
    }


int calcularContaminacion()
    {
        int suma = 0;
        for(int i=0;i<tamano;i++)

        {
            MemoriaParcial obj = listado[i];


                suma = suma + obj.calcularDistanciaContaminacion();
                 std::cout<<"\t" <<"AAAAAAA  "<< "  "<< obj.calcularDistanciaContaminacion();


        }

        return suma;
    }


    void ActualizarDistancia()
    {
        for(int i=0;i<tamano;i++)
        {
            //MemoriaParcial obj = listado[x];
            //obj.distancia= obj.calcularDistanciaSecuencia();
            listado[i].distancia= listado[i].calcularDistanciaSecuencia();

        }
    }

    void ActualizarContaminacion()
    {
        for(int i=0;i<tamano;i++)
        {
            //MemoriaParcial obj = listado[x];
            //obj.distancia= obj.calcularDistanciaSecuencia();
            listado[i].distancia= listado[i].calcularDistanciaSecuencia();

        }
    }





    MemoriaParcial  cargarMemoParcial(int nNodo)
    {
        for(int i=0;i<tamano;i++)
        {
            if(obtenerRuta(i).ex1==nNodo || obtenerRuta(i).ex2==nNodo )
            {
                return obtenerRuta(i);
            }
        }
        MemoriaParcial t;
        t.Num_Ruta=0;
        t.CodigoProve=0;
        t.carga=0;
        t.espacio=0;

        return t;
    }

    MemoriaParcial obtenerRuta(int posicion){

        return listado[posicion];
    }

    arrayMemoriaParcial (){

        tamano=0;

    }

};
