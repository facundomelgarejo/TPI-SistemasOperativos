#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <cctype>
#include <iostream>

//Ambiente
//Generamos las estructuras necesarias
struct Proceso{
	int id;
	int tamano;
	int TA;
	int TI;
	int dirInicio;
	bool estado;
	int TIRes; //TE = TR - TI
	int TR; // INSTNTE RETORNO - TA
	struct Proceso *proximo;
} *A, *M, *PrLS, *PrL, *PrSB, *PrB, *PrT, *MLS, *MT, *ML, *Mo, *An, *CPU, *PrN, *ULS, *aux;
/*
A: puntero al Anterior (Revisar si se ocupa)
M: puntero que se mueve (Revisar si se ocupa)
PrLS: puntero al primer elemento de: LISTO Y SUSPENDIDO
PrL: puntero al primer elemento de: LISTO (PROCESOS ASIGNADOS A PARTICIONES)
ML: Puntero para moverse en: Lista de LISTO
MLS: Puntero para moverse en: Lista de LISTO Y SUSPENDIDO
MT: Puntero para moverse en: Lista de TERMINADO
CPU: Puntero a proceso ejecutandose
PrT: Puntero al primer elemento de: Lista de TERMINADO
PrN: Puntero al primer elemento de: Lista de NUEVOS
ULS: Puntero al ultimo elemento de la lista de LISTOS Y SUSPENDIDOS
*/

struct Particiones{
	int id;
	int dirInicio;
	int tamano;
	int id_Proc;
	int FI;
}Par[4];

int MemDisp;
int c;
int i;
int m, te, control, n;
//INICIO
bool estadoCPU = false;
struct Proceso *creanodo(void) {
  return (struct Proceso *) malloc(sizeof(struct Proceso));
}

//SJF
void agregarElemento(Proceso *&prim, Proceso *&nodo){ //Agrega un elemento a alguna cola
	//prim: primer elemento de una cola
	//nodo: elemento a agregar
	struct Proceso *act;
	struct Proceso *ant;
	if (prim == NULL){
		prim = nodo;
		nodo->proximo = NULL;
			
		}else{																				
			act = prim;
			ant = NULL;
			while ((act != NULL) && (nodo->TI >= act->TI)){
				ant = act;
				act = act->proximo;
			}
			if (prim == act){ //Se carga al comienzo
				nodo->proximo = act;
				prim = nodo;
			}else {
				ant->proximo= nodo;
				nodo->proximo= act;
			}
	}
}

void imprimirListaLS(){
	M = PrLS;
	printf("Los procesos en la lista de LISTOS Y SUSPENDIDOS son: \n");
	while (M != NULL){
		printf("Proceso id: %i\n", M->id);
		M = M->proximo;
	}
}

bool HayLugar(Proceso *mov){
	bool hay = false;
	for (i = 1; i < 4; i++){
		if (Par[i].id_Proc == 0  && Par[i].tamano >= mov->tamano){
			hay = true;
		}
	}
	return(hay);
}

int bestFit(){
	int FI;
	int pos = 0;
	int min = 999;
	for (i = 1; i < 4; i++){
		if (MLS->tamano <= Par[i].tamano ){
			if ((Par[i].id_Proc == 0) && (MLS->estado == false)){//control particion vacia y estado sin asignar
				FI = Par[i].tamano - MLS->tamano;
				if (FI < min){
					min = FI;
					pos = i;
				}
			}
		}
	}
	return(pos);
}

void asignarParticion(Proceso *nodo, int pos){
	Par[pos].id_Proc = nodo->id;
	Par[pos].FI = Par[pos].tamano - nodo->tamano;
	nodo->estado = true;
}

void suspenderProceso(Proceso *prim, Proceso *nodo, int pos){
	struct Proceso *act;
	struct Proceso *ant;
	//Cargo a la lista de SUSPENDIDOS correspondiente
	if (prim == NULL){
		prim = nodo; //Lista vacia
	}else{
		act = prim;
		ant = NULL;
		while (act != NULL && nodo->TA > act->TA){
			ant = act;
			act = act->proximo;
		}
		if (PrN == M){ //Se carga al comienzo
			nodo->proximo = act;
			prim = nodo;
		}else {
			ant->proximo = nodo;
			nodo->proximo = act;
		}
	}
	//Devuelvo particion
	Par[pos].id_Proc = 0;
	Par[pos].FI = 0;
	nodo->estado = false;
}

char s[20];

int main (){
	//Definimos el tamaÃ±o de las particiones
	for (i = 0; i < 4; i++){
		Par[i].tamano = 0;
		Par[i].FI = 0;
		Par[i].id = 0;
	}
	Par[0].tamano = 100;
	Par[1].tamano = 250;
	Par[2].tamano = 120;
	Par[3].tamano = 60;
	
	//Asignamos los id de particiones y sus direcciones de inicio
	c = 0;
	for(i=0 ; i < 4; i++){
		Par[i].id = i;
		Par[i].dirInicio = c;
		c = c + Par[i].tamano;
		Par[i].id_Proc = 0;
	}
	MemDisp = 0; //Calculamos tamaÃ±o total de memoria principal
	for (i = 0; i < 4; i++){
		MemDisp = MemDisp + Par[i].tamano;
	}
	
	//Obtenemos la lista de procesos
	while (n <= 0){
		printf("Â¿Cuantos procesos son?\n");
		scanf("%s",&s);
		n = atoi(s);
		system("cls");
	}
	int TamControl=0;
	PrLS = NULL;
	PrN = NULL;
	int numero = 0;
	bool ctrl = false;
	control = 0;
	for (i = 1; i <= n; i++){
		//printf("------------------------------------------\n");
		TamControl = 0;
		while (TamControl <= 0){
			printf("Ingrese el tamaÃ±o del proceso %i\n ", i);
			scanf("%s",&s);
			TamControl = atoi(s);
			system("cls");
		}
		
		if (TamControl<251){
			struct Proceso *p;
			p = creanodo();
			p->id = i;
			p->tamano=TamControl;	
			while (numero <= 0){
				printf("Ingrese el tiempo de arribo del proceso %i\n ", i);
				scanf("%s",&s);
				numero = atoi(s);
				if (s[0] == '0'){
					numero = 1;
					ctrl = true;
				}
				system("cls");
			}
			if (ctrl == true){
				numero = 0;
				ctrl = false;	
			}
			p->TA = numero;
			numero = 0;
			while (numero <= 0){
				printf("Ingrese el tiempo de irrupcion del proceso %i\n ", i);
				scanf("%s",&s);
				numero = atoi(s);
				system("cls");
			}
			p->TI = numero;
			numero = 0;
			p->estado = 0;
			p->proximo=NULL; //FALTABA ESTO
			p->TIRes = p->TI;
			p->TR = 0;
			printf("id: %i | tamaÃ±o: %i | TA: %i | Ti: %i\n", p->id, p->tamano, p->TA, p->TI);
			if (PrN == NULL){
				PrN = p; //Lista vacia
			}else{
				M = PrN;
				A = NULL;
				while (M != NULL && p->TA > M->TA){
					A = M;
					M = M->proximo;
				}
				if (PrN == M){ //Se carga al comienzo
					p->proximo = M;
					PrN = p;
				}else {
					A->proximo = p;
					p->proximo = M;
				}
			}
    	} else{
     	printf("\n No hay suficiente memoria para ejecutar este proceso\n");
    	control ++; // para que no entre al while principal
		}
		system("pause");
		system("cls");
	}
	n = n - control;
	
	// Imprimimos la tabla de particiones
	M = PrN;
	A = NULL;
	CPU = NULL;
	PrL = NULL;
	MLS = NULL;
	int instante =-1;
	int cont = 0;
	bool encabezado;
	MT = PrT;
	int pos = 0;
	
	while (cont != n){
		encabezado = false;
		instante ++;
		printf("\n-----------------------------\tInstante %i\t--------------------------------\n\n", instante);
		M  = PrN;
		while (M != NULL && M->TA == instante){
			if (HayLugar(M) == true){ //Carga cola de LISTOS, pasa de la cola de NUEVOS a la de LISTOS
				int FI;
				int pos = 0;
				int min = 999;
				for (i = 1; i < 4; i++){
					if (M->tamano <= Par[i].tamano ){
						if ((Par[i].id_Proc == 0) && (M->estado == false)){//control particion vacia y estado sin asignar
							FI = Par[i].tamano - M->tamano;
							if (FI < min){
								min = FI;
								pos = i;
							}
						}
					}
				}
				//asigno la particiÃ³n
				Par[pos].id_Proc = M->id;
				Par[pos].FI = min;
				M->estado = true;
				if (encabezado == false){
					printf("Nueva asignacion de memoria:");
					printf("\n   Id Particion |  Dir Comienzo | TamParticion  |  IdProceso    |      FI \n");
					encabezado = true;
				}
				printf("\t %i \t|\t %i \t|\t %i\t|\t %i \t|\t%i\n",Par[pos].id, Par[pos].dirInicio, Par[pos].tamano, Par[pos].id_Proc, Par[pos].FI);
				
				//Paso el proceso de la lista de NUEVOS a la de LISTOS
				if (M == PrN){ //Desvinculo el elemento de la lista de NUEVOS
					PrN = M->proximo;
				}else{
					A->proximo = M->proximo;
				}
				aux = M;
				A = M;
				M = M->proximo;
				agregarElemento(PrL, aux); //Agrego el elemento a la lista de LISTOS
				//printf("cargue elemento a lista de listos\n");	
			}else{ //Carga encolada de la lista LISTOS Y SUSPENDIDOS
				if (PrLS == NULL){
					PrLS = PrN;
					ULS = PrLS;
				}else{
					ULS->proximo = PrN;
					ULS = PrN;
				}
				A = M;
				M = M->proximo;
				PrN = PrN->proximo;
				ULS->proximo = NULL;
				printf("El proceso %i entro en la lista de LISTOS Y SUSPENDIDOS\n", A->id);
			}
		}
		MLS = PrLS;
		Proceso *aux = MLS;
		while (MLS != NULL){ //Cargamos la lista de LISTOS --> SE ASIGNA LAS PARTICIONES
			pos = bestFit();
			if (pos != 0){ //me dice si se le asigno memoria o no al proceso
				//asigno la particiÃ³n
				Par[pos].id_Proc = MLS->id;
				Par[pos].FI = Par[pos].tamano - MLS->tamano;
				MLS->estado = true;
				if (encabezado == false){
					printf("Nueva asignacion de memoria:");
					printf("\n   Id Particion |  Dir Comienzo | TamParticion  |  IdProceso    |      FI \n");
					encabezado = true;
				}
				printf("\t %i \t|\t %i \t|\t %i\t|\t %i \t|\t%i\n",Par[pos].id, Par[pos].dirInicio, Par[pos].tamano, Par[pos].id_Proc, Par[pos].FI);
				
				//Paso el proceso de la lista de LISTOS Y SUSPENDIDOS a la de LISTOS
				if (MLS == PrLS){ //Desvinculo el elemento de la lista de LISTOS Y SUSPENDIDOS
					PrLS = MLS->proximo;
				}else{
					A->proximo = MLS->proximo;
				}
				aux = MLS;
				MLS = MLS->proximo;
				agregarElemento(PrL, aux); //Agrego el elemento a la lista de LISTOS
				//printf("cargue elemento a lista de listos\n");
			}else{
				A = MLS;
				MLS = MLS->proximo;
			}
			//printf("%i\n", MLS->id);
		} //PARTICIONES CARGADAS
		MLS = PrLS;
		//--------------------------------------------Asignamos CPU----------------------------------------------------
		if (PrL != NULL){
			if (estadoCPU == false){ //Caso inicial, asigno CPU al primer proceso
				CPU = PrL; // se asigna CPU
				CPU->estado = true;
				estadoCPU = true;
				PrL = PrL->proximo; //Se saca al proceso de la lista de Listos
				CPU->proximo=NULL;
				printf("El proceso %i comenzo a ejecutarse\n", CPU->id);
			}else{
				if (CPU->TI > PrL->TI){ //Se cambia de proceso  
					PrL->estado = true;  //tengo que ver donde quedo parado con ML
					CPU->estado = false;
					agregarElemento(PrL, CPU);
					CPU= PrL;
					CPU ->estado=true;
					PrL=PrL->proximo;
					CPU->proximo=NULL; 
					printf("El proceso %i comenzo a ejecutarse\n", CPU->id);
					/*
					//Cargo lista de bloqueados
					Mo = PrL;
					Mo->proximo = PrB;
					PrB = Mo;*/
				} 
			}
		}
		
		//------------------------------------------------------------------------------------
		if (CPU != NULL){
			CPU->TI --; //Actualizar TI proceso en ejecucion
			
			//Preguntar si termino de ejecutar
			if(CPU->TI == 0){ //Sacar de listos, agregar en terminado y devolver particion
				CPU->TR = instante - CPU->TA;
				te = CPU->TR - CPU->TIRes;
				if (te < 0){
					te = 0;
				}
				CPU->proximo = PrT;
				PrT = CPU;
				estadoCPU = false;
				for (i = 1; i < 4; i++){
					if (Par[i].id_Proc == CPU->id){
						Par[i].id_Proc = 0;
						Par[i].FI = 0;
						CPU->estado = false;
						pos = i;
					}
				}
				cont++; //cantidad de procesos que terminan
				printf("El proceso %i finalizo. Con un tiempo de retorno %i y un tiempo de espera de %i.\n", CPU->id, CPU->TR, te);
				printf("Se libero la particion:");
				printf("\n   Id Particion |  Dir Comienzo | TamParticion  |  IdProceso    |      FI \n");
				printf("\t%i \t|\t %i \t|\t %i\t|\t %i \t|\t%i\n",Par[pos].id, Par[pos].dirInicio, Par[pos].tamano, Par[pos].id_Proc, Par[pos].FI);
			}
		}
		system("pause");
	}

	return 0;
}
