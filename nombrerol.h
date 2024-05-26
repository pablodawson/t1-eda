#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NombreRol {
    char caracter;
    struct NombreRol *sgte;
} NombreRol;

typedef struct NombreRolEncriptado {
    char caracter;
    struct NombreRolEncriptado *sgte;
} NombreRolEncriptado;

void ingresarNombreRol(char *str, NombreRol **head);
int encontrarMin(NombreRol *head);
int encontrarMax(NombreRol *head);
int encontrarLongitud(NombreRol *head);
void encriptarNombreRol(NombreRol *head, NombreRolEncriptado **encriptedHead, int min, int max);
void desencriptarNombreRol(NombreRol *head, NombreRol **headDesencriptado, int min, int max);
//void pushPila(NombreRol *head);
