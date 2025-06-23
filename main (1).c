/**************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

***************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZONAS 5
#define DIAS 30
#define CONTAMINANTES 6 // pm25, pm10, o3, no2, so2, co

const char* nombresZonas[ZONAS] = {"Cotocollao", "Carapungo", "Centro Histórico", "Tumbaco", "Guamani"};
const char* nombresContaminantes[CONTAMINANTES] = {"PM2.5", "PM10", "O3", "NO2", "SO2", "CO"};
const float limitesOMS[CONTAMINANTES] = {25.0, 50.0, 100.0, 40.0, 20.0, 4000.0}; // Límites de la OMS

typedef struct {
    float temperatura;
    float viento;
    float humedad;
} Clima;

typedef struct {
    float niveles[CONTAMINANTES]; // pm25, pm10, o3, no2, so2, co
} DiaContaminacion;

typedef struct {
    DiaContaminacion historico[DIAS];
    DiaContaminacion actual;
    Clima clima;
} Zona;

Zona zonas[ZONAS];

// Carga de datos reales CORREGIDOS
void cargarDatosReales() {
    // Datos de calidad del aire para cada zona - CORREGIDOS
    float datos[ZONAS][DIAS][CONTAMINANTES] = {
        // Cotocollao
        {
            {35, 31, 30, 25, 3, 6}, {38, 34, 24, 20, 3, 6}, {47, 24, 22, 18, 4, 5},
            {47, 23, 26, 22, 3, 3}, {48, 28, 19, 19, 4, 5}, {42, 29, 29, 24, 3, 4},
            {34, 26, 26, 21, 3, 2}, {38, 28, 28, 23, 3, 3}, {43, 20, 24, 20, 3, 4},
            {51, 26, 37, 28, 3, 4}, {52, 19, 25, 22, 3, 3}, {38, 23, 22, 19, 3, 3},
            {45, 19, 26, 24, 3, 2}, {37, 23, 27, 21, 4, 2}, {38, 12, 31, 26, 4, 3},
            {35, 23, 16, 18, 4, 6}, {45, 37, 36, 29, 4, 5}, {37, 20, 30, 25, 3, 1},
            {29, 51, 25, 22, 4, 4}, {27, 45, 28, 23, 3, 3}, {35, 32, 24, 21, 3, 4},
            {40, 28, 22, 19, 3, 3}, {33, 25, 26, 22, 4, 3}, {39, 30, 28, 24, 3, 4},
            {42, 27, 31, 26, 3, 3}, {36, 24, 29, 23, 3, 3}, {41, 31, 27, 25, 4, 4},
            {38, 26, 25, 21, 3, 3}, {44, 29, 33, 27, 4, 4}, {46, 32, 30, 26, 3, 4}
        },
        // Carapungo
        {
            {20, 31, 31, 18, 1, 4}, {35, 34, 21, 16, 1, 4}, {51, 24, 31, 23, 1, 3},
            {50, 23, 18, 15, 1, 3}, {42, 28, 17, 14, 2, 9}, {64, 29, 13, 12, 1, 7},
            {45, 26, 24, 21, 1, 6}, {46, 28, 28, 25, 1, 6}, {57, 20, 24, 22, 1, 7},
            {59, 26, 37, 34, 2, 7}, {73, 19, 28, 26, 1, 7}, {62, 23, 25, 23, 1, 7},
            {57, 19, 24, 22, 1, 7}, {42, 23, 25, 23, 1, 7}, {52, 12, 28, 26, 2, 7},
            {55, 23, 18, 16, 1, 9}, {52, 37, 33, 31, 2, 11}, {43, 20, 32, 29, 1, 7},
            {44, 51, 22, 20, 5, 24}, {50, 48, 25, 23, 2, 8}, {48, 35, 27, 25, 2, 6},
            {53, 40, 29, 27, 2, 7}, {47, 33, 24, 22, 1, 6}, {55, 38, 31, 28, 2, 8},
            {49, 29, 26, 24, 1, 6}, {52, 34, 28, 26, 2, 7}, {58, 41, 33, 30, 2, 9},
            {45, 27, 25, 23, 1, 5}, {51, 36, 30, 28, 2, 7}, {54, 39, 32, 29, 2, 8}
        },
        // Centro Histórico
        {
            {28, 35, 31, 28, 6, 4}, {28, 32, 25, 23, 8, 8}, {49, 45, 22, 20, 7, 6},
            {60, 55, 29, 27, 5, 5}, {56, 52, 22, 20, 4, 7}, {48, 44, 30, 28, 3, 3},
            {25, 22, 28, 26, 2, 4}, {37, 34, 29, 27, 3, 4}, {46, 42, 28, 26, 4, 6},
            {49, 45, 41, 38, 5, 5}, {51, 47, 26, 24, 4, 5}, {40, 37, 27, 25, 3, 5},
            {46, 42, 25, 23, 3, 4}, {41, 38, 32, 30, 4, 4}, {43, 40, 29, 27, 3, 5},
            {42, 39, 29, 27, 4, 6}, {38, 35, 34, 32, 2, 3}, {24, 21, 35, 33, 2, 4},
            {24, 21, 30, 28, 3, 5}, {29, 26, 27, 25, 2, 3}, {32, 29, 31, 29, 3, 4},
            {35, 32, 33, 31, 4, 5}, {27, 24, 26, 24, 2, 3}, {41, 38, 35, 33, 5, 6},
            {33, 30, 29, 27, 3, 4}, {39, 36, 32, 30, 4, 5}, {45, 42, 37, 35, 6, 7},
            {31, 28, 25, 23, 2, 3}, {37, 34, 30, 28, 3, 4}, {43, 40, 34, 32, 5, 6}
        },
        // Tumbaco
        {
            {22, 25, 26, 24, 1, 10}, {30, 33, 32, 30, 1, 10}, {28, 31, 30, 28, 1, 3},
            {29, 32, 36, 34, 1, 3}, {26, 29, 20, 18, 2, 5}, {40, 43, 29, 27, 2, 4},
            {30, 33, 29, 27, 1, 2}, {32, 35, 26, 24, 1, 2}, {36, 39, 34, 32, 1, 4},
            {32, 35, 44, 42, 2, 4}, {32, 35, 33, 31, 1, 4}, {20, 23, 33, 31, 1, 5},
            {21, 24, 31, 29, 1, 5}, {25, 28, 34, 32, 2, 5}, {27, 30, 36, 34, 1, 6},
            {20, 23, 28, 26, 1, 8}, {34, 37, 42, 40, 2, 8}, {20, 23, 30, 28, 2, 9},
            {20, 23, 32, 30, 3, 11}, {25, 28, 29, 27, 2, 5}, {28, 31, 33, 31, 2, 6},
            {24, 27, 31, 29, 1, 4}, {30, 33, 35, 33, 2, 7}, {26, 29, 28, 26, 1, 3},
            {33, 36, 40, 38, 2, 8}, {23, 26, 26, 24, 1, 4}, {29, 32, 34, 32, 2, 6},
            {31, 34, 37, 35, 2, 7}, {27, 30, 31, 29, 1, 5}, {35, 38, 39, 37, 2, 8}
        },
        // Guamani
        {
            {41, 44, 34, 32, 1, 2}, {27, 30, 23, 21, 2, 5}, {49, 52, 23, 21, 2, 7},
            {65, 68, 23, 21, 2, 5}, {52, 55, 23, 21, 2, 4}, {40, 43, 34, 32, 1, 3},
            {38, 41, 30, 28, 2, 4}, {41, 44, 30, 28, 2, 5}, {52, 55, 30, 28, 2, 4},
            {47, 50, 50, 48, 2, 6}, {59, 62, 29, 27, 2, 5}, {45, 48, 31, 29, 2, 6},
            {54, 57, 26, 24, 2, 5}, {54, 57, 35, 33, 2, 5}, {58, 61, 35, 33, 2, 7},
            {46, 49, 36, 34, 1, 5}, {34, 37, 28, 26, 1, 3}, {20, 23, 25, 23, 1, 2},
            {35, 38, 32, 30, 2, 4}, {42, 45, 37, 35, 2, 6}, {38, 41, 29, 27, 1, 3},
            {48, 51, 35, 33, 2, 5}, {44, 47, 31, 29, 2, 4}, {51, 54, 38, 36, 2, 6},
            {39, 42, 27, 25, 1, 3}, {45, 48, 33, 31, 2, 5}, {53, 56, 40, 38, 2, 7},
            {41, 44, 30, 28, 1, 4}, {47, 50, 36, 34, 2, 6}, {49, 52, 38, 36, 2, 5}
        }
    };

    for (int z = 0; z < ZONAS; z++) {
        for (int d = 0; d < DIAS; d++) {
            for (int c = 0; c < CONTAMINANTES; c++) {
                zonas[z].historico[d].niveles[c] = datos[z][d][c];
            }
        }
        // Cargar datos actuales (último día)
        for (int c = 0; c < CONTAMINANTES; c++) {
            zonas[z].actual.niveles[c] = datos[z][DIAS - 1][c];
        }
        // Cargar clima simulado
        zonas[z].clima.temperatura = 20 + rand() % 10; // Temperatura entre 20 y 30
        zonas[z].clima.viento = 5 + rand() % 10; // Viento entre 5 y 15
        zonas[z].clima.humedad = 40 + rand() % 20; // Humedad entre 40 y 60
    }
}
void exportarReporte() {
    FILE* f = fopen("reporte_contaminacion.txt", "w");
    if (!f) {
        printf("Error: No se pudo crear el archivo de reporte.\n");
        return;
    }

    fprintf(f, "REPORTE DE CALIDAD AIRE\n\n");
    
    for (int z = 0; z < ZONAS; z++) {
        fprintf(f, "Zona: %s\n", nombresZonas[z]);
        fprintf(f, "Clima: Temp: %.1fC, Viento: %.1f km/h, Humedad: %.1f%%\n", zonas[z].clima.temperatura, zonas[z].clima.viento, zonas[z].clima.humedad);
        fprintf(f, "Niveles actuales:\n");
        
        for (int c = 0; c < CONTAMINANTES; c++) {
            fprintf(f, "  %s: %.2f  ", nombresContaminantes[c], zonas[z].actual.niveles[c]);
            if (zonas[z].actual.niveles[c] > limitesOMS[c]) {
                fprintf(f, "(SUPERA LIMITE)\n");
            } else {
                fprintf(f, "\n");
            }
        }
        
    }
    fclose(f);
}

int main() {
    int opcionZona, opcionAccion;
    int salir = 0;

    cargarDatosReales();

    printf("SISTEMA DE MONITOREO DE CALIDAD DEL AIRE - QUITO\n");

    while (!salir) {
        printf("\nSeleccione una zona:\n");
        for (int i = 0; i < ZONAS; i++) {
            printf("%d. %s\n", i + 1, nombresZonas[i]);
        }
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcionZona);

        if (opcionZona == 0) {
            salir = 1;
            break;
        }
        if (opcionZona < 1 || opcionZona > ZONAS) {
            printf("Zona invalida. Intente de nuevo.\n");
            continue;
        }

        int z = opcionZona - 1;

        printf("\nZona seleccionada: %s\n", nombresZonas[z]);
        printf("Seleccione una accion:\n");
        printf("1. Ver promedios historicos\n");
        printf("2. Ver prediccion de contaminacion\n");
        printf("3. Ver recomendaciones\n");
        printf("4. Ver clima y niveles actuales\n");
        printf("5. Exportar reporte completo\n");
        printf("0. Volver al menu de zonas\n");
        printf("Opcion: ");
        scanf("%d", &opcionAccion);

        switch (opcionAccion) {
            case 1:
                printf("\nPROMEDIOS HISTORICOS DE %s\n", nombresZonas[z]);
                for (int c = 0; c < CONTAMINANTES; c++) {
                    float suma = 0;
                    int diasValidos = 0;
                    for (int d = 0; d < DIAS; d++) {
                        if (zonas[z].historico[d].niveles[c] > 0) {
                            suma += zonas[z].historico[d].niveles[c];
                            diasValidos++;
                        }
                    }
                    if (diasValidos > 0) {
                        float promedio = suma / diasValidos;
                        printf("%s Promedio: %.2f ", nombresContaminantes[c], promedio);
                            if (promedio > limitesOMS[c]) {
                                printf("(Supera limite OMS)\n");
                            } else {
                                printf("(Dentro del limite)\n");
                            }
                    } else {
                        printf("%s: Sin datos validos\n", nombresContaminantes[c]);
                    }
                }
                break;
            case 2:
                printf("\nPREDICCION DE %s\n", nombresZonas[z]);
                for (int c = 0; c < CONTAMINANTES; c++) {
                    float pred = 0;
                    float sumaPesos = 0;
                    for (int d = DIAS - 5; d < DIAS; d++) {
                        if (zonas[z].historico[d].niveles[c] > 0) {
                            float peso = (d - (DIAS - 6)) + 1;
                            pred += zonas[z].historico[d].niveles[c] * peso;
                            sumaPesos += peso;
                        }
                    }
                    if (sumaPesos > 0) {
                        pred /= sumaPesos;
                        printf("%s Predicho: %.2f ", nombresContaminantes[c], pred);
                            if (pred > limitesOMS[c]) {
                                printf("(¡ALERTA!)\n");
                            } else {
                                printf("(Normal)\n");
                            }
                    } else {
                        printf("%s: Sin datos suficientes\n", nombresContaminantes[c]);
                    }
                }
                break;
            case 3: {
                int alertaCount = 0;
                for (int c = 0; c < CONTAMINANTES; c++) {
                    if (zonas[z].actual.niveles[c] > limitesOMS[c]) alertaCount++;
                }
                printf("\nRECOMENDACIONES PARA %s\n", nombresZonas[z]);
                printf("NIVEL DE ALERTA:\n");
                    if(alertaCount >= 3){
                        printf("ALTO\n");
                        printf("- Evitar actividades físicas intensas al aire libre\n");
                        printf("- Mantener ventanas cerradas\n");
                        printf("- Usar mascarilla al salir\n");
                    }
                    if (alertaCount == 2) {
                        printf("MEDIO\n");
                        printf("- Aplicar restricciones vehiculares\n");
                        printf("- Supervisar emisiones industriales\n");
                    }
                } else {
                    printf("- Calidad del aire: BUENA\n");
                    printf("- Se pueden realizar actividades al aire libre\n");
            }
            case 4:
                printf("\nClima en %s: Temp: %.1f°C, Viento: %.1f km/h, Humedad: %.1f%%\n", 
                    nombresZonas[z],
                    zonas[z].clima.temperatura,
                    zonas[z].clima.viento,
                    zonas[z].clima.humedad);
                printf("Niveles actuales:\n");
                for (int c = 0; c < CONTAMINANTES; c++) {
                    printf("  %s: %.2f μg/m³ %s\n",
                        nombresContaminantes[c],
                        zonas[z].actual.niveles[c],
                        zonas[z].actual.niveles[c] > limitesOMS[c] ? "(SUPERA LÍMITE)" : "");
                }
                break;
            case 5:
                exportarReporte();
                break;
            case 0:
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    }

    printf("\nGracias por usar el sistema.\n");
    return 0;
}
