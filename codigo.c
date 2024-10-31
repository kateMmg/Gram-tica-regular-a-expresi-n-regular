#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM 300

typedef struct nodo1 {
    char *identificador_regla;
    char *produccion;              
    struct nodo1 *sig; 
} TNodo;

typedef struct nodoN {
    char *identificador_regla;
    char **producciones;
    int num_producciones;
    struct nodoN *sig;
} TNodoNuevo;

TNodo *crea_nodo(char *identificador_regla, char *produccion);
void inserta_final(TNodo **cab, char *identificador_regla, char *produccion);
TNodo* leer_archivo(char *nombre_archivo);
void imprime(TNodo *cab);
void inserta_finalNuevo(TNodoNuevo **cab, char *identificador_regla, char *produccion);
void juntar_producciones(TNodo **cab, TNodoNuevo **cabN);
void eliminar_recursividad(TNodoNuevo **cab1, TNodoNuevo **cab2);
void convertir_arriba_abajo(TNodoNuevo *cab);
void liberar_lista(TNodo *cab);
void liberar_lista_nueva(TNodoNuevo *cab);
void imprimeNuevo(TNodoNuevo *cab);

int main() {
    char *nombre_archivo = "gramatica2.txt";
    TNodo *lista = leer_archivo(nombre_archivo);
    TNodoNuevo *cabN = NULL;
    TNodoNuevo *cab2 = NULL;

    imprime(lista);
    juntar_producciones(&lista, &cabN);
    imprimeNuevo(cabN);
    eliminar_recursividad(&cabN, &cab2);
    printf("Lista:\n");
    imprimeNuevo(cab2);
    liberar_lista(lista);
    liberar_lista_nueva(cabN);
    liberar_lista_nueva(cab2);
    
    return 0;
}

TNodo *crea_nodo(char *identificador_regla, char *produccion) {
    TNodo *aux = (TNodo *)malloc(sizeof(TNodo));
    if (aux) {
        aux->identificador_regla = strdup(identificador_regla);
        aux->produccion = strdup(produccion);
        aux->sig = NULL;
    }
    return aux;
}

void inserta_final(TNodo **cab, char *identificador_regla, char *produccion) {
    TNodo *aux = crea_nodo(identificador_regla, produccion);
    if (aux) {
        if (*cab == NULL) { 
            *cab = aux;
        } else {
            TNodo *corre = *cab;
            while (corre->sig != NULL) { 
                corre = corre->sig;
            }
            corre->sig = aux;
        }
    }
}

void inserta_finalNuevo(TNodoNuevo **cab, char *identificador_regla, char *produccion) {
    TNodoNuevo *nuevo_nodo = (TNodoNuevo *)malloc(sizeof(TNodoNuevo));
    if (nuevo_nodo == NULL) {
        perror("Error al asignar memoria para el nodo");
        exit(EXIT_FAILURE);
    }
    nuevo_nodo->identificador_regla = strdup(identificador_regla);
    nuevo_nodo->producciones = (char **)malloc(sizeof(char *)); 
    nuevo_nodo->producciones[0] = strdup(produccion); 
    nuevo_nodo->num_producciones = 1; 
    nuevo_nodo->sig = NULL;

    if (*cab == NULL) { 
        *cab = nuevo_nodo;
    } else {
        TNodoNuevo *corre = *cab;
        while (corre->sig != NULL) { 
            corre = corre->sig;
        }
        corre->sig = nuevo_nodo;
    }
}

TNodo* leer_archivo(char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    TNodo *cabeza = NULL;
    char linea[TAM];
    char identificador_regla[TAM];
    char productor[TAM];

    while (fgets(linea, TAM, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
        const char *delimitador = strstr(linea, "->");
        if (delimitador != NULL) {
            strncpy(identificador_regla, linea, delimitador - linea);
            identificador_regla[delimitador - linea] = '\0';
            strcpy(productor, delimitador + 2);
            inserta_final(&cabeza, identificador_regla, productor); 
        } 
    }
    fclose(archivo);
    return cabeza;
}

void imprime(TNodo *cab) {
    TNodo *aux = cab;
    printf("Lista impresa en terminal:\n");
    while (aux != NULL) {
        printf("identificador: %s, produccion: %s\n", aux->identificador_regla, aux->produccion);
        aux = aux->sig;
    }
}

void juntar_producciones(TNodo **cab, TNodoNuevo **cabN) {
    TNodo *corre = *cab;
    while (corre) {
        TNodoNuevo *existente = *cabN;
        while (existente != NULL && strcmp(existente->identificador_regla, corre->identificador_regla) != 0) {
            existente = existente->sig;
        }

        if (existente == NULL) {
            inserta_finalNuevo(cabN, corre->identificador_regla, corre->produccion);
        } else {
            existente->producciones = realloc(existente->producciones, (existente->num_producciones + 1) * sizeof(char *));
            if (existente->producciones != NULL) {
                existente->producciones[existente->num_producciones] = strdup(corre->produccion);
                existente->num_producciones++; 
            }
        }

        TNodo *temp = corre;
        corre = corre->sig;
        free(temp->identificador_regla);
        free(temp->produccion);
        free(temp);
    }
    *cab = NULL; 
}

void eliminar_recursividad(TNodoNuevo **cab1, TNodoNuevo **cab2) {
    TNodoNuevo *aux1 = *cab1; 
    while (aux1) {
        TNodoNuevo *nuevo_nodo = (TNodoNuevo *)malloc(sizeof(TNodoNuevo));
        if (nuevo_nodo == NULL) {
            perror("Error al asignar memoria para el nodo");
            exit(EXIT_FAILURE);
        }

        nuevo_nodo->identificador_regla = strdup(aux1->identificador_regla); 
        nuevo_nodo->producciones = NULL; 
        nuevo_nodo->num_producciones = 0; 

        for (int i = 0; i < aux1->num_producciones; i++) {
            char *produccion = aux1->producciones[i];
            char *produccion_modificada = strdup(produccion);
            int tiene_recursividad = strchr(produccion, aux1->identificador_regla[0]) != NULL;

            if (tiene_recursividad) {
                for (int j = 0; produccion_modificada[j] != '\0'; j++) {
                    if (produccion_modificada[j] == aux1->identificador_regla[0]) {
                        produccion_modificada[j] = '*'; 
                    }
                }
                nuevo_nodo->producciones = realloc(nuevo_nodo->producciones, 
                    (nuevo_nodo->num_producciones + 1) * sizeof(char *));
                nuevo_nodo->producciones[nuevo_nodo->num_producciones++] = produccion_modificada;
            } else {
                nuevo_nodo->producciones = realloc(nuevo_nodo->producciones, 
                    (nuevo_nodo->num_producciones + 1) * sizeof(char *));
                nuevo_nodo->producciones[nuevo_nodo->num_producciones++] = produccion_modificada;
            }
        }

        nuevo_nodo->sig = *cab2;
        *cab2 = nuevo_nodo;

        aux1 = aux1->sig;
    }

    aux1 = *cab1;
    while (aux1) {
        TNodoNuevo *temp = aux1;
        aux1 = aux1->sig;
        free(temp->identificador_regla);
        free(temp->producciones);
        free(temp);
    }
}

void imprimeNuevo(TNodoNuevo *cab) {
    TNodoNuevo *corre = cab;
    while (corre) {
        printf("%s -> ", corre->identificador_regla);
        for (int i = 0; i < corre->num_producciones; i++) {
            if (i > 0) printf(" | "); 
            printf("%s", corre->producciones[i]);
        }
        printf("\n");
        corre = corre->sig;
    }
}

void liberar_lista(TNodo *cab) {
    TNodo *temp;
    while (cab != NULL) {
        temp = cab;
        cab = cab->sig;
        free(temp->identificador_regla);
        free(temp->produccion); 
        free(temp);    
    }
}

void liberar_lista_nueva(TNodoNuevo *cab) {
    TNodoNuevo *temp;
    while (cab != NULL) {
        temp = cab;
        cab = cab->sig;
        free(temp->identificador_regla);
        for (int i = 0; i < temp->num_producciones; i++) {
            free(temp->producciones[i]);
        }
        free(temp->producciones);
        free(temp);
    }
}
