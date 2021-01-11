#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CADENA_FIN '\0'

char *substr(const char *str, size_t n) {
   return strndup(str, n);

} 

/* Funcion auxiliar */
size_t contar_separaciones(const char *str, char sep, size_t largo) {
    size_t contador = 0;
    for (size_t j = 0; j < largo; j++) {
        if (str[j] == sep) contador ++;
    }
    return contador;
}

char **split(const char *str, char sep) {
    if (!str) return NULL;
    size_t largo = strlen(str);
    size_t cantidad_subcadenas = contar_separaciones(str, sep, largo) + 1; // Le sumo 1 por la ultima subcadena despues del ultimo separador
    
    char **cadena = malloc(sizeof(char*) * (cantidad_subcadenas + 1)); // Le sumo 1 para el '\0' del final
    if (!cadena) return NULL;

    //if (cantidad_subcadenas == 2 || sep == CADENA_FIN) {
      //  cadena[0] = strdup(str);
    //}
    size_t posicion_str = 0; // Determina la posicion actual
    size_t posicion_cadena = 0;
    size_t posicion_copiar = 0; // Determinar a partir de que posicion del arreglo original copiar
    for (int i = 0; i < cantidad_subcadenas; i++) {
        size_t cantidad_caracteres = 0;
        while(str[posicion_str] != sep && posicion_str < largo) {
            cantidad_caracteres++;
            posicion_str++;
        }
        char *sub_cadena = substr(str + posicion_copiar, cantidad_caracteres);
        cadena[posicion_cadena] = sub_cadena;
        posicion_cadena++;
        posicion_copiar += cantidad_caracteres + 1; // Le sumo 1 para no copiar al separador
        posicion_str++;  // Le sumo 1 para saltear al separador
    }

    cadena[posicion_cadena] = NULL;
    return cadena;
    
} 


char *join(char **strv, char sep) {
    size_t largo_palabras = 0;
    size_t cantidad_separadores = 0;

    
    for (size_t j = 0; strv[j]; j++) {
        largo_palabras += (strlen(strv[j]));
        cantidad_separadores++;
    }
    
    char *cadena = calloc((largo_palabras + cantidad_separadores + 1), sizeof(char));
    if (!cadena) return NULL;
    
    size_t posicion_cadena = 0;
    for(size_t i = 0; strv[i]; i++) {
        for (size_t j = 0; strv[i][j]; j++) {
            cadena[posicion_cadena] = strv[i][j];
            posicion_cadena++;
        }
        if (sep != CADENA_FIN && strv[i+1]) {
          cadena[posicion_cadena] = sep;
          posicion_cadena++;
        }
    }
    return cadena;
}

void free_strv(char *strv[]) {
    for (size_t j = 0; strv[j]; j++) {
        free(strv[j]);
    } 
    free(strv);
}


