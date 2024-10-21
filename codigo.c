#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM 300

typedef struct nodo1 
{
    char *identificador_regla;
    char *produccion;              
    struct nodo1 *sig; 
} TNodo;

TNodo *crea_nodo(char *identificador_regla, char *produccion);
void inserta_final(TNodo **cab, char *identificaodr_regla, char *produccion);
TNodo* leer_archivo(char *nombre_archivo);
void imprime(TNodo *cab);

int main() 
{
    char *nombre_archivo = "gramatica1.txt";
    TNodo *lista = leer_archivo(nombre_archivo);

    imprime(lista);

    TNodo *temp;
    while (lista != NULL) 
    {
        temp = lista;
        lista = lista->sig;
        free(temp->identificador_regla);
        free(temp->produccion); 
        free(temp);    
    }
    return 0;
}

TNodo *crea_nodo(char *identificaodr_regla, char *produccion) 
{
    TNodo *aux = (TNodo *)malloc(sizeof(TNodo));
    if (aux) 
    {
        aux->identificador_regla = strdup(identificaodr_regla);
        aux->produccion = strdup(produccion);
        aux->sig = NULL;
    }
    return aux;
}

void inserta_final(TNodo **cab, char *identificador_regla, char *produccion) 
{
    TNodo *aux = crea_nodo(identificador_regla, produccion);
    if (aux) 
    {
        if (*cab == NULL) 
        { 
            *cab = aux;
        } 
        else 
        {
            TNodo *corre = *cab;
            while (corre->sig != NULL) 
            { 
                corre = corre->sig;
            }
            corre->sig = aux;
        }
    }
}

TNodo* leer_archivo(char *nombre_archivo) 
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) 
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    TNodo *cabeza = NULL;
    char linea[TAM];
    char identificador_regla[TAM];
    char productor[TAM];

    while (fgets(linea, TAM, archivo)) 
    {
        linea[strcspn(linea, "\n")] = '\0';
        const char *delimitador = strstr(linea, "->");
        if(delimitador != NULL)
        {
            strncpy(identificador_regla, linea, delimitador - linea);
            identificador_regla[delimitador - linea] = '\0';
            strcpy(productor, delimitador + 2);
            inserta_final(&cabeza, identificador_regla, productor); 
        } 
    }
    fclose(archivo);
    return cabeza;
}

void imprime(TNodo *cab) 
{
    TNodo *aux = cab;
    printf("Lista impresa en terminal:\n");
    while (aux != NULL) 
    {
        printf("identificador: %s, produccion: %s\n", aux->identificador_regla, aux->produccion);
        aux = aux->sig;
    }
}
