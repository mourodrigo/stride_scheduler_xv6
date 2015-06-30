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

*swtch.S - Troca de contexto entre processos de nível de usuário e de kernel*
```
# Context switch
#
#   void swtch(struct context **old, struct context *new);
# 
# Save current register context in old
# and then load register context from new.

.globl swtch
swtch:
  movl 4(%esp), %eax
  movl 8(%esp), %edx

  # Save old callee-save registers
  pushl %ebp
  pushl %ebx
  pushl %esi
  pushl %edi

  # Switch stacks
  movl %esp, (%eax)
  movl %edx, %esp

  # Load new callee-save registers
  popl %edi
  popl %esi
  popl %ebx
  popl %ebp
  ret

```
O código apresentado acima é responsável por salvar e resgatar as informações dos registradores (contextos).<br> Quando é chegado o momento de um processo sair da execução da CPU, a thread do kernel do processo chamará o swtch para salvar seu próprio contexto e retornar para a contexto do escalonador. <br>
Esta ação é originalmente disparada pela classe trap.c, responsável por administrar as interrupções do sistema.<br>

*Trap.c*
```
 // Força o processo à sair se for morto e estiver no espaço do usuário
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Força o processo à sair de execução por tempo de relógio
  if(proc && proc->state == RUNNING && tf->trapno == T_IRQ0+IRQ_TIMER)
    yield();

  // Verifica se o processo foi morto desde o momento que foi removido da execução
  if(proc && proc->killed && (tf->cs&3) == DPL_USER)
    exit();
```

Cada contexto é representado por um ponteiro para uma estrutura armazenada no próprio kernel em que está envolvido, o swtch recebe dois argumentos, um ponteiro para o contexto antigo e um para o novo contexto. Então são alternados os registradores da CPU atual na pilha e salvos o ponteiro da pilha no ponteiro do contexto antigo.<br>

<br>*proc.c*
```
void
sched(void) //exemplo de contexto sendo alternado
{
  int intena;

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(cpu->ncli != 1)
    panic("sched locks");
  if(proc->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = cpu->intena;
  swtch(&proc->context, cpu->scheduler);
  cpu->intena = intena;
}
```
Na estrutura original o escalonador roda em um looping, procurando um processo para rodar, coloca-o em execução até que o mesmo pare ou seja removido por tempo de relógio e repete o processo novamente. O escalonador mantém ptable.lock para a maioria de suas rotinas, porém libera o bloqueio e habilita as interrupções a cada interação do looping. Isto é importante para o caso especial em que a CPU está ociosa ou hajam múltiplas CPUs.<br>
Hibernar e acordar são métodos de sincronização entre processos simples e precisos podendo ser utilizados para vários tipos de espera. Um exemplo é a chamada de espera que um processo pai usa para aguardar enquanto um processo filho está sendo executado. No XV6 quando um processo termina, este não é finalizado imediatamente. Ele é enviado para um estado zumbi até que o processo pai seja notificado da finalização. O processo pai é então responsável por liberar a memória associada. Caso o processo pai seja finalizado antes do filho, o processo init é responsável por adotar este processo filho e aguardar por ele, de tal maneira que sempre haverá um processo pai para limpar a memória após sua utilização.

#####3. Problema proposto
Implementar o escalonador de processos stride scheduling (escalonamento em passos largos). Semelhante ao escalonamento por loteria, cada processo recebe um número fixo de bilhetes (tickets). Ao invés de utilizar um sorteio (abordagem probabilística), calcula-se o “passo” (stride) de cada processo como sendo o resultado da divisão de um valor constante (e.g., 10.000) pelo número de bilhetes do processo. Cada processo inicia com uma “passada inicial” igual a zero (0). O escalonador seleciona o processo com o menor valor de passada atual; portanto, inicialmente qualquer um dos processos prontos podem ser selecionados (utilizou-se o número do PID como critério de desempate). Após selecionado, a passada do processo é incrementada com o valor do “passo” do processo. 
#####4. Implementações realizadas
Foram realizadas mudanças em uma série de arquivos, as mudanças propostas seguem em imagens abaixo com os devidos comentários justificando a alteração conforme o problema proposto.<br><br>

**Defs.h**<br>
Foram desenvolvidos métodos para criar processos conforme o nível de prioridade, cada método cria processos da mesma maneira porém utilizando números de tickets diferentes.

* forkLowest - 100 tickets
* forkLow - 300 tickets
* forkMedium - 500 tickets
* forkHigh - 750 tickets
* forkHighest - 990 tickets

O método padrão para criação de processos (fork) também utilizará 500 tickets na criação de processos.
Também foi desenvolvido o método switchDebug, responsável por habilitar logs de debug, esses logs são disparados dentro da execução do kernel, na criação e finalização de processos, indicando o número do PID e quantidade de tickets. <br>
Os logs do kernel são impressos dentro de tags |- -|.


![defs.h](http://s23.postimg.org/evomtie3f/Captura_de_Tela_2015_06_30_s_19_17_46.png)<br><br>

**Init.c**<br>
O primeiro processo responsável pelo shell foi inicializado com baixa prioridade (300 tickets).
![init.c](http://s8.postimg.org/qzhxxetyt/init_c.png)<br><br>

**makefile**<br>
Houveral alterações no arquivo makefile para permitir a compilação mesmo existindo warnings no código (deve-se à implementação ter sido feita utilizando a IDE x-code)
![makefile](http://s8.postimg.org/mmet18vud/makefile1.png)<br><br>

**makefile**<br>
Outras flags para permitir a compilação mesmo com warnings removidas.
![makefile](http://s8.postimg.org/w88dhjn05/makefile2.png)<br><br>

**makefile**<br>
Alterações no makefile para compilação dos "programas de usuário" desenvolvidos. Os programas de testes serão abordados em seguida.
![makefile](http://s8.postimg.org/u144tmhpx/makefile3.png)<br><br>

**proc.c**<br>
Contante do projeto que permite alternar entre os escalonadores passo largo e round robin. Durante a implementação do código foram feitas condições para que seja permitida a execução com ambas as técnicas de escalonamento.
![proc.c](http://s8.postimg.org/agplquh4l/proc_c1.png)<br><br>

**proc.c**<br>
O método allocproc, responsável por alocar um processo em sua fase inicial, obter o número do pid e setar as configurações iniciais sofreu alterações para receber o número de tickets, bem como atribuir o número correto de tickets, passada e o limite da passada (até quanto o processo pode "andar") ao processo criado.
![proc.c](http://s8.postimg.org/vzfe5czlh/proc_c2.png)<br><br>

**proc.c**<br>
O primeiro processo p (pid 1) alogado dentro do userinit é alocado diretamente através do allocproc, não passando pelo fork, portanto o mesmo foi criado também com baixa prioridade (300 tickets).
![proc.c](http://s8.postimg.org/g3hdaku9h/proc_c3.png)<br><br>

**proc.c**<br>
O método fork passou à ser um método semelhante aos outros métodos com prioridade, não alocando de fato o processo. O método responsável pela alocação é o método forks(int tickets), que passa a ser chamado por todos os outros métodos com suas devidas prioridades. No método também são atribuídas as informações de tickets e passada do processo.
![proc.c](http://s8.postimg.org/ye79psfud/proc_c4.png)<br><br>

**proc.c**<br>
Implementação dos métodos de fork com prioridades e seus devidos números de tickets.
![proc.c](http://s8.postimg.org/mssi4c4z9/proc_c5.png)<br><br>

**proc.c**<br>
Principal método do escalonador ( void scheduler(void){} ), responsável por adquirir um método e repassá-lo à CPU para execução. <br>
O for que percorre todos os processos foi alterado para percorrer toda a lista buscando o processo com menor passada executada. O processo não pode ser escolhido se não estiver pronto para execução (EMBRYO) ou se sua execução já tiver sido finalizada (ZOMBIE). Considerando que o if de comparação da passada considera apenas um valor MENOR de passos efetuados, não são necessários critérios de desempate pois o algoritmo seguirá a ordem lógica da lista (consequentemente pelo número do PID).<br>
Após identificado o método à ser executado o valor do limite da passada é atualizado, bem como o número do passo.<br>
O processo escolhido passa a ter seu estado definido como RUNNING, passando também à ser o processo principal. Definido o estado é feito um tratamento para o número da passada, onde caso o número tenha extrapolado o valor máximo de um inteiro, reinicia a contagem, impedindo que processos com longa execução fiquem em looping por limitações de alocação de informação.<br>
Em seguida é feita a troca de contexto do kernel para o processo e a tabela de processos é desbloqueada.

![proc.c](http://s8.postimg.org/722twywc5/proc_c6.png)<br><br>

**proc.c**<br>
A syscall criada switchDebug é responsável por habilitar e desabilitar a constante de debug.
![proc.c](http://s8.postimg.org/rq62pg6yd/proc_c7.png)<br><br>

**proc.c**<br>
Alterações realizadas na estrutura do processo, para permitir o número de tickets, tempo de uso da cpu, tamanho do passo, passos efetuados, limite de passos à serem efetuados a cada rodada.
![proc.h](http://s8.postimg.org/tabenufcl/proc_h.png)<br><br>

**sh.c**<br>
Métodos de teste e chamadas do sistemas foram alteradas para seguir o padrão de alocação de processsos com tickets.<br>
![sh.c](http://s8.postimg.org/iftylww8l/sh_c.png)<br><br>

**syscall.c**<br>
Adicionadas chamadas de sistema conforme os novos métodos, permitindo que sejam chamados à nível de kernel.<br>
![syscall.c](http://s30.postimg.org/ya1dnb6td/syscall_c.png)<br><br>

**syscall.h**<br>
Constantes numéricas para chamadas do sistema (syscalls).
![syscall.h](http://s8.postimg.org/j6moroylx/syscall_h.png)<br><br>

**sysproc.c**<br>
Implementação das syscalls.
![sysproc.c](http://s8.postimg.org/f6ay1pir9/sysproc_c.png)<br><br>

**trap.c**<br>
Importante alteração do escalonador pois permite que o processo seja retirado da execução assim que executado o número de passos necessários. Caso ainda não tenha chegado no limite de passos o número de passos realizados é incrementado.
![trap.c](http://s8.postimg.org/ya49i1vlh/trap_c.png)<br><br>

**user.h**<br>
Declaração do cabecalho das syscalls na classe importada pelo usuário, permitindo que sejam chamados à nível de usuário.
![user.h](http://s8.postimg.org/tyfnt1ool/user_h.png)<br><br>

**proc.c**<br>
Declaração das syscalls com protótipo das funções.
![usys.S/](http://s8.postimg.org/6lhmaj8l1/usys_S.png)<br><br>


#####5 Testes

Os testes seguem um algoritmo padrão que realiza cálculos em looping para manter o processo "ocupado" por um grande interalo de tempo. 
```
// Recebe como parametros a prioridade do processo e número de rodadas que realizará o cálculo, em seguida calcula o fatorial de x pelo numero de rodadas
void zombie(int prioridade,int rodadas){
    printf(0, "\nIniciando pid %d com prioridade %d",getpid(),prioridade);
    uint x=0; uint y=0; uint turn = rodadas;
    uint limit = 12;
    uint result = 0;
    uint systemTime = uptime();
    
    for(turn=turn;turn!=1;turn--){
        for(x=1;x<limit;x++){
            result = x;
            for(y=x;y!=1;y--){
                result = result*y;
            }
            
        }
        result=result-10;
    }
    printf(0, "\n Pid %d com prioridade %d finalizado em %d segundos\n\n" ,getpid(),prioridade,uptime()-systemTime);
    exit();
}
```

Todos os testes criam x processos com prioridades que variam da mais baixa até mais alta, calcularão o fatorial de 0 ate 12 por 4000000 vezes respectivamente. Após finalizar o cálculo será apresentado o PID do processo com o tempo de execucão.

*Teste1*
```
    int turns = 9600000; //processos executados em looping por 9600000 vezes
    
    forkTest(3,turns); // 500 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(3,turns); // 500 tickets
```
*Teste2*
```
    int turns = 2400000; //processos executados em looping por 9600000 vezes
    
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
```
*Teste3*
```
    int turns = 2400000; //processos executados em looping por 9600000 vezes
    
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(2,turns); // 300 tickets
    forkTest(3,turns); // 500 tickets
    forkTest(4,turns); // 750 tickets
    forkTest(5,turns); // 990 tickets
```
*Teste4*
```
    int turns = 2400000;
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(5,turns); // 990 tickets
```
*Teste5*
```
    int turns = 2400000;
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(1,turns); // 100 tickets
    forkTest(5,turns); // 990 tickets
```
