//int mostrarNombreProveedor(int indice);

struct arrayDinamico {
	public:
	int tamano;
	int* listadoDinamico;

	arrayDinamico (){
		tamano=0;

	}
    void mostrarListado()
    {

        for (int i=0; i< tamano;i++)
		{
			std::cout<< /*mostrarNombreProveedor*/listadoDinamico[i]<< ",";
        }
    }


    void unirListado(arrayDinamico listadoTmp)
    {
        int _tamano=tamano+listadoTmp.tamano;
        int* listadoViejo=listadoDinamico;

        //antes de unir los 2 listados
        // evaluar del listado destino, los ahorros con listado origen
        //el mayor ahorro se coloca entre los listados destino, a la derecha del nodo origen que tuvo ese ahorro



        listadoDinamico=new int[_tamano];
        //listado 1
        for(int i=0;i<tamano;i++)
        {
            listadoDinamico[i]=listadoViejo[i];
        }

        //listado 2 Se encarga de unir dos listados
        for(int i=0;i<listadoTmp.tamano;i++)
        {
            listadoDinamico[tamano+i]=listadoTmp.listadoDinamico[i];
        }
        tamano =_tamano;


    }

    void AsignarCantidad(int CantidadTamano) {

	listadoDinamico = new int [CantidadTamano];
		tamano = CantidadTamano;
	}

    void AsignarValor (int posicion, int valor) {

    	listadoDinamico [posicion] = valor;
	}

};


