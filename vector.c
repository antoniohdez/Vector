#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void){
	printf("Creando vector...\n");
	vector v;
	VectorNew(&v, sizeof(int), NULL, 10);
	printf("Vector creado!\n");
	int i = 0;
	VectorAppend(&v, &i);
	i++;
	VectorAppend(&v, &i);
	i++;
	VectorAppend(&v, &i);
	i++;
	VectorAppend(&v, &i);
	i++;
	VectorAppend(&v, &i);
	i++;
	VectorAppend(&v, &i);
	i++;
	//VectorInsert(&v, &i, 1);
	VectorDelete(&v, 1);

	printf("%d\n", *(int *)VectorNth(&v, 0) );
	printf("%d\n", *(int *)VectorNth(&v, 1) );
	printf("%d\n", *(int *)VectorNth(&v, 2) );
	printf("%d\n", *(int *)VectorNth(&v, 3) );
	printf("%d\n", *(int *)VectorNth(&v, 4) );
	printf("%d\n", *(int *)VectorNth(&v, 5) );
	//printf("%d\n", *(int *)VectorNth(&v, 6) );

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
{
	int i;
	void *elemAddr;
	if(v->freeFn != NULL){
		for(i = 0; i < v->pos; i++){
			v->freeFn( (char *)v->elems + i*v->elemSize );
		}
	}
	free(v->elems);
	printf("Vector eliminado\n");
}

int VectorLength(const vector *v)
{
	return v->pos;
}

void *VectorNth(const vector *v, int position)//Regresa la posición de memoria del elemento
{ 
	if(position >= 0 && position <= v->pos){
		return v->elems + v->elemSize*position;
	}
	return NULL;
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
	if(position >= 0 && position <= v->pos){
		void * elemNewAddr = (char *)v->elems + position*v->elemSize;
		memcpy(elemNewAddr,elemAddr,v->elemSize);
	}
}

void VectorInsert(vector *v, const void *elemAddr, int position)//Se utiliza memmove
{
	if(position >= 0 && position <= v->pos){
		if(position == v->pos){
			VectorAppend(v, elemAddr);
		}else{
			printf("Insertando en la posición %d...\n", position);
			void * elemNewAddr;
			void * addrDest;
			elemNewAddr = (char *) v->elems + position*v->elemSize;
			addrDest = (char *) elemNewAddr + v->elemSize;
			size_t memSize= ( v->elems + v->pos*v->elemSize ) - ( elemNewAddr );
			memmove(addrDest, elemNewAddr, memSize);
			memcpy(elemNewAddr, elemAddr, v->elemSize);
		}
	}
}

void VectorAppend(vector *v, const void *elemAddr)
{
	if(v->pos == v->memSize){
		v->memSize += v->reSize;
		v->elems = realloc(v->elems,v->memSize*v->elemSize);
	}
	void * elemNewAddr;
	elemNewAddr = (char *) v->elems + v->pos * v->elemSize;
	memcpy(elemNewAddr,elemAddr,v->elemSize);
	v->pos++;
}

void VectorDelete(vector *v, int position)
{
	if(position >= 0 && position <= v->pos){
		printf("Elimiando de la posición %d...\n", position);
		void * memNewAddr;
		void * memOldAddr;
		memNewAddr = (char *) v->elems + position*v->elemSize;
		memOldAddr = (char *) memNewAddr + v->elemSize;
		size_t memSize = ( v->elems + v->pos*v->elemSize ) - ( memOldAddr );
		memmove(memNewAddr, memOldAddr, memSize);
		v->pos--;
	}	
}

void VectorSort(vector *v, VectorCompareFunction compare)
{}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)//auxData se usa como segundo argumento de la funcipn mapFn
{}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)//lfind para busqueda lineal
{ return -1; } 
