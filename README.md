##Análise do sistema operacional baseado em Unix XV6
####Implementação de um escalonador passo largo (Stride Scheduler)
<br><br><br><br><br><br>
#####Trabalho apresentado ao curso de Ciência da Computação da Universidade Federal da Fronteira Sul (UFFS) como trabalho final da disciplina de Sistemas Operacionais
<br><br><br><br><br><br>
#####Acadêmico: Rodrigo Bueno Tomiosso
<br><br><br><br><br><br>
**Abstract.** This meta-paper describes the knowledge developed in XV6 operating system, version 6 implementation of Unix for Intel x86 platform, implemented at the Massachusetts Institute of Technology (MIT) and a stride scheduler implementation.<br>
**Resumo.** Este meta-artigo descreve um relatório sobre o conhecimento desenvolvido no sistema operacional XV6, implementação do Unix versão 6 para a plataforma Intel x86, implementado no Massachusetts Institute of Technology (MIT) e implementação de um escalonador de passo largo.


---------------------------------------
<br><br><br><br><br><br>
####1. Introdução
Para a análise do sistema foram utilizados o código disponibilizado (git://pdos.csail.mit.edu/xv6/xv6.git) pelo Massachusetts Institute of Technology (MIT) sobre a própria licença de distribuição além da [documentação](http://pdos.csail.mit.edu/6.828/2014/xv6.html) elaborada pela classe 6.828 do MIT conforme disponibilizado na bibliografia.

####2. Compreendendo o XV6
O sistema operacional é responsável pelo controle e desenvolvimento da melhor performance possível à partir de um hardware suportado, compartilhando partes do sistema entre diversos programas. O kernel (ponte entre as aplicações e o hardware) utiliza sistemas de proteção que asseguram que cada processo tenha acesso apenas à sua própria porção de memória. Quando um programa realiza uma chamada no sistema, o software verifica seu nível de privilégio e executa esta função no kernel.
Uma chamada de sistema ou syscall é um serviço disponibilizado pelo sistema operacional que acessa o kernel, o kernel realiza a função desejada e, em seguida retorna para o usuário. Desta maneira os processos são executados alternadamente no ambiente de usuário e no ambiente do kernel.
O XV6 disponibiliza uma lista de serviços e chamadas do sistema, são elas:

Syscall	Descrição<br>
**fork()**	Cria um processo<br>
**exit()**	Finaliza um processo<br>
**wait()**	Aguarda um processo filho finalizar<br>
**kill(pid)**	Finaliza o processo de id “pid"<br>
**getpid()**	Retorna o id do processo<br>
**sleep(n)**	“Dorme” por n unidades de tempo<br>
**exec(nomearquivo, *argv)**	Carrega um arquivo e o executa<br>
**sbrk(n)**	Aumenta a memória de um processo em n bytes<br>
**open(nomearquivo, flags)**	Abre um arquivo, flags indica leitura/escrita<br>
**read(fd,buffer,n)**	Le n bytes de um arquivo aberto para o buffer<br>
**write(fd, buffer, n)**	Escreve n bytes para um arquivo aberto<br>
**close(fd)**	Libera o arquivo aberto fd<br>
**dup(fd)**	Duplica fd<br>
**chdir(diretorio)**	Alterna para o diretorio<br>
**mkdir(diretorio)**	Cria um novo diretorio<br>
**mknod(name, major, minor)**	Cria um novo “nodo" (arquivo, dispositivo, arquivo especial ou pipe)<br>
**fstat(fd)**	Retorna informações à respeito do arquivo fd aberto.<br>
**link(f1, f2)**	Cria um atalho ou nome (f2) para o arquivo f1<br>
**unlink(arquivo)**	Remove um arquivo<br>

Abaixo podemos visualizar o trecho de código onde as chamadas de sistema são tratadas.<br><br>
*initcode.S*
```
# Initial process execs /init.

#include "syscall.h"
#include "traps.h"


# exec(init, argv)
.globl start
start:
  pushl $argv
  pushl $init
  pushl $0  // where caller pc would be
  movl $SYS_exec, %eax
  int $T_SYSCALL

# for(;;) exit();
exit:
  movl $SYS_exit, %eax
  int $T_SYSCALL
  jmp exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .long init
  .long 0
```
O processo enviou argumentos por uma chamada à exec na pilha de processos e pôs o número da syscall em %eax. Os números de chamadas do sistema devem combinar com a tabela de syscalls. É necessário observar que a instrução int ($T_SYSCALL) alterna o processador entre o modo usuário e o modo kernel, onde o kernel invoca a função solicitada (exemplo: mkdir), o kernel também pode recuperar informação de argumentos para esta função mkdir. Este mesmo bloco também trata as interrupções e exceções disparadas no sistema.

#####2.1 Exceções e interrupções
As interrupções são semelhantes à chamadas do sistema, exceto por serem disparadas à qualquer momento. Graças às placas com múltiplos processadores, novas técnicas de interrupção são necessárias pois cada CPU precisa tratar as interrupções enviadas à elas, além de haver um método de roteamento de interrupções para estes processadores.

#####2.2 Sistema de Arquivos
O sistema de arquivos é responsável por organizar e armazenar dados e geralmente suporta compartilhamento de dados entre usuários e aplicações além da própria persistência dos dados, de maneira que estes dados estejam disponíveis mesmo após reiniciar a máquina.
O sistema de arquivos do XV6 provê arquivos, diretórios e caminhos. Há uma série de necessidades como controlar estruturas para representar a árvore de diretórios e arquivos para identificar os blocos que contém cada arquivo, suportar a recuperação de erros de maneira a permitir a continuidade do sistema caso uma falha ocorra, coordenar acesso de diferentes processos que utilizam o mesmo sistema de arquivos e acessar o disco em velocidade proporcional ao acesso de memória, mantendo em cache os blocos mais utilizados.

#####2.3 Boot
Quando o "PC" é iniciado, começa a execução da BIOS onde o hardware é preparado para transferir o controle para o sistema operacional e especificamente para o setor de boot que contém a rotina responsável por carregar o kernel na memória, no caso do xv6, feito através da classe bootasm.S.

#####2.4 Bloqueios
O XV6 opera em computadores com múltiplas CPUs executando códigos independentes. Essas CPUs operam em um endereço de memória específico e compartilham informações. Mesmo em um único processador, o XV6 usa mecanismos para prevenir interrupções de interferir com códigos que não devem ser interrompidos. A principal informação na estrutura é locked, uma palavra que é zero quando está disponível e não zero quando está ocupado.

*Spinlock.h*
```
// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?
  
  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
};
```

#####2.5 Escalonador de processos
De modo geral, todo sistema operacional roda mais processos do que a quantidade de processadores disponíveis, desta maneira, é necessário considerar estratégias para compartilhar o tempo de processamento. Uma técnica comum é dar ao processo a impressão de que ele possui seu próprio processador virtual.<br>
O XV6 utiliza a técnica de escalonamento RoundRobin ou seja, quando um processo está aguardando pelo disco ou por outro process o Xv6 coloca este processo em estado de espera e prepara outro processo para execução, mandendo um esquema de fila circular. Os processos também podem sair da execução após um determinado tempo, permitindo assim que todos processos tenham a mesma fatia de acesso ao processador.<br>
Para alternar entre um processo e outro são utilizados mecanismos de trocas de contexto, são executados dois tipos de troca de contexto de baixo nível: De uma thread do escalonador da CPU para um processo e de uma thread do escalonador para uma thread do kernel.<br>
No XV6 não há trocas diretas de um processo do usuário para outro, isto ocorre através de uma transição do kernel de usuário (chamada do sistema ou interrupção), cada processo tem sua pilha de kernel e conjunto de registradores e cada CPU tem sua própria thread do escalonador para execução. Alternar entre uma thread e outra envolve salvar os registradores da CPU da thread atual e restaurar os registradores salvos anteriormente da thread atual.

