#include "functions.h"

void agregarNodo(ListaEnlazadaLinea* platos, NodoLinea* plato){
	if (platos->largo == 0){
		platos->primero = plato;
		platos->ultimo = plato;
	} else {
		platos->ultimo->siguiente = plato;
		platos->ultimo = plato;
	}

	platos->largo++;
}

void addJump(ListaEnlazadaLinea* listaPlatos){
	char* token;
	token = strtok(listaPlatos->ultimo->ingredientes[listaPlatos->ultimo->cantidadIngredientes-1], "\n");
	if (token) strcat(listaPlatos->ultimo->ingredientes[listaPlatos->ultimo->cantidadIngredientes-1], "\n");
}

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

	agregarNodo(lineas, nodo);
	addJump(lineas);
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
		if (strcmp(buffer, "\n") != 0){
			NodoLinea* nodo;
			nodo = (NodoLinea*)calloc(1, sizeof(NodoLinea));
			agregarLinea(lineas, buffer);
		}
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

	// printf("\n");
}

void agregarPlato(ListaEnlazadaLinea* listaPlatos){
	NodoLinea* nuevoPlato;
	char* nombrePlato;
	int calorias, cantidadIngredientes;
	
	nombrePlato = (char*)calloc(64, sizeof(char));
	nuevoPlato = (NodoLinea*)calloc(1, sizeof(NodoLinea));
	nuevoPlato->comida = (char*)calloc(64, sizeof(char));

	printf("Nombre del nuevo plato: ");
	scanf("%[^\n]%*c", nombrePlato);
	strcpy(nuevoPlato->comida, nombrePlato);

	printf("Cantidad de calorías: ");
	scanf("%d", &calorias);
	nuevoPlato->calorias = calorias;

	printf("Cantidad de Ingredientes: ");
	scanf("%d", &cantidadIngredientes);
	getchar();
	nuevoPlato->cantidadIngredientes = cantidadIngredientes;

	nuevoPlato->ingredientes = (char**)calloc(cantidadIngredientes, sizeof(char*));
	for (int i=0; i<cantidadIngredientes; i++) {
		nuevoPlato->ingredientes[i] = (char*)calloc(64, sizeof(char));
		printf("Ingrese ingrediente número %d: ", i+1);
		scanf("%[^\n]%*c", strcat(nuevoPlato->ingredientes[i], "\n"));
	}

	agregarNodo(listaPlatos, nuevoPlato);
}

void ejecutar(){
	ListaEnlazadaLinea* archivo;
	
	archivo = leerArchivo("foodDatabase.txt");
	imprimirPlatos(archivo);
	agregarPlato(archivo);
	imprimirPlatos(archivo);
}



















