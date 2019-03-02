#include "functions.h"

void agregarLinea(ListaEnlazadaLinea* lineas, char* linea){
	NodoLinea* nodo;
	nodo = (NodoLinea*)calloc(1, sizeof(NodoLinea));

	char* token;
	int cantidadIngredientes, calorias;

	token = strtok(linea, "-");
	nodo->comida = (char*)calloc(64, sizeof(char));
	strcpy(nodo->comida, token);

	token = strtok(NULL, "-");
	calorias = atoi(token);
	nodo->calorias = calorias;

	token = strtok(NULL, "-");
	cantidadIngredientes = atoi(token);
	nodo->cantidadIngredientes = cantidadIngredientes;

	nodo->ingredientes = (char**)calloc(nodo->cantidadIngredientes, sizeof(char*));

	for (int i=0; i<cantidadIngredientes; i++) nodo->ingredientes[i] = (char*)calloc(64, sizeof(char));

	for (int i=0; i<cantidadIngredientes; i++){
		token = strtok(NULL, "-");
		strcpy(nodo->ingredientes[i], token);
	}

	if (lineas->largo == 0){
		lineas->primero = nodo;
		lineas->ultimo = nodo;
	} else {
		lineas->ultimo->siguiente = nodo;
		lineas->ultimo = nodo;
	}

	lineas->largo++;
}

ListaEnlazadaLinea* leerArchivo(char* nombreArchivo){
	FILE* f = fopen(nombreArchivo, "r");

	if (! f){
		printf("El archivo %s no existe.\nSaliendo...", nombreArchivo);
		exit(1);	
	} 

	ListaEnlazadaLinea* lineas;
	lineas = (ListaEnlazadaLinea*)calloc(1, sizeof(ListaEnlazadaLinea));

	char buffer[255];

	while (fgets(buffer, 255, (FILE*)f)){
		NodoLinea* nodo;
		nodo = (NodoLinea*)calloc(1, sizeof(NodoLinea));

		agregarLinea(lineas, buffer);
	}

	return lineas;
}

void imprimirPlatos(ListaEnlazadaLinea* listaPlatos){
	NodoLinea* aux;
	aux = listaPlatos->primero;

	while (aux){
		printf("Plato de comida: %s\n", aux->comida);
		printf("Calorías: %d\n", aux->calorias);
		printf("Ingredientes (%d): ", aux->cantidadIngredientes);

		for (int i=0; i<aux->cantidadIngredientes; i++) printf(" %s ", aux->ingredientes[i]);

		printf("\n");
		aux = aux->siguiente;
	}

}

void ejecutar(){
	ListaEnlazadaLinea* archivo;
	
	archivo = leerArchivo("foodDatabase.txt");
	imprimirPlatos(archivo);
}