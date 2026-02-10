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
#include <time.h>

//Nome unico do algoritmo. Deve ter 4 caracteres.
const char lottName[]="LOTT";
//=====Funcoes Auxiliares=====

// Retorna total de tickets de todos os processos prontos
static int contarTicketsProntos(Process *lista) {
   int total = 0;
    Process *p;
    for (p = lista; p != NULL; p = processGetNext(p)) {
        if (processGetStatus(p) == PROC_READY) {
            LotterySchedParams *parametros = processGetSchedParams(p);
            total += parametros->num_tickets;
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
   // Inicializa semente para aleatorios
    srand((unsigned int)time(NULL));

    // Registra o escalonador
    SchedInfo *info = malloc(sizeof(SchedInfo));
    snprintf(info->name, MAX_NAME_LEN+1, "%s", lottName);
    info->initParamsFn = lottInitSchedParams;
    info->notifyProcStatusChangeFn = lottNotifyProcStatusChange;
    info->scheduleFn = lottSchedule;
    info->releaseParamsFn = lottReleaseParams;
    schedRegisterScheduler(info);
}

//Inicializa os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' associado ao slot de Lottery
void lottInitSchedParams(Process *p, void *params) {
 LotterySchedParams *parametros = (LotterySchedParams*)params;

    if (!parametros) {
        parametros = malloc(sizeof(LotterySchedParams));
        parametros->num_tickets = 1; // Ticket inicial padrao
    }

    processSetSchedParams(p, parametros);
}

//Recebe a notificação de que um processo sob gerência de Lottery mudou de estado
//Deve realizar qualquer atualização de dados da Loteria necessária quando um processo muda de estado
void lottNotifyProcStatusChange(Process* p) {
	 (void)p;
}

//Retorna o proximo processo a obter a CPU, conforme o algortimo Lottery 
Process* lottSchedule(Process *plist) {
	   int total_tickets = contarTicketsProntos(plist);
    if (total_tickets == 0) return NULL;

    int ticket_sorteado = rand() % total_tickets + 1;
    int acumulador = 0;
    Process *p;
    for (p = plist; p != NULL; p = processGetNext(p)) {
        if (processGetStatus(p) == PROC_READY) {
            LotterySchedParams *parametros = processGetSchedParams(p);
            acumulador += parametros->num_tickets;
            if (acumulador >= ticket_sorteado)
                return p;
        }
    }

    return NULL;
}

//Libera os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' desassociado do slot de Lottery
//Retorna o numero do slot ao qual o processo estava associado
int lottReleaseParams(Process *p) {
	  LotterySchedParams *parametros = processGetSchedParams(p);
    if (parametros) {
        free(parametros);
        processSetSchedParams(p, NULL);
    }
    return processGetSchedSlot(p);
}

//Transfere certo numero de tickets do processo src para o processo dst.
//Retorna o numero de tickets efetivamente transfeirdos (pode ser menos)
int lottTransferTickets(Process *src, Process *dst, int tickets) {
	if (!src || !dst || tickets <= 0) return 0;

    LotterySchedParams *param_origem = processGetSchedParams(src);
    LotterySchedParams *param_destino = processGetSchedParams(dst);
    if (!param_origem || !param_destino) return 0;

    int transferidos = tickets;
    if (transferidos > param_origem->num_tickets)
        transferidos = param_origem->num_tickets;

    param_origem->num_tickets -= transferidos;
    param_destino->num_tickets += transferidos;

    return transferidos;
}