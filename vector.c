#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void){
	printf("Creando vector...\n");
	vector v;
	VectorNew(&v, sizeof(int), NULL, 8);
	printf("Vector creado!\n");
	printf("Size: %d\n", VectorLength(&v));
	int i = 1;
	VectorAppend(&v,&i);
	printf("Size: %d\n", VectorLength(&v));
	VectorInsert(&v, &i, 2);
	printf("%p\n", VectorNth(&v, 0));
	printf("%p\n", VectorNth(&v, 1));
	return 0;
}

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
	v->pos = 0;	//longitud inicial del vector
	v->memSize = initialAllocation;	//Especifica el tamaño en memoria a asignar al crear el vector
	v->reSize = initialAllocation;	//Indica en cuanto va a aumentar la memoria cuando el vector se llene de elementos
	v->elemSize = elemSize;	//Tamaño del tipo de dato a manejar
	v->freeFn = freeFn;	//Función para liberar de memoria los elementos del vector de forma individual
	v->elems = malloc(v->memSize*elemSize);	//Crea el espacio de memoria asignado para el vector
	assert(v->elems != NULL);	//Termina si no se asigno memoria
}

void VectorDispose(vector *v)//Borra los elementos del vector, se hace free con la función freeFn
{}

int VectorLength(const vector *v)
{
	return v->pos;
}

void *VectorNth(const vector *v, int position)//Regresa la posición de memoria del elemento
{ 
	if(position >= 0 && position <= v->pos){
		printf("position %d\n", position);
		return v->elems + (v->elemSize*position);
	}
	return NULL;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{}

void VectorInsert(vector *v, const void *elemAddr, int position)//Se utiliza memmove
{
	if(position >= 0 && position <= v->pos){
		printf("Se inserto en la posición %d\n", position);
	}
	//if( pos > 0 && position < pos)
		//...
	//data apuntador (dentro de la estructura) al vector
	//*elemInsert = (char *) data + position*elemSize;
	//*elemNext = (char * ) data + (position+1)*elemSize;
	//memmove(elemNext, elemInsert ...);
}

void VectorAppend(vector *v, const void *elemAddr)
{
	if(v->pos == v->memSize){
		v->memSize += v->reSize;
		v->elems = realloc(v->elems,v->memSize*v->elemSize);
	}
	elemAddr =  (char *)v->elems + v->pos * v->elemSize;
	memcpy(elemAddr,&elemAddr,v->elemSize);
	v->pos++;
}

void VectorDelete(vector *v, int position)
{}

void VectorSort(vector *v, VectorCompareFunction compare)
{}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)//auxData se usa como segundo argumento de la funcipn mapFn
{}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)//lfind para busqueda lineal
{ return -1; } 
