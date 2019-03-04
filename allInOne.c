#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodoLinea{
	char* comida;
	int calorias;
	int cantidadIngredientes;
	char** ingredientes;
	struct nodoLinea* anterior;
	struct nodoLinea* siguiente;
}NodoLinea;

typedef struct listaEnlazadaLinea{
	NodoLinea* primero;
	NodoLinea* ultimo;
	int largo;
}ListaEnlazadaLinea;

int i;

void agregarNodo(ListaEnlazadaLinea* platos, NodoLinea* plato){
	if (platos->largo == 0){
		platos->primero = plato;
		platos->ultimo = plato;
	} else {
		platos->ultimo->siguiente = plato;
		platos->ultimo->siguiente->anterior = platos->ultimo;
		platos->ultimo = plato;
	}

	platos->largo++;
}

void limpiarPantalla(){
	#ifdef _WIN32
		system("cls");
	#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
		system("clear");
	#else
		#error "Sistema Operativo no soportado."
	#endif
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

	int i;
	for (i=0; i<cantidadIngredientes; i++)
		nodo->ingredientes[i] = (char*)calloc(64, sizeof(char));

	for (i=0; i<cantidadIngredientes; i++){
		token = strtok(NULL, "-");
		strcpy(nodo->ingredientes[i], token);
	}

	agregarNodo(lineas, nodo);
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

void imprimirPlatosOrden(ListaEnlazadaLinea* listaPlatos){
	NodoLinea* aux;
	aux = listaPlatos->primero;

	while (aux){
		char* token;
		token = strtok(aux->ingredientes[aux->cantidadIngredientes-1], "\n");
	
		if (token) 
			strcat(aux->ingredientes[aux->cantidadIngredientes-1], "\n");

		printf("Plato de comida: %s\n", aux->comida);
		printf("Calorías: %d\n", aux->calorias);
		printf("Ingredientes (%d): ", aux->cantidadIngredientes);

		for (i=0; i<aux->cantidadIngredientes; i++) 
			printf("- %s ", aux->ingredientes[i]);

		printf("\n");

		aux = aux->siguiente;
	}
}

void imprimirPlatosAlReves(ListaEnlazadaLinea* listaPlatos){
	NodoLinea* aux;
	aux = listaPlatos->ultimo;

	while (aux){
		char* token;
		token = strtok(aux->ingredientes[aux->cantidadIngredientes-1], "\n");
	
		if (token) 
			strcat(aux->ingredientes[aux->cantidadIngredientes-1], "\n");

		printf("Plato de comida: %s\n", aux->comida);
		printf("Calorías: %d\n", aux->calorias);
		printf("Ingredientes (%d): ", aux->cantidadIngredientes);

		for (i=0; i<aux->cantidadIngredientes; i++) 
			printf("- %s ", aux->ingredientes[i]);

		printf("\n");

		aux = aux->anterior;
	}
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
	for (i=0; i<cantidadIngredientes; i++) {
		nuevoPlato->ingredientes[i] = (char*)calloc(64, sizeof(char));
		printf("Ingrese ingrediente número %d: ", i+1);
		scanf("%[^\n]%*c", strcat(nuevoPlato->ingredientes[i], "\n"));
	}

	agregarNodo(listaPlatos, nuevoPlato);

	printf("\nEl plato se ha añadido exitosamente a los registros.\n\n");
}

NodoLinea* buscarPlato(ListaEnlazadaLinea* listaPlatos, char* platoBuscado){
	NodoLinea* aux;
	aux = listaPlatos->primero;

	while(aux){
		if (strcmp(aux->comida, platoBuscado) == 0)
			return aux;
		aux = aux->siguiente;
	}

	return NULL;
}

void eliminarPlato(ListaEnlazadaLinea* listaPlatos, char* nombreBuscado){
	NodoLinea* platoEliminar;
	platoEliminar = buscarPlato(listaPlatos, nombreBuscado);

	if (platoEliminar){
		platoEliminar->siguiente->anterior = platoEliminar->anterior;
		platoEliminar->anterior->siguiente = platoEliminar->siguiente;
		free(platoEliminar->comida);

		for (i=0; i<platoEliminar->cantidadIngredientes; i++) 
			free(platoEliminar->ingredientes[i]);

		free(platoEliminar->ingredientes);
		free(platoEliminar);

		printf("El plato %s ha sido eliminado exitosamente de los registros.\n", nombreBuscado);

	} else {
		printf("El plato %s no existe.\n", nombreBuscado);
	}
}

void editarPlato(ListaEnlazadaLinea* listaPlatos, char* nombreBuscado){
	NodoLinea* platoEditar;
	platoEditar = buscarPlato(listaPlatos, nombreBuscado);

	if (platoEditar){
		printf("Ingrese un nombre de plato (actual: %s): ", platoEditar->comida);
		scanf("%[^\n]%*c", platoEditar->comida);

		printf("Ingrese cantidad de calorías (actual: %d): ", platoEditar->calorias);
		scanf("%d", &platoEditar->calorias);

		printf("Ingrese cantidad de ingredientes (actual: %d): ", platoEditar->cantidadIngredientes);
		scanf("%d", &platoEditar->cantidadIngredientes);
		getchar();

		for (i=0; i<platoEditar->cantidadIngredientes; i++){
			printf("Ingrese ingrediente número %d: ", i+1);
			scanf("%[^\n]%*c", platoEditar->ingredientes[i]);
		}

		printf("\nEl plato %s ha sido modificado exitosamente de los registros.\n\n", nombreBuscado);

	} else {
		printf("El plato %s no existe.\n", nombreBuscado);
	}
}

void guardarCambios(ListaEnlazadaLinea* listaPlatos, char* nombreArchivo){
	FILE* f = fopen(nombreArchivo, "w");

	NodoLinea* aux;
	aux = listaPlatos->primero;

	while (aux){
		char* token;
		token = strtok(aux->ingredientes[aux->cantidadIngredientes-1], "\n");
	
		if (token) 
			strcat(aux->ingredientes[aux->cantidadIngredientes-1], "\n");

		fprintf(f, "%s-%d-%d", aux->comida, aux->calorias, aux->cantidadIngredientes);

		for (i=0; i<aux->cantidadIngredientes; i++) 
			fprintf(f, "-%s", aux->ingredientes[i]);

		aux = aux->siguiente;
	}

	fclose(f);
}

void mostrarPlato(NodoLinea* plato){
	char* token;
	token = strtok(plato->ingredientes[plato->cantidadIngredientes-1], "\n");

	if (token) 
		strcat(plato->ingredientes[plato->cantidadIngredientes-1], "\n");

	printf("\n");

	printf("Nombre del plato: %s\n", plato->comida);
	printf("Cantidad de calorías: %d\n", plato->calorias);
	printf("Ingredientes (%d): ", plato->cantidadIngredientes);

	for (i=0; i<plato->cantidadIngredientes; i++)
		printf("- %s ", plato->ingredientes[i]);

	printf("\n");
}

void liberarEspacio(NodoLinea* nodo){
	NodoLinea* aux;

	while(nodo){
		aux = nodo;
		free(aux->comida);

		for (i=0; i<aux->cantidadIngredientes; i++)
			free(aux->ingredientes[i]);

		free(aux->ingredientes);

		nodo = nodo->siguiente;
		free(aux);
	}
}

void buscarLimiteCalorias(ListaEnlazadaLinea* listaPlatos, int calorias){
	NodoLinea* aux;
	int encontrados;
	encontrados = 0;

	aux = listaPlatos->primero;

	while (aux){
		if (aux->calorias <= calorias){
			mostrarPlato(aux);
			encontrados++;
		}
		aux = aux->siguiente;
	}

	if (encontrados)
		printf("\nResultados encontrados: %d\n\n", encontrados);
	else
		printf("\nNo se han encontrado resultados para un limite de %d calorías.\n\n", calorias);
}

void ejecutar(){
	ListaEnlazadaLinea* archivo;
	int opcion;
	char* nombreArchivo;
	nombreArchivo = (char*)calloc(64, sizeof(char));
	opcion = -1;

	printf("¡Bienvenido al administrador del restaurant!\n\n");

	while (opcion != 9){

		printf("¿Qué desea?:\n");
		printf("(0) Cargar los platos mediante archivo.\n");
		printf("(1) Ver los platos de comida.\n");
		printf("(2) Agregar un plato de comida.\n");
		printf("(3) Buscar un plato de comida por nombre.\n");
		printf("(4) Editar un plato de comida.\n");
		printf("(5) Eliminar un plato de comida.\n");
		printf("(6) Buscar plato de comidas con límites de calorías.\n");
		printf("(7) Buscar plato de comidas con ingrediente específico.\n");
		printf("(9) Cerrar programa y guardar los datos.\n\n");
		printf("Seleccione una opción y presione [ENTER]: ");
		scanf("%d", &opcion);
		getchar();


		switch(opcion){
			case 0:
				limpiarPantalla();
				printf("Ingrese el nombre del archivo con los platos (debe incluir la extensión): ");
				scanf("%s", nombreArchivo);

				archivo = leerArchivo(nombreArchivo);

				if (archivo)
					printf("\nEl archivo %s fue cargado satisfactoriamente.\n\n", nombreArchivo);
				else
					printf("\nHubieron problemas para cargar los datos.\n\n");

				break;

			case 1:
				limpiarPantalla();
				if (archivo)
					imprimirPlatosOrden(archivo);
				else
					printf("\n¡Debe cargar un archivo antes de esta operación!\n\n");

				break;

			case 2:
				limpiarPantalla();
				if (archivo)
					agregarPlato(archivo);
				else
					printf("\n¡Debe cargar un archivo antes de esta operación!\n\n");

				break;

			case 3:
				limpiarPantalla();
				if (archivo){
					char* nombreBuscado;
					nombreBuscado = (char*)calloc(64, sizeof(char));

					printf("Ingrese el nombre del plato a buscar: ");
					scanf("%[^\n]%*c", nombreBuscado);

					NodoLinea* plato;
					plato = buscarPlato(archivo, nombreBuscado);
					free(nombreBuscado);

					if (plato)
						mostrarPlato(plato);

				} else {
					printf("\n¡Debe cargar un archivo antes de esta operación!\n\n");
				}
				break;

			case 4:
				limpiarPantalla();
				if (archivo){
					char* platoAEditar;
					platoAEditar = (char*)calloc(64, sizeof(char));

					printf("Ingrese el nombre del plato a editar: ");
					scanf("%[^\n]%*c", platoAEditar);

					editarPlato(archivo, platoAEditar);
					free(platoAEditar);

				} else {
					printf("\n¡Debe cargar un archivo antes de esta operación!\n\n");
				}

				break;

			case 5:
				limpiarPantalla();
				if (archivo){
					char* platoABorrar;
					platoABorrar = (char*)calloc(64, sizeof(char));

					printf("Ingrese el nombre del plato a eliminar: ");
					scanf("%[^\n]%*c", platoABorrar);

					editarPlato(archivo, platoABorrar);
					free(platoABorrar);

				} else {
					printf("\n¡Debe cargar un archivo antes de esta operación!\n\n");
				}

				break;

			case 6:
				limpiarPantalla();
				if (archivo){
					int calorias;

					printf("Ingrese el límite de calorías a buscar: ");
					scanf("%d", &calorias);
					getchar();

					buscarLimiteCalorias(archivo, calorias);
				} else {
					printf("\n¡Debe cargar un archivo antes de esta operación!\n\n");
				}

				break;

			case 7:
				limpiarPantalla();
				printf("\nAquí se buscarán platos de acuerdo a un ingrediente específico\n\n");
				break;

			case 9:
				limpiarPantalla();
				printf("¡Gracias por usar el administrador! Nos vemos pronto.\n");
				
				if(strlen(nombreArchivo) != 0){
					guardarCambios(archivo, nombreArchivo);

					if (archivo)
						liberarEspacio(archivo->primero);

					free(nombreArchivo);
				}

				break;

			default:
				limpiarPantalla();
				printf("[¡ERROR!] La opción ingresada no está disponible, vuelva a intentarlo\n\n");
				opcion = -1;
				break;
		}
	}
}

int main(){
	ejecutar();

	return 0;
}