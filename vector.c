#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>


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
	if(v->freeFn != NULL){//si hay una funcion para borrar la utiliza
		int i;
		for(i = 0; i < v->pos; i++){//Itera para aplicar la funcion freeFn en cada elemento del vector
			v->freeFn( (char *)v->elems + i*v->elemSize );
		}
	}
	free(v->elems);
	v->pos = 0;
}

int VectorLength(const vector *v)
{
	return v->pos;//Regresa la posicion, la posicion es igual al tamaño del vector
}

void *VectorNth(const vector *v, int position)//Regresa la posición de memoria del elemento
{ 
	assert(position >= 0 && position <= v->pos);
	return v->elems + v->elemSize*position;//Calcula y regresa la posicion de memoria del elementos
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
	assert(position >= 0 && position <= v->pos);
	void * elemNewAddr = v->elems + position*v->elemSize; //Calcula la direccion de memoria del elemento a reemplazar
	memcpy(elemNewAddr,elemAddr,v->elemSize);//escribe el nuevo valor en la direccion de memoria
}

void VectorInsert(vector *v, const void *elemAddr, int position)//Se utiliza memmove
{
	assert(position >= 0 && position <= v->pos);
	if(position == v->pos){//Si es la ultima posicion hace un append
		VectorAppend(v, elemAddr);
	}else{
		if(v->pos == v->memSize){//si la posicion es igual a el tamaño de memoria asignada se hace un realloc
			v->memSize += v->reSize;
			v->elems = realloc(v->elems, (v->memSize + v->reSize) );
		}
		void * elemNewAddr;
		void * addrDest;
		elemNewAddr = v->elems + position*v->elemSize;//Calcula la direccion de memoria a donde se va a insertar
		addrDest = elemNewAddr + v->elemSize;//Calcula la direccion a donde se van a recorrer los elementos que estan a partir de esa posicion
		size_t memSize= ( v->elems + v->pos*v->elemSize ) - ( elemNewAddr );//Calcula el tamaño de memoria a mover
		memmove(addrDest, elemNewAddr, memSize);//Recorre los elementos a la sig posicion de memoria
		memcpy(elemNewAddr, elemAddr, v->elemSize);//Copia el valor a la direccion de memoria de la posicion
		v->pos++;
	}
}

void VectorAppend(vector *v, const void *elemAddr)
{
	if(v->pos == v->memSize){//Si es necesario se asigna mas memoria
		v->memSize += v->reSize;
		v->elems = realloc(v->elems,v->memSize*v->elemSize);
	}
	void * elemNewAddr;
	elemNewAddr = v->elems + v->pos * v->elemSize;//Calcula la direccion de memoria a donde se va a insertar
	memcpy(elemNewAddr,elemAddr,v->elemSize);//copia el valor a la direccion siguiente en el vector
	v->pos++;
}

void VectorDelete(vector *v, int position)
{
	assert(v->pos >= position); //se valida la posicion a borrar
	assert(position >= 0);
	void *addr, *prevAddr, *addrCero;
	if(position == 0 )//si se desea borrar el primer elemento del vector se trata como caso especial
	{		
		addrCero = v->elems + 1 * v->elemSize;
		memmove(v->elems, addrCero, (v->pos)*v->elemSize);
	}else{
		void *addr, *prevAddr;
		addr = v->elems + position * v->elemSize;//Calcula la direccion de memoria del elemento a borrar
		prevAddr = v->elems + (position-1) * v->elemSize;//Calcula la direccion a donde se va a mover la memoria
		memmove(prevAddr, addr, (v->pos-position)*v->elemSize);
	}
	v->pos--;//se decrementa el puntero
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
	qsort(v->elems, v->pos, v->elemSize, compare);//Utiliza quicksort para ordenar los elementos
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)//auxData se usa como segundo argumento de la funcipn mapFn
{
	int i;
	void * elemAddr;
	for(i = 0; i < v->pos; i++)//Itera en cada elemento para aplicar la funcion mapFn
		mapFn(VectorNth(v, i), auxData);
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)//lfind para busqueda lineal
{ 
	void *addr, *temp;
	int i;
	if(isSorted){//Caso en donde los datos estan ordenados
		addr = bsearch(key, v->elems, v->pos, v->elemSize, searchFn);//Busca la direccion de memoria
		for(i=startIndex; i < v->pos; i++){//Itera para obtener la poscion de la direccion de memoria
			temp = v->elems + i * v->elemSize;
			if(temp==addr)
				return i;
		}
	}else{//Caso donde los elementos NO estan ordenados
		size_t size = v->pos;
		addr = lfind(key, v->elems, &size, v->elemSize, searchFn);//Busca la direccion de memoria
		for(i=startIndex; i < v->pos; i++){//Itera para obtener la poscion de la direccion de memoria
			temp = v->elems + i * v->elemSize;
			if(temp==addr)
				return i;
		}
	}
	return -1; 
} 
