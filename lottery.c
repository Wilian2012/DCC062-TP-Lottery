/*
*  lottery.c - Implementacao do algoritmo Lottery Scheduling e sua API
*
*  Autores: SUPER_PROGRAMADORES_C
*  Projeto: Trabalho Pratico I - Sistemas Operacionais
*  Organizacao: Universidade Federal de Juiz de Fora
*  Departamento: Dep. Ciencia da Computacao
*
*/

#include "lottery.h"
#include <stdio.h>
#include <string.h>

//Nome unico do algoritmo. Deve ter 4 caracteres.
const char lottName[]="LOTT";
//=====Funcoes Auxiliares=====

// Retorna total de tickets de todos os processos prontos
static int contarTicketsProntos(Process *lista) {
    int total = 0;
    Process *p;
    for (p = lista; p != NULL; p = processGetNext(p)) {
        if (processGetStatus(p) == PROC_READY) {
            LotterySchedParams *params = processGetSchedParams(p);
            total += params->num_tickets;
        }
    }
    return total;
}




//=====Funcoes da API=====

//Funcao chamada pela inicializacao do S.O. para a incializacao do escalonador
//conforme o algoritmo Lottery Scheduling
//Deve envolver a inicializacao de possiveis parametros gerais
//Deve envolver o registro do algoritmo junto ao escalonador
void lottInitSchedInfo(void) {
	//...
   SchedInfo info;
    strncpy(info.name, lottName, MAX_NAME_LEN);
    info.initParamsFn = lottInitSchedParams;
    info.notifyProcStatusChangeFn = lottNotifyProcStatusChange;
    info.scheduleFn = lottSchedule;
    info.releaseParamsFn = lottReleaseParams;

    // Registrar no slot do escalonador
    schedRegisterScheduler(&info);

    // Inicializar semente do rand
    srand((unsigned int)time(NULL));

}

//Inicializa os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' associado ao slot de Lottery
void lottInitSchedParams(Process *p, void *params) {
	//...
	LotterySchedParams *parametros = (LotterySchedParams*)params;

    if (!parametros) {
        parametros = malloc(sizeof(LotterySchedParams));
        parametros->num_tickets = 1; // valor padrao
    }

    processSetSchedParams(processo, parametros);
}

//Recebe a notificação de que um processo sob gerência de Lottery mudou de estado
//Deve realizar qualquer atualização de dados da Loteria necessária quando um processo muda de estado
void lottNotifyProcStatusChange(Process* p) {
	//...
}

//Retorna o proximo processo a obter a CPU, conforme o algortimo Lottery 
Process* lottSchedule(Process *plist) {
	//...
	return NULL;
}

//Libera os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' desassociado do slot de Lottery
//Retorna o numero do slot ao qual o processo estava associado
int lottReleaseParams(Process *p) {
	//...
	return 0;
}

//Transfere certo numero de tickets do processo src para o processo dst.
//Retorna o numero de tickets efetivamente transfeirdos (pode ser menos)
int lottTransferTickets(Process *src, Process *dst, int tickets) {
	//...
	return 0;
}
