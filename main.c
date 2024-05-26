// main.c
#include "nombrerol.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <math.h>

void ingresarNombreRol(char *str, NombreRol **head) {
    NombreRol *newNode, *temp;
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        newNode = (NombreRol *)malloc(sizeof(NombreRol));
        newNode->caracter = str[i];
        newNode->sgte = NULL;
        if (*head == NULL) {
            *head = newNode;
        } else {
            temp = *head;
            while (temp->sgte != NULL) {
                temp = temp->sgte;
            }
            temp->sgte = newNode;
        }
    }
}

int encontrarMin(NombreRol *head) {
    NombreRol *temp = head;
    int min = 255;
    while (temp != NULL) {
        unsigned char ascii = (unsigned char)temp->caracter;
        if (ascii < min) {
            min = ascii;
        }
        temp = temp->sgte;
    }
    return min;
}

int encontrarMax(NombreRol *head) {
    NombreRol *temp = head;
    int max = 0;
    while (temp != NULL) {
        unsigned char ascii = (unsigned char)temp->caracter;
        if (ascii > max) {
            max = ascii;
        }
        temp = temp->sgte;
    }
    return max;
}

int encontrarLongitud(NombreRol *head) {
    NombreRol *temp = head;
    int longitud = 0;
    while (temp != NULL) {
        longitud++;
        temp = temp->sgte;
    }
    return longitud;
}

void encriptarNombreRol(NombreRol *head, NombreRolEncriptado **encriptedHead, int min, int max) {
    NombreRol *temp = head;
    NombreRolEncriptado *newNode, *eTemp;

    int range = max - min;

    temp = head;

    while (temp != NULL) {
        unsigned char ascii = (unsigned char)temp->caracter;
        int scaledValue = round((((float)ascii - (float)min) / (float)range) * 255);

        newNode = (NombreRolEncriptado *)malloc(sizeof(NombreRolEncriptado));
        newNode->caracter = scaledValue;
        newNode->sgte = NULL;

        if (*encriptedHead == NULL) {
            *encriptedHead = newNode;
        } else {
            eTemp = *encriptedHead;
            while (eTemp->sgte != NULL) {
                eTemp = eTemp->sgte;
            }
            eTemp->sgte = newNode;
        }
        temp = temp->sgte;
    }
}

//void escribirEnImagen(NombreRolEncriptado *head, unsigned char **imagen, int x, int y, int ANCHO, int ALTO) {
    
//}

void pushPila(NombreRol *head) {
    NombreRol *temp = head;
    while (temp != NULL) {
        printf("%c", temp->caracter);
        temp = temp->sgte;
    }
    printf("\n");
}

unsigned char obtener_luminancia(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned char)round(0.2126 * r + 0.7152 * g + 0.0722 * b);
}

void desencriptarNombreRol(NombreRol *head, NombreRol **headDesencriptado, int min, int max) {
    NombreRol *temp = head;
    NombreRol *newNode, *dTemp;

    int range = max - min;

    temp = head;

    while (temp != NULL) {
        unsigned char ascii = (unsigned char)temp->caracter;
        int scaledValue = round(range*((float)ascii / 255.0)) + min;
        printf("%d ", scaledValue);

        newNode = (NombreRol *)malloc(sizeof(NombreRol));
        newNode->caracter = scaledValue;
        newNode->sgte = NULL;

        if (*headDesencriptado == NULL) {
            *headDesencriptado = newNode;
        } else {
            dTemp = *headDesencriptado;
            while (dTemp->sgte != NULL) {
                dTemp = dTemp->sgte;
            }
            dTemp->sgte = newNode;
        }
        temp = temp->sgte;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nombre y rol>\n", argv[0]);
        return 1;
    }

    NombreRol *head = NULL;
    NombreRolEncriptado *headEncriptado = NULL;
    

    ingresarNombreRol(argv[1], &head);

    int min = encontrarMin(head);
    int max = encontrarMax(head);
    int longitud = encontrarLongitud(head);

    printf("min: %d\n", min);
    printf("max: %d\n", max);
    
    encriptarNombreRol(head, &headEncriptado, min, max);

    // Imprimir original
    NombreRol *temp1 = head;

    printf("\n");
    printf("Original: ");
    while (temp1 != NULL) {
        printf("%u ", (unsigned char)temp1->caracter);
        temp1 = temp1->sgte;
    }

    printf("\n");
    printf("Encriptado: ");
    // Imprimir encriptado
    NombreRolEncriptado *temp2 = headEncriptado;
    
    while (temp2 != NULL) {
        printf("%u ", (unsigned char)temp2->caracter);
        temp2 = temp2->sgte;
    }
    printf("\n");


    // Cargar imagen
    int i, j;

    int ANCHO, ALTO, canales;
    unsigned char* image = stbi_load("utfsmGREY.jpg", &ANCHO, &ALTO, &canales, 3);
    unsigned char imagen_gs[ALTO][ANCHO];

    if (image == NULL) {
        fprintf(stderr, "Error cargando imagen\n");
        return 1;
    }

    // Crear matriz de luminancia
    for (i = 0; i < ALTO; i++) {
        for (j = 0; j < ANCHO; j++) {
            unsigned char* pixel = &image[(i * ANCHO + j) * canales];
            imagen_gs[i][j] = obtener_luminancia(pixel[2], pixel[1], pixel[0]);
        }
    }

    stbi_image_free(image);

    // Coordenadas donde escribir en la imagen
    int x = 100, y = 100;

    // Escribir en la imagen
    NombreRolEncriptado *temp = headEncriptado;
    i = y;
    j = x;
    while (temp != NULL) {
        imagen_gs[i][j] = (unsigned char)temp->caracter;
        temp = temp->sgte;
        j++;
        if (j >= ANCHO) { // Ajustar al ancho de la imagen
            j = 0;
            i++;
            if (i >= ALTO) break; // Ajustar al alto de la imagen
        }
    }
    //imagen_gs[i][j] = 0;

    // Guardar imagen nueva (rgb) repitiendo los valores en todos los canales
    unsigned char imagen_rgb[ALTO][ANCHO][3];

    for (i = 0; i < ALTO; i++) {
        for (j = 0; j < ANCHO; j++) {
            imagen_rgb[i][j][0] = imagen_gs[i][j];
            imagen_rgb[i][j][1] = imagen_gs[i][j];
            imagen_rgb[i][j][2] = imagen_gs[i][j];
        }
    }

    // Guardar imagen
    stbi_write_jpg("utfsmGREY_encriptado.jpg", ANCHO, ALTO, 3, imagen_rgb, 100);

    //unsigned char* image_encriptada = stbi_load("utfsmGREY_encriptado.jpg", &ANCHO, &ALTO, &canales, 3);
    unsigned char imagen_encriptada_gs[ALTO][ANCHO];

    // Matriz de luminancia. En base a matriz creada anteriormente.
    for (i = 0; i < ALTO; i++) {
        for (j = 0; j < ANCHO; j++) {
            unsigned char* pixel = imagen_rgb[i][j];
            imagen_encriptada_gs[i][j] = pixel[0];//obtener_luminancia(pixel[2], pixel[1], pixel[0]);
        }
    }

    //stbi_image_free(image_encriptada);

    NombreRol *headNormalizado = NULL;
    
    j = x;
    i = y;

    printf("\n:");
    printf("Desencriptado sin escalar: \n");

    int z = 0;

    NombreRol **current = &headNormalizado;
    while (z < longitud) {
        if (j >= ANCHO) {
            j = 0;
            i++;
        }
        NombreRol *newNode = malloc(sizeof(NombreRol));
        newNode->caracter = imagen_encriptada_gs[y][j];
        printf("%u ", (unsigned char)newNode->caracter);
        newNode->sgte = NULL;
        *current = newNode;
        current = &newNode->sgte;
        j++;
        z++;
    }

    NombreRol *nombreRolOriginal = head;
    NombreRol *nombreRolDesencriptado = NULL;

    printf("\n");
    desencriptarNombreRol(headNormalizado, &nombreRolDesencriptado, min, max);
    printf("\n");
    
    printf("Desencriptado (pila): ");
    NombreRol *temp3 = nombreRolDesencriptado;
    while (temp3 != NULL) {
        printf("%c", temp3->caracter);
        temp3 = temp3->sgte;
    }
    
    return 0;
}