#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo{
    char *linea;
    struct nodo *siguiente;
}TNodo;

TNodo *crea_nodo(char *linea);
void inserta_final(TNodo **cab, char *linea);
TNodo* leerArchivo(TNodo **cab, char *nombreArchivo);
void imprimirLista(TNodo *cab);

int main() {
    char *nombreArchivo = "gramatica1.txt";
    TNodo *cab = NULL;
    TNodo *lista = leerArchivo(&cab, nombreArchivo);
    
    imprimirLista(lista);
    return 0;
}

TNodo *crea_nodo(char *linea)
{
    TNodo *nodo;
    nodo = (TNodo*)malloc(sizeof(TNodo));
    if (nodo)
    {
        nodo->linea = strdup(linea);
        nodo->siguiente = NULL;
    }
    return (nodo);
}

void inserta_final(TNodo **cab, char *linea)
{
    TNodo *corre;
    if(*cab == NULL)
        *cab = crea_nodo(linea);
    else{
        corre = *cab;
        while(corre->siguiente != NULL)
            corre = corre->siguiente;
        corre->siguiente = crea_nodo(linea);
    }
}

TNodo* leerArchivo(TNodo **cab, char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char linea[50];
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
        inserta_final(cab, linea);
    }

    fclose(archivo);
    return *cab;
}

void imprimirLista(TNodo *cab) {
    TNodo *corre = cab;
    while (corre != NULL) 
    {
        printf("%s\n", corre->linea);
        corre = corre->siguiente;
    }
}
