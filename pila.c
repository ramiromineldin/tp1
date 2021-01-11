#include "pila.h"
#include <stdlib.h>

#define CAPACIDAD_INI 10
#define FACTOR_REDIM 2
#define FACTOR_REDIM_AUX 4
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    
    if (pila == NULL) {
        return NULL;
    }
    
    void **datos_nuevo = malloc(CAPACIDAD_INI*sizeof(void*));
    pila->datos = datos_nuevo;
    
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    
    pila->capacidad = CAPACIDAD_INI;
    pila->cantidad = 0;
    
    return pila; 
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
     return pila->cantidad == 0;
    
}

static bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {
    if (capacidad_nueva > pila->capacidad) {
        void **datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));
        if (datos_nuevo == NULL) return false; 

        pila->datos = datos_nuevo;
        pila->capacidad *= FACTOR_REDIM;
        return true;
    }
    else {
        void **datos_nuevo = realloc(pila->datos, (capacidad_nueva * sizeof(void*)));
        
        if (datos_nuevo == NULL) return false;
        
        pila->datos = datos_nuevo;
        pila->capacidad /= FACTOR_REDIM;
        return true;

    }
}

bool pila_apilar(pila_t *pila, void *valor) {
    
    if (pila->cantidad == pila->capacidad) {
        size_t capacidad_nueva = pila->capacidad * FACTOR_REDIM;
        if (!pila_redimensionar(pila, capacidad_nueva)) return false;
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad ++ ;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;
    
    return pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    
    if (pila->capacidad > CAPACIDAD_INI && pila->cantidad * FACTOR_REDIM_AUX <= pila->capacidad) {
        size_t capacidad_nueva = pila->capacidad / FACTOR_REDIM;
        pila_redimensionar(pila, capacidad_nueva);
    }
    void *tope = pila->datos[pila->cantidad-1];
    pila->cantidad --;
    
    return tope;
}
    
    
        
