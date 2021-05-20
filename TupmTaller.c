// LIBRERIAS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// Declaración funciones

int menuA(); // Menu autenticación de los usuarios

int menu(); // Menu normal de la app

void header(); // Header de la app

void delay(int); // Delay para ciertos momentos

void LeeCadena(char [], FILE*, int); /* Función para sustituir el \n que se pone al final de una cadena al leerla con la funcion fgets,
									 por un \0  para no tener ese "intro" no deseado a la hora de imprimir la cadena con un printf */

int comprar(int, int, int, int); // Funcion comprar

int canjear(int, int);

// Estructuras guardado de datos

struct TUser{
	char nick[15]; // Nickname o Username
	char nombre[20]; // Nombre del Usuario
	char ape1[30]; // Primer apellido del Usuario
	char ape2[30]; // Segundo apellido del Usuario
	char tlf[10]; // Numero de tlf o movil
	char mat[8]; // Matrícula del coche del usuario
	char pass[16]; // Contraseña del Usuario
	int puntos; // Puntos usuario
	int cup10; // Por si tiene cupon de 10% descuento o no
	int cup25; // Por si tiene cupon de 25% descuento o no
}; // Estructura datos usuarios

struct TRepair {
	int nref; // Numero de referencia para indicar la compra
	char repnom[100]; // Nombre de la reparación
	int precio; // Precio de la reparación
}; // Estructura datos reparaciones

struct TCupon {
	char cupnom[100]; // Nombre del cupón
	int points; // Puntos necesarios para adquirir el cupón
}; // Estructura datos Cupones
   
// MAIN

int main() {
	setlocale(0, ""); // Set de la región local, en este caso España para poder utilizar acentos y la ñ

	// Declaración de todas las variables

	struct TUser users[100];
	struct TRepair reps[100];
	struct TCupon cup[100];

	int op, op1, nUser = 0, nUserLogin = 0, nrep = 0, ncup = 0, test = 0, selector = 1, i, passlen, numRef, numCupon;
	char nickLogin[15], pass[16], nickRegister[15];

	// GUARDADO DE LA INFORMACION DE LOS USUARIOS, LAS REPARACIONES Y LOS CUPONES EN EL PROGRAMA DESDE EL FICHERO
	// Usuarios
	FILE * finput;
	finput = fopen("usuarios.txt", "r");
	if (finput == NULL) {
		printf("Error al abrir fichero de usuarios\n");
		return 0;
	}

	while (fscanf(finput, "%s %s %s %s %s %s %s %d %d %d", users[nUser].nick, users[nUser].nombre, users[nUser].ape1, users[nUser].ape2, users[nUser].tlf, users[nUser].mat, users[nUser].pass, &users[nUser].puntos, &users[nUser].cup10, &users[nUser].cup25) != EOF) {
		nUser++;
	} // Hasta "EOF" (End Of File) va cargando uno a uno los usuarios y guardándolos en la estructura correspondiente

	fclose(finput);

	// Reparaciones
	FILE * frep;
	frep = fopen("repairs.txt", "r");
	if (frep == NULL) {
		printf("Error al abrir fichero de reparaciones.\n");
		return 0;
	}
	
	for (i = 0; i < 10; i++) {
		fscanf(frep, "%d", &reps[i].nref);
		LeeCadena(reps[i].repnom, frep, 100);
		fscanf(frep, "%d", &reps[i].precio);
		nrep++;
	}
	fclose(frep);

	// Cupones
	FILE* fcup;
	fcup = fopen("cupones.txt", "r");
	if (fcup == NULL) {
		printf("Error al abrir el fichero cupones.\n");
		return 0;
	}

	for (i = 0; i < 10; i++) {
		fscanf(fcup, "%d", &cup[i].points);
		LeeCadena(cup[i].cupnom, fcup, 100);
		ncup++;
	}
	
	// Comienzo del programa en sí

	while (selector == 1) {
		op = menuA();
		switch (op) {
			case 1: // Logeo o Inicio de Sesión
				do {
					header();
					test = 0;
					printf("\nIntroduce tu nombre de usuario (Nick): ");
					scanf("%s", nickLogin);
					fflush(stdin);
					printf("\nIntroduce tu contraseña: ");
					scanf("%s", pass);
					fflush(stdin);
					for (i = 0; i < nUser; i++) {
						if (strcmp(users[i].nick, nickLogin) == 0 && strcmp(users[i].pass, pass) == 0) {
							nUserLogin = i;
							header();
							printf("\nUsuario y contraseña correctos, éxito al iniciar sesión...\n");
							test = 1;
							delay(2);
							break;
						}
					}
					if (test == 0) {
						printf("\nEl usuario y contraseña introducidos no coinciden, porfavor vuelva a intentarlo.\n");
						delay(2);
					}
				} while (strcmp(users[i].nick, nickLogin) != 0 || strcmp(users[i].pass, pass) != 0);

				op1 = menu(users[nUserLogin].nick);
				selector = 0;

				while (selector == 0) {
					switch (op1) {
						case 1: // Comprar
							header();
							printf("\n");
							printf("+-------+---------------------------------------------+----------+\n");
							printf("|  Ref  | Reparaciones/ Sustituciones/ Mantenimientos |  Precio  |\n");
							printf("+-------+---------------------------------------------+----------+\n");
							for (i = 0; i < nrep - 1; i++) {
								printf("|   %*d|%*s|%*d %c|\n", -4, reps[i].nref, -45, reps[i].repnom, -8, reps[i].precio, 36);
							}
							printf("+-------+---------------------------------------------+----------+\n");
							
							do {
								printf("Indique el número de referencia del prodcuto que quiere adquirir:\n");
								scanf("%d", &numRef);

								if (numRef < reps[0].nref || numRef > reps[nrep - 2].nref) {
									printf("\nEl número de referencia no existe, porfavor introduzca uno válido.\n\n");
								}
							} while (numRef < reps[0].nref || numRef > reps[nrep - 2].nref);

							for (i = 0; i < nrep - 1; i++) {
								if (numRef == reps[i].nref) {
									numRef = i;
									break;
								}
							}
							// Funcion de comprar
							users[nUserLogin].puntos = comprar(reps[numRef].nref, users[nUserLogin].puntos, users[nUserLogin].cup10, users[nUserLogin].cup25);
							if (reps[numRef].nref == 5 && users[nUserLogin].cup25 > 0) {
								users[nUserLogin].cup25 -= 1;
							}
							else if (reps[numRef].nref == 6 && users[nUserLogin].cup10 > 0) {
								users[nUserLogin].cup10 -= 1;
							}
							op1 = menu(users[nUserLogin].nick);
							break;

						case 2: // Mostrar Puntos Usuario
							header();
							printf("\nTus puntos: %d\n\n", users[nUserLogin].puntos);
							system("pause");
							op1 = menu(users[nUserLogin].nick);
							break;

						case 3: // Canjear Puntos
							header();
							printf("\n");
							printf("+-------+-------------------------------------------------------------+--------+\n");
							printf("|  Ref  |                           Cupón                             | Puntos |\n");
							printf("+-------+-------------------------------------------------------------+--------+\n");
							for (i = 0; i < ncup - 3; i++) {
								printf("|   %*d|%*s|%*d p|\n", -4, i + 1, -61, cup[i].cupnom, -6, cup[i].points);
							}
							printf("+-------+-------------------------------------------------------------+--------+\n");

							do {
								printf("Indique el número del cupón que quiere\n");
								scanf("%d", &numCupon);

								if (1 < reps[0].nref || numCupon > 7) {
									printf("\nEl número de cupón no existe, porfavor introduzca uno válido.\n\n");
								}
							} while (numCupon < 1 || numCupon > 7);

							if (numCupon == 3) {
								users[nUserLogin].cup10 += 1;
							}
							else if (numCupon == 6) {
								users[nUserLogin].cup25 += 1;
							}

							users[nUserLogin].puntos = canjear(numCupon, users[nUserLogin].puntos);

							system("pause");
							op1 = menu(users[nUserLogin].nick);
							break;
						
						case 4: // Mi cuenta
							header();
							printf("\n MI CUENTA \n");
							printf("\t - Nickname: %s\n", users[nUserLogin].nick);
							printf("\t - Nombre: %s\n", users[nUserLogin].nombre);
							printf("\t - Apellidos: %s %s\n", users[nUserLogin].ape1, users[nUserLogin].ape2);
							printf("\t - Móvil o tlf: %s\n", users[nUserLogin].tlf);
							printf("\t - Matrícula de su vehículo: %s\n", users[nUserLogin].mat);
							printf("\t - Puntos acumulados: %d\n\n", users[nUserLogin].puntos);
							system("pause");
							op1 = menu(users[nUserLogin].nick);
							break;

						case 5: // Logout
							selector = 1;
							break;

						default: // Caso default
							printf("Introduzca un número correcto.");
							break;
					}
				}

				break;
			
			case 2: // Registro de usuario
				do {
					header();
					// Comprobación de si el nombre de usuario ya está en uso
					printf("Introduzce un nombre de usuario (Nick): ");
					scanf("%s", nickRegister);
					for (i = 0; i < nUser; i++) {
						if (strcmp(users[i].nick, nickRegister) == 0) {
							printf("\nEl nombre de usuario introducido ya está en uso, porfavor introduzce otro\n");
							delay(2);
							break;
						}
					}
				} while (strcmp(users[i].nick, nickRegister) == 0);

				if (strcmp(users[i].nick, nickRegister) != 0) {
					strcpy(users[nUser].nick, nickRegister); // Guardado del nombre de usuario
				}
				fflush(stdin);
				while (getchar() != '\n');
				printf("\nIntroduce tu nombre: ");
				gets(users[nUser].nombre);
				printf("\nIntroduce tus apellidos: ");
				scanf("%s %s", users[nUser].ape1, users[nUser].ape2);
				while (getchar() != '\n');
				printf("\nIntroduce tu número de tlf o móvil: ");
				gets(users[nUser].tlf);
				printf("\nIntroduce la matrícula de tu vehículo: ");
				gets(users[nUser].mat);
				fflush(stdin);
				do {
					// Comprobar que la contraseña introducida tiene más de 8 caracteres
					printf("\nIntroduce una contraseña: ");
					gets(users[nUser].pass);
					passlen = strlen(users[nUser].pass); // Guardado de la longitud de la cadena
					if (passlen < 8) { 
						printf("\nLa contraseña debe contener 8 caracteres como mínimo.");
					}
				} while (passlen < 8);
				users[nUser].puntos = 0;
				users[nUser].cup10 = 0;
				users[nUser].cup25 = 0;
				nUser++;
				header();
				printf("\nSe ha registrado con éxito!\n");
				printf("\nHas iniciado sesión correctamente.\n");
				delay(3);
				op1 = menu(users[nUser - 1].nick);
				selector = 0;

				while (selector == 0) {
					switch (op1) {
					case 1: // Comprar
						header();
						printf("\n");
						printf("+-------+---------------------------------------------+----------+\n");
						printf("|  Ref  | Reparaciones/ Sustituciones/ Mantenimientos |  Precio  |\n");
						printf("+-------+---------------------------------------------+----------+\n");
						for (i = 0; i < nrep - 1; i++) {
							printf("|   %*d|%*s|%*d %c|\n", -4, reps[i].nref, -45, reps[i].repnom, -8, reps[i].precio, 36);
						}
						printf("+-------+---------------------------------------------+----------+\n");
						
						do {
							printf("Indique el número de referencia del prodcuto que quiere adquirir:\n");
							scanf("%d", &numRef);

							if (numRef < reps[0].nref && numRef > reps[nrep].nref) {
								printf("\nEl número de referencia no existe, porfavor introduzca uno válido.\n");
							}
						} while (numRef < reps[0].nref && numRef > reps[nrep].nref);

						for (i = 0; i < nrep - 1; i++) {
							if (numRef == reps[i].nref) {
								numRef = i;
								break;
							}
						}
						// Funcion de comprar
						users[nUser - 1].puntos = comprar(reps[numRef].nref, users[nUser - 1].puntos, users[nUser - 1].cup10, users[nUser - 1].cup25);
						if (reps[numRef].nref == 5 && users[nUser - 1].cup25 > 0) {
							users[nUser - 1].cup25 -= 1;
						}
						else if (reps[numRef].nref == 6 && users[nUser - 1].cup10 > 0) {
							users[nUser - 1].cup10 -= 1;
						}
						op1 = menu(users[nUser - 1].nick);
						break;

					case 2: // Mostrar Puntos Usuario
						header();
						printf("\nTus puntos: %d\n", users[nUser - 1].puntos);
						system("pause");
						op1 = menu(users[nUser - 1].nick);
						break;

					case 3: // Canjear Puntos
						header();
						printf("\n");
						printf("+-------+-------------------------------------------------------------+--------+\n");
						printf("|  Ref  |                           Cupón                             | Puntos |\n");
						printf("+-------+-------------------------------------------------------------+--------+\n");
						for (i = 0; i < ncup - 3; i++) {
							printf("|   %*d|%*s|%*d p|\n", -4, i + 1, -61, cup[i].cupnom, -6, cup[i].points);
						}
						printf("+-------+-------------------------------------------------------------+--------+\n");

						do {
							printf("Indique el número del cupón que quiere\n");
							scanf("%d", &numCupon);

							if (1 < reps[0].nref || numCupon > 7) {
								printf("\nEl número de cupón no existe, porfavor introduzca uno válido.\n\n");
							}
						} while (numCupon < 1 || numCupon > 7);

						if (numCupon == 3) {
							users[nUser - 1].cup10 += 1;
						}
						else if (numCupon == 6) {
							users[nUser - 1].cup25 += 1;
						}

						users[nUser - 1].puntos = canjear(numCupon, users[nUser - 1].puntos);

						system("pause");
						op1 = menu(users[nUser - 1].nick);
						break;

					case 4: // Mi cuenta
						header();
						printf("\n MI CUENTA \n");
						printf("\t - Nickname: %s\n", users[nUser - 1].nick);
						printf("\t - Nombre: %s\n", users[nUser - 1].nombre);
						printf("\t - Apellidos: %s %s\n", users[nUser - 1].ape1, users[nUser - 1].ape2);
						printf("\t - Móvil o tlf: %s\n", users[nUser - 1].tlf);
						printf("\t - Matrícula de su vehículo: %s\n", users[nUser - 1].mat);
						printf("\t - Puntos acumulados: %d\n\n", users[nUser - 1].puntos);
						system("pause");
						op1 = menu(users[nUser - 1].nick);
						break;

					case 5: // Logout
						selector = 1;
						break;

					default: // Caso default
						printf("Introduzca un número correcto.");
						break;
					}
				}

				break;
			
			case 3: // Cerrar o Salir del programa
				printf("\nSaliendo del programa...\n\n");

				// Reescribir el fichero de usuarios.txt para guardar los posibles nuevos usuarios
				FILE* pfinput;
				pfinput = fopen("usuarios.txt", "w");

				for (i = 0; i < nUser; i++) {
					fprintf(pfinput, "%s %s %s %s %s %s %s %d %d %d\n", users[i].nick, users[i].nombre, users[i].ape1, users[i].ape2, users[i].tlf, users[i].mat, users[i].pass, users[i].puntos, users[i].cup10, users[i].cup25);
				}
				fclose(pfinput);
				return 0;
				break;
			
			default: // Caso default
				printf("Introduzca un número correcto.");
				break;
		}
	}
	return 0;
}

int menuA() {
	int op;

	do {
		header();
		printf("\n");
		printf("¿Tienes ya cuenta?\n\n");
		printf("\t 1. Sí, logeate!\n\t 2. No, regístrate...\n\t 3. Salir\n\n");
		fflush(stdin);
		scanf("%d", &op);
	} while (op != 1 && op != 2 && op != 3);

	return op;

}

int menu(char nick[]) {
	int op;

	do {
		header();
		printf("\n");
		printf("Bienvenido %s, qué deseas hacer?\n\n", nick);
		printf("\t 1. Comprar\n\t 2. Mis Puntos\n\t 3. Canjear Puntos\n\t 4. Mi cuenta\n\t 5. Logout\n\n");
		fflush(stdin);
		scanf("%d", &op);
	} while (op != 1 && op != 2 && op != 3 && op != 4 && op != 5);

	return op;

}

void header() {
	system("cls");
	printf("  ______                         _________    __    __    __________  \n");
	printf(" /_  __/_  ______  ____ ___     /_  __/   |  / /   / /   / ____/ __ %c \n", 92);
	printf("  / / / / / / __ %c/ __ `__ %c     / / / /| | / /   / /   / __/ / /_/ / \n", 92, 92);
	printf(" / / / /_/ / /_/ / / / / / /    / / / ___ |/ /___/ /___/ /___/ _, _/  \n");
	printf("/_/  %c__,_/ .___/_/ /_/ /_/    /_/ /_/  |_/_____/_____/_____/_/ |_|   \n", 92);
	printf("         /_/                                                          \n\n\n");
}

void delay(int secs) {
	int i;
	for (i = (time(NULL) + secs); time(NULL) != i; time(NULL));
}

void LeeCadena(char cadena[], FILE *frep, int max)
{
	char* p;
	int n;
	p = fgets(cadena, max, frep);
	if (p != NULL)
	{
		n = strlen(cadena);
		if (n > 0 && cadena[n - 1] == '\n')
			cadena[n - 1] = '\0';
	}
}

int comprar(int reps, int puntos, int cup10, int cup25) {
	header();
	if (reps == 1) {
		puntos += 22;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Sustitución de la Suspensión' se ha realizado con éxito!\nHa obtenido 22 puntos con su compra.\n\n");
	}
	else if (reps == 2) {
		puntos += 9;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Sustitución de la Batería' se ha realizado con éxito!\nHa obtenido 9 puntos con su compra.\n\n");
	}
	else if (reps == 3) {
		puntos += 18;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Kit de Distribución Completo' se ha realizado con éxito!\nHa obtenido 18 puntos con su compra.\n\n");
	}
	else if (reps == 4) {
		puntos += 2;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Cambio de Escobillas Limpiaparabrisas' se ha realizado con éxito!\nHa obtenido 2 puntos con su compra.\n\n");
	}
	else if (reps == 5) {
		puntos += 25;
		printf("Procesando su compra...\n\n");
		delay(2);
		if (cup25 != 0) {
			printf("Aplicando su cupón de 25 %c de descuento, se ha ahorrado 62,5 $ y el precio final de la compra es de 187,5 $ !!! \n", 37);
		}
		printf("Su compra de 'Cambio de Neumáticos' se ha realizado con éxito!\nHa obtenido 25 puntos con su compra.\n\n");
	}
	else if (reps == 6) {
		puntos += 8;
		printf("Procesando su compra...\n\n");
		delay(2);
		if (cup10 != 0) {
			printf("Aplicando su cupón de 10 %c de descuento, se ha ahorrado 8,5 $ y el precio final de la compra es de 76,5 $ !!! \n", 37);
		}
		printf("Su compra de 'Revisión Completa' se ha realizado con éxito!\nHa obtenido 8 puntos con su compra.\n\n");
	}
	else if (reps == 7) {
		puntos += 2;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Lavado Completo Interior y Exterior' se ha realizado con éxito!\nHa obtenido 2 puntos con su compra.\n\n");
	}
	else if (reps == 8) {
		puntos += 7;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Cambio de Pastilla de Freno' se ha realizado con éxito!\nHa obtenido 7 puntos con su compra.\n\n");
	}
	else if (reps == 9) {
		puntos += 5;
		printf("Procesando su compra...\n\n");
		delay(2);
		printf("Su compra de 'Alineación y Equilibrado' se ha realizado con éxito!\nHa obtenido 5 puntos con su compra.\n\n");
	}
	system("pause");
	return puntos;
}

int canjear(int num, int points) {
	if (num == 1) {
		if (points >= 25) {
			printf("\nHas canjeado 25 puntos por 'Lavado gratis' con éxito!!!\n\n");
			points -= 25;

		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}
	else if (num == 2) {
		if (points >= 30) {
			printf("\nHas canjeado 30 puntos por '50 $ en combustible en cualquier gasolinera' con éxito!!!\n\n");
			points -= 30;
		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}
	else if (num == 3) {
		if (points >= 50) {
			printf("\nHas canjeado 50 puntos por '10 %c de descuento en tu próxima revisión' con éxito!!!\n\n", 37);
			points -= 50;
		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}
	else if (num == 4) {
		if (points >= 100) {
			printf("\nHas canjeado 100 puntos por 'Camiseta oficial de tu equipo favorito de La Liga Santander' con éxito!!!\n\n");
			points -= 100;
		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}
	else if (num == 5) {
		if (points >= 150) {
			printf("\nHas canjeado 150 puntos por '4 entradas para el Parque de Atracciones de Madrid' con éxito!!!\n\n");
			points -= 150;
		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}
	else if (num == 6) {
		if (points >= 200) {
			printf("\nHas canjeado 200 puntos por '25 %c en tu próximo cambio de neumáticos' con éxito!!!\n\n", 37);
			points -= 200;
		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}
	else if (num == 7) {
		if (points >= 500) {
			printf("\nHas canjeado 500 puntos por 'Moto marca DERBI' con éxito!!!\n\n");
			points -= 500;
		}
		else {
			printf("\nNo tienes puntos suficientes para canjear este cupón\n\n");
		}
	}

	return points;
}
