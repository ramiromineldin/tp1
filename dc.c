#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {

    char *linea = NULL;
    size_t tam = 0;
    while ((getline(&linea, &tam, stdin) != EOF)) {
        struct calc_token tok;
        pilanum_t *pila = pilanum_crear();
        char **palabras = dc_split(linea);
        bool error = false;

        int contador_numeros = 0; // Cuenta la cantidad de numeros que hay en la pila.
        int contador_operadores = 0; // Cuenta la cantidad de operadores que hay en la linea.
        for (size_t i = 0; palabras[i]; i++) {
            calc_num resultado;
            calc_parse(palabras[i], &tok);
            if (tok.type == TOK_NUM) {
                apilar_num(pila, tok.value);
                contador_numeros++;
            }
            else if (tok.type == TOK_OPER) { // Determina que tipo de operador es y realiza la operacion que corresponda.
                contador_operadores++;                
                if (tok.oper.num_operandos == 2 && contador_numeros >= 2) {
                    calc_num num_1, num_2;
                    desapilar_num(pila, &num_1);
                    desapilar_num(pila, &num_2);
                    
                    if (tok.oper.op == OP_ADD) resultado = num_1 + num_2;
                    else if (tok.oper.op == OP_SUB) resultado = num_2 - num_1;
                    else if (tok.oper.op == OP_MUL) resultado = num_1 * num_2;
                    else if (tok.oper.op == OP_DIV) {
                        if (num_1 == 0) { // Comprueba que el divisor no sea 0
                        printf("ERROR\n");
                        error = true;
                        break;
                        }
                        resultado = num_2 / num_1;
                    }
                    else if (tok.oper.op == OP_POW) {
                        if (num_1 < 0) { // Comprueba que el exponente no sea negativo.
                        printf("ERROR\n");
                        error = true;
                        break;
                        }
                        double calculo = pow((double) num_2, (double) num_1);
                        resultado = (calc_num)calculo;
                    }
                    else if (tok.oper.op == OP_LOG) {
                        if (num_1 < 2 || num_2 < 0) {
                        printf("ERROR\n");
                        error = true;
                        break;
                        }
                        double calculo =  (log10((double) num_2) / log10((double) num_1));
                        resultado = (calc_num) calculo;
                    }
                    contador_numeros--;
                    contador_operadores--;

                }
                else if (tok.oper.num_operandos == 1  && contador_numeros >= 1) {
                    calc_num num_1;
                    desapilar_num(pila, &num_1);
                    if (tok.oper.op == OP_RAIZ) {
                        if (num_1 < 0) { // Comprueba que el radicando no sea menor a 0
                        printf("ERROR\n");
                        error = true;
                        break;
                        }
                        double calculo = sqrt((double) num_1);
                        resultado = (calc_num)calculo;
                    }
                    contador_operadores--;
                }
                else if (tok.oper.num_operandos == 3 && contador_numeros >= 3) {
                    calc_num num_1, num_2, num_3;
                    desapilar_num(pila, &num_1);
                    desapilar_num(pila, &num_2);
                    desapilar_num(pila, &num_3);
                    resultado = (num_3 != 0) ? num_2 : num_1;
                    contador_numeros -= 2;
                    contador_operadores--;
                }
                apilar_num(pila, resultado);
            }
            else { // En caso de que el carácter no sea ni un numero ni un operador imprime ERROR
                printf("ERROR\n");
                error = true;
                break;
            } 
            
            
        }
        if (!error) {
            if (contador_operadores >= contador_numeros) printf("ERROR\n");
            else if (contador_numeros > 1) printf("ERROR\n");
            else if (contador_numeros == 0) printf("ERROR\n");
            else {
                calc_num resultado_final;
                desapilar_num(pila, &resultado_final);
                fprintf(stdout,"%li\n", resultado_final);
            }
        }
        pilanum_destruir(pila);
        free_strv(palabras);
    }
    free(linea);
    return 0;
}
