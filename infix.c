#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    char *linea = NULL;
    size_t tam = 0;
    while ((getline(&linea, &tam, stdin) != EOF)) {
        struct calc_token tok;
        pila_t *pila_operadores = pila_crear();
        char **palabras = infix_split(linea);
        int contador_caracteres = 0; // Cuenta la cantidad de caracteres que tiene la linea sin contar los espacios
        
        for (int i = 0; palabras[i]; i++) {
            contador_caracteres++;
        }
        
        char **linea_transformada = calloc((contador_caracteres + 1), sizeof(char*));
        size_t posicion = 0;
        
        for(size_t j = 0; palabras[j]; j++) {
            calc_parse(palabras[j], &tok);
            if (tok.type == TOK_NUM) {
                linea_transformada[posicion] = palabras[j];
                posicion++;
            }
            else if (tok.type == TOK_OPER) {
                struct calc_token tok_tope;
                if (!pila_esta_vacia(pila_operadores)) {
                    calc_parse((char*)pila_ver_tope(pila_operadores), &tok_tope);
                }
                while (!pila_esta_vacia(pila_operadores) && (tok_tope.oper.precedencia >= tok.oper.precedencia)) {
                    if (tok.oper.asociatividad == ASSOC_RIGHT) break;
                    linea_transformada[posicion] = (char*)pila_desapilar(pila_operadores);
                    if (!pila_esta_vacia(pila_operadores)) calc_parse((char*)pila_ver_tope(pila_operadores), &tok_tope);
                    posicion++;
                }
                pila_apilar(pila_operadores, palabras[j]);
                 
            } 
            else if (tok.type == TOK_LPAREN) {
                pila_apilar(pila_operadores, palabras[j]);
            }
            else if (tok.type == TOK_RPAREN) {
                struct calc_token tok_tope;
                calc_parse((char*)pila_ver_tope(pila_operadores), &tok_tope);
                while(tok_tope.type != TOK_LPAREN) { // Desapilo y agrego al arreglo todos los operadores hasta que el tope sea el parentesis de apertura
                    linea_transformada[posicion] = (char*) pila_desapilar(pila_operadores);
                    calc_parse((char*)pila_ver_tope(pila_operadores), &tok_tope);
                    posicion++;
                }
                pila_desapilar(pila_operadores); // Desapilo el parentesis de apertura
            }    
        }
        while (!pila_esta_vacia(pila_operadores)) { // Agrego al arreglo  todos los operadores que quedaron en la pila
            linea_transformada[posicion] = (char*)pila_desapilar(pila_operadores);
            posicion++;
        }
        linea_transformada[contador_caracteres] = NULL;
        char *linea_final = join(linea_transformada, ' ');
        fprintf(stdout,"%s\n", linea_final);
        free_strv(palabras);
        free(linea_final);
        free(linea_transformada);
        pila_destruir(pila_operadores);
        
    }
    free(linea);
    return 0;
}

