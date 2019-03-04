#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

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