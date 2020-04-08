## Luiz Felipe Lehmen Lersch
## elc139-Programação Paralela

## runtime
Uma opção está com a variável de ambiente OMP_SCHEDULE e a outra opção está com a função omp_set_schedule. No programa
a função fillArrayConcurrently() atualiza a variável.

## ordered
O  construtor  ordered  permite  que  um  laço  seja  executado  na  ordem  seqüencial.

## static
Por padrão, o OpenMP atribui estaticamente iterações de loop às threads.
Quando o paralelo para o bloco é inserido, ele atribui a cada thread o conjunto de iterações de loop que deve executar.

Case 1: static no chunk
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 
#### sem chunk o conjunto de iterações é o total dividido pelo número de thred

Case 2: static chunk = 5
AAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCC
A=20 B=20 C=20 
#### com chunk igual a 5 o número de iterações é 5

## dynamic
Aqui, o OpenMP atribui uma iteração para cada thread.
Quando o encadeamento terminar, será atribuída a próxima iteração que ainda não foi executada.

Case 3: dynamic no chunk
CBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBA
A=20 B=20 C=20
#### sem chunk uma iteração é dado a cada thread por vez

Case 4: dynamic chunk = 5
BBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAABBBBBCCCCCAAAAA
A=20 B=20 C=20 
#### com chunk igual a 5 o número de iterações é 5

## guided
Essa política de agendamento é semelhante a um agendamento dinâmico, exceto que o tamanho do chunk muda à medida 
que o programa é executado. Começa com grandes chunks, mas depois se ajusta a tamanhos menores de chunk se a 
carga de trabalho estiver desequilibrada.

Case 5: guided no chunk
BBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCAAAAAAAAABBBBBBCCCCAAABBCA
A=13 B=28 C=19 
#### sem chunk o menor chunk é 1

Case 6: guided chunk = 5
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBCCCCCCCCCAAAAAABBBBBCCCCCA
A=27 B=19 C=14 
#### com chunk igual a 5 o menor é 5(se sobrar menos pode executar menos como no exemplo)

Case 7: auto
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 
#### O tipo de agendamento automático delega a decisão do agendamento ao compilador e / ou sistema de tempo de execução.no caso static sem chunk

Case 8: static no lock chunk = 5
CACBACBCBAC-BA-BACBACBACBACA-BACACBACBACABCAB-ACACBACB-CBCB-
A=18 B=17 C=19 
#### sem exclusão mútua as variáveis acessam ao mesmo tempo o index, o que gera erros.



http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/OpenMP_Dynamic_Scheduling.pdf
http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
