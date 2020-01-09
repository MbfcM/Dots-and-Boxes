#include <stdio.h>			//libraria para input e output, printf e scanf
#include <stdlib.h>			//libraria para random, system pause e system clear
#include <string.h> 		//libraria para copiar/concatenar strings, strcpy, strlen e memset
#include <conio.c> 			//libraria para usar cores (textcolor e textbackground)
#include <time.h>			//libraria para funcao time()
#include <stdarg.h>         //libraria para receber argumentos numa funcao

//variaveis globais
char tabul[77][77]; 										//matriz (tabuleiro do jogo)
int tamanho; 												//numero quadrados que utilizador quer no tabuleiro
int uJogadas; 												//historico jogadas -> 0 não aparece as ultimas jogadas do jogo <> 1 aparece.
int jogPos; 												//total de jogadas Possiveis
char nomeJogadorA[9];										//nome jogadorA
char nomeJogadorB[9];										//nome jogadorB
int pontJogadorA; 											//pontuacao do jogador A
int pontJogadorB; 											//pontuacao do jogador B
char jogadorAtual; 											//jogador Atual
int corJogadorA;											//cor jogadorA
int corJogadorB;											//cor jogadorB
int corTabuleiro;											//cor tabuleiro
int corCoordenadas;											//cor coordenadas
int mJogo;													//modo de jogo escolhido (HxH || HxPC || PCxPC)
int coords[2];												//coordenadas x y das jogadas dos PC

//Declaracao subrotinas
void tipoJogo();														//escolher modo jogo
void iniciaTabul();														//inicializar tabuleiro
void opcoesJogo();														//opcoes jogo para utilizador escolher
void nomesComputador();													//nomes disponiveis para pc		
void quemComeca(int nMenor, int nMaior);								//descobrir quem comeca a jogar
void atualizarEcra();													//atualizar estado tabuleiro
void jogadas();															//fazer jogadas
void jogadasPC();														//fazer pc fechar quadrados sempre que possivel
void verificacaoJogada(int x, int y, char jogador);						//verificar se fechou quadrado
void atribuirPontos(char jogador);										//atribuir pontos
void fimJogo();															//mostrar final jogo
int verificarValorIntroduzido(int nMenor, int nMaior, int opcao);		//verificar se valores introduzidos estao dentro dos parametros definidos
int gerarNumero(int nMinimo, int nMaximo);								//gerar numeros aleatorios
void textoPersonalizado(int centro, const char *string, ...);			//conseguir centrar texto e meter cor de forma mais simples
void infJogo();															//informacoes acerca do jogo

int main(int argc, char *argv[]) {

    //gerar diferentes numeros -> (a cada execução o valor da "semente" e diferente)
    srand(time(NULL));
    
    //variaveis
    int opcaoMenu;
    int corInicio = gerarNumero(1,15);
	
	//repetir enquanto utilizador não inserir opcao 4
    do
    {
        //menu inicial - nome jogo
        textoPersonalizado(1, "#%d                                                              #0 ", corInicio);
        textoPersonalizado(1, "#%d                        $0 Dots and Boxes                        #0 ",corInicio);
        textoPersonalizado(1, "#%d                        $7                                       #0 ", corInicio);
        puts("");

        //menu inicial - mostrar opcoes
        textoPersonalizado(1, "+-------------------------------------------------------+");
        textoPersonalizado(1, "| 1 - Comecar || 2 - Acerca || 3 - Creditos || 4 - Sair |");
        textoPersonalizado(1, "+-------------------------------------------------------+");
        
        //escolher opçao
        printf("\nInsira o numero para escolher a opcao: ");
        opcaoMenu = verificarValorIntroduzido(1, 4, 0);
        fflush(stdin);

        //dependendo da escolha do utilizador executar as tarefas em cada opcao
        switch(opcaoMenu){
            case 1 :						//jogar
                tipoJogo();
                atualizarEcra();
                jogadas();
                break;
            case 2 :						//informacoes sobre jogo
                infJogo();
                break;
            case 3 :						//informacoes sobre quem realizou trabalho
                printf("\nBruno Carvalho\nLinguagens de Programacao\nAplicacoes Moveis\n2019-2020\n\n");
                system("pause");			//pausa
                system("cls");				//limpar ecra
                break;						
            case 4 :						//sair da aplicacao
                printf("\nA sair da aplicacao...\n");
                break;
            default :						//mensagem caso utilizador insira numero nao correspondente aos apresentados
                printf("\nEscolha invalida\n\n");
        }

    }while(opcaoMenu != 4);
}

//permite a escolha do modo de jogo
void tipoJogo()
{
	//escolher modo jogo
    printf("\nEscolha um modo de jogo: \n");
    printf("1 - (Hum x Hum) \n2 - (Hum x PC ou PC x Hum) \n3 - (PC x PC)\n");
    printf("Modo de Jogo: ");
    mJogo = verificarValorIntroduzido(1, 3, 1);
    fflush(stdin);
    puts("");
    
    //procedimento inicializar tabuleiro
    iniciaTabul();
}

//inicializar tabuleiro
void iniciaTabul()
{
    int i, j, total=0;

	//escolher tamanho tabuleiro
    printf("Insira quantos quadrados tem o tabuleiro por linha/coluna: ");
    printf("\nTamanho min: 1 <> max: 38\n", tamanho);
    printf("Tamanho: ");
    tamanho = verificarValorIntroduzido(1, 38, 1);
    fflush(stdin);
    puts("");
    tamanho = tamanho * 2 + 1; 				//alterar valor introduzido pelo utilizador para fazer os quadrados perfeitos

    //reseta pontuaçoes e tabuleiro
    pontJogadorA = 0;
    pontJogadorB = 0;
    memset(&tabul[0][0], ' ', sizeof(tabul));

	//criar tabuleiro e contar espacos vazios do mesmo
    for (i = 0; i < tamanho; i++)    	    
        for (j = 0; j < tamanho; j++) 	    
            if (i%2 == 0 && j%2 == 0 )		//se for par insere o "o" na matriz do tabuleiro
                tabul[i][j] = 'o';
            else 							//se nao for par incrementa a variavel total
                total++;


    jogPos = total - ((tamanho - 1) / 2 * ((tamanho - 1) / 2)); 	//calcular numero de jogadas possiveis
    
    //procedimento escolher opcoes jogo
    opcoesJogo();
}

//opcoes nome jogadores, historico de jogadas e cores
void opcoesJogo()
{	
	//opcao escolher ultimas jogadas ou nao
    printf("Deseja mostrar ultimas jogadas da partida? (0 - Nao <> 1 - Sim)\n");
    printf("Ultimas Jogadas: ");
    uJogadas = verificarValorIntroduzido(0, 1, 1);
    fflush(stdin);
    puts("");

    do
    {
    	//nomes jogadores
        if (mJogo == 1)
        {
        	//limitar tamanho nome jogadores
            do
            {
            	//inserir nome
                printf("Introduza o nome do 1 jogador (ate %d caracteres): ", sizeof(nomeJogadorA));
                scanf("%s", &nomeJogadorA);
                fflush(stdin);									//limpar buffer
				
				//verificar se nome esta dentro do limite definido
                if (strlen(nomeJogadorA) > sizeof(nomeJogadorA))
                    printf("Insira um nome mais curto\n");

            }while(strlen(nomeJogadorA) > sizeof(nomeJogadorA));

            do
            {
                printf("Introduza o nome do 2 jogador (ate %d caracteres): ", sizeof(nomeJogadorB));
                scanf("%s", &nomeJogadorB);
                fflush(stdin);

                if (strlen(nomeJogadorB) > sizeof(nomeJogadorB))
                    printf("Insira um nome mais curto\n");
                    
            }while(strlen(nomeJogadorB) > sizeof(nomeJogadorB));
            puts("");
        }
        else if (mJogo == 2)
        {
            do
            {
                printf("Introduza o nome do 1 jogador (ate %d caracteres): ", sizeof(nomeJogadorA));
                scanf("%s", &nomeJogadorA);
                fflush(stdin);

                if (strlen(nomeJogadorA) > sizeof(nomeJogadorA))
                    printf("Insira um nome mais curto\n");

            }while(strlen(nomeJogadorA) > sizeof(nomeJogadorA));
			puts("");
			
			//escolher nome computador
            nomesComputador();
        }
        else
            nomesComputador();

		//comparar se os nomes sao iguais
        if(strcmp(nomeJogadorA, nomeJogadorB) == 0)
            printf("Os nomes nao podem ser iguais!!\n\n");
        else
        {
        	//escolher cores
            do
            {
            	//mostrar cores disponiveis a escolher
                puts("");
                textoPersonalizado(1,"$1 1 $2 2 $3 3 $4 4 $5 5 $6 6 $7 7 $8 8 $9 9 $10 10$11 11$12 12$13 13$14 14$15 15  $7 ");
				
				//escolher cores para o jogo
                if(mJogo == 1)
                {
                	//cor tabuleiro
                    printf("\n\nEscolha a cor do tabuleiro: ");
                    corTabuleiro = verificarValorIntroduzido(1, 15, 1);
                    fflush(stdin);
                    
                    //cores jogadores
                    printf("Escolha a cor %s: ", nomeJogadorA);
                    corJogadorA = verificarValorIntroduzido(1, 15, 1);
                    fflush(stdin);
                    printf("Escolha a cor %s: ", nomeJogadorB);
                    corJogadorB = verificarValorIntroduzido(1, 15, 1);
                    fflush(stdin);
                }
                else if (mJogo == 2)
                {
                    printf("\n\nEscolha a cor do tabuleiro: ");
                    corTabuleiro = verificarValorIntroduzido(1, 15, 1);
                    fflush(stdin);
                    
                    printf("Escolha a cor %s: ", nomeJogadorA);
                    corJogadorA = verificarValorIntroduzido(1, 15, 1);
                    fflush(stdin);

                    do
                    {
                        corJogadorB = gerarNumero(1, 15);
                    }while(corTabuleiro == corJogadorB || corJogadorA == corJogadorB);

                   printf("%s escolheu a cor: %d\n", nomeJogadorB, corJogadorB);
                   
                }
                else
                {
                    corTabuleiro = verificarValorIntroduzido(1, 15, 2);
                    fflush(stdin);
                    printf("\n\nA cor do tabuleiro e: %d\n", corTabuleiro);
                   
                    do
                    {
                        corJogadorA = gerarNumero(1, 15);
                        corJogadorB = gerarNumero(1, 15);
                    }while(corTabuleiro == corJogadorB || corJogadorA == corJogadorB);
                    
                    printf("%s escolheu a cor: %d\n", nomeJogadorA, corJogadorA);
					printf("%s escolheu a cor: %d\n", nomeJogadorB, corJogadorB);
                }

				//verificar se existem cores iguais
                if(corJogadorA == corJogadorB || corJogadorA == corTabuleiro || corJogadorB == corTabuleiro)
                    printf("\nAs cores tem de ser todas diferentes!!\n\n");

            }while(corJogadorA == corJogadorB || corJogadorA == corTabuleiro || corJogadorB == corTabuleiro);
        }
    }while(strcmp(nomeJogadorA, nomeJogadorB) == 0);

	//cores nas coordenadas
    printf("\nDeseja usar cores nas coordenadas? (0 - Nao <> 1 - Sim)\n");
    printf("Cores coordenadas: ");
    corCoordenadas = verificarValorIntroduzido(0, 1, 1);
    fflush(stdin);
    puts("");
    
    //procedimento ver quem comeca jogar
    quemComeca(0, 10);
}

//escolhe nome aleatorio para computador
void nomesComputador()
{
	//nomes computador
    const char* nomes[6];
    nomes[0] = "Kz3X";
    nomes[1] = "Mpr7L";
    nomes[2] = "7FwP-";
    nomes[3] = "Jtu90";
    nomes[4] = "TvrZxZ";
    nomes[5] = "7Hj3TSLQ";

	//copiar os nomes pre-definidos
    if(mJogo == 2)
        strcpy(nomeJogadorB, nomes[gerarNumero(0,5)]);
    else if (mJogo == 3)
    {
    	//evitar computador ter 2 nomes iguais
        do
        {
            strcpy(nomeJogadorA, nomes[gerarNumero(0,5)]);
            strcpy(nomeJogadorB, nomes[gerarNumero(0,5)]);
        }while(strcmp(nomeJogadorA, nomeJogadorB) == 0);
    }
}

//ver quem começa a jogar
void quemComeca(int nMenor, int nMaior)
{
    int numero, n1, n2, result1, result2;

	//repetir se houver empate no calculo para ver quem começa 
    do
    {	
    	//inserir numeros e repetir se numeros inseridos forem iguais
        if (mJogo == 1)
        {
        	//repetir ate numeros serem diferentes
            do
            {
            	//inserir numeros
                printf("Insira um numero inteiro de 0 a 10 para saber quem comeca a jogar!\n");
                textoPersonalizado(2,"Jogador $%d %s $7 insira valor: ", corJogadorA, nomeJogadorA);
                n1 = verificarValorIntroduzido(0, 10, 1);
                fflush(stdin);
                textoPersonalizado(2,"Jogador $%d %s $7 insira valor: ", corJogadorB, nomeJogadorB);
                n2 = verificarValorIntroduzido(0, 10, 1);
                fflush(stdin);
				
				//verificar se numeros sao iguais
                if(n1 == n2)
                    textoPersonalizado(0,"Nao podem inserir o mesmo numero! Insiram os numeros novamente!\n");

            }while(n1 == n2);
        }
        else if (mJogo == 2)
        {
            do
            {
                printf("Insira um numero inteiro de 0 a 10 para saber quem comeca a jogar\n");
                textoPersonalizado(2,"Jogador $%d %s $7 insira valor: ", corJogadorA, nomeJogadorA);
                n1 = verificarValorIntroduzido(0, 10, 1);
                n2 = verificarValorIntroduzido(0, 10, 2);
                fflush(stdin);
                textoPersonalizado(0,"O $%d %s $7 inseriu o numero: %d", corJogadorB, nomeJogadorB, n2);

                if(n1 == n2)
                    textoPersonalizado(0,"Nao podem inserir o mesmo numero! Insiram os numeros novamente!\n");

            }while(n1 == n2);
        }
        else
        {
            do
            {
                printf("Ver quem comeca a jogar...\n");
                n1 = verificarValorIntroduzido(0, 10, 2);
                fflush(stdin);
                textoPersonalizado(0,"O $%d %s $7 inseriu o numero: %d", corJogadorA, nomeJogadorA, n1);
                n2 = verificarValorIntroduzido(0, 10, 2);
                fflush(stdin);
                textoPersonalizado(0,"O $%d %s $7 inseriu o numero: %d", corJogadorB, nomeJogadorB, n2);
				
                if(n1 == n2)
                    textoPersonalizado(0,"Nao podem inserir o mesmo numero! Insiram os numeros novamente!\n");

            }while(n1 == n2);
        }
			
        //atribuir valores as  variaveis numero, result1 e result2
        numero = gerarNumero(nMenor, nMaior);
		result1 = 0; 
		result2 = -1;
        
        //mostrar numero gerado pelo computador
		printf("Numero gerado automaticamente: %d\t", numero);
        puts("");

		//calcular quem comeca a jogar
        if(n1 != numero && n2 == numero)
        {
            textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorB, nomeJogadorB);
            jogadorAtual = 'B';
        }
        else if(n1 == numero && n2 != numero)
        {
            textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorA, nomeJogadorA);
            jogadorAtual = 'A';
        }
        else if(n1 < n2)
        {
            if (numero < n1)
            {
                textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorA, nomeJogadorA);
                jogadorAtual = 'A';
            }
            else if (numero > n2)
            {
                textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorB, nomeJogadorB);
                jogadorAtual = 'B';
            }
            else
            {
                result1 = numero - n1;
                result2 = n2 - numero;

                if(result1 > result2)
                {
                    textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorB, nomeJogadorB);
                    jogadorAtual = 'B';
                }
                else if (result1 < result2)
                {
                    textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorA, nomeJogadorA);
                    jogadorAtual = 'A';
                }
                else
                    printf("Empate! Insiram os numeros novamente!\n\n");
            }
        }
        else
        {
            if (numero > n1)
            {
                textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorA, nomeJogadorA);
                jogadorAtual = 'A';
            }
            else if (numero < n2)
            {
                textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorB, nomeJogadorB);
                jogadorAtual = 'B';
            }
            else
            {
                result1 = n1 - numero;
                result2 = numero - n2;

                if (result1 > result2)
                {
                    textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorB, nomeJogadorB);
                    jogadorAtual = 'B';
                }
                else if (result1 < result2)
                {
                    textoPersonalizado(2,"Comeca $%d %s $7 a jogar!\n", corJogadorA, nomeJogadorA);
                    jogadorAtual = 'A';
                }
                else
                    printf("Empate! Insiram os numeros novamente!\n");
            }
        }
        

    }while(n1 < n2 && result1 == result2 || n1 > n2 && result1 == result2);
    puts("");
    
    puts("");

    if(uJogadas == 0)
        system("cls");		
}

//atualiza estado do tabuleiro
void atualizarEcra()
{
    int i, j;

    if (!(tamanho >= 11))     //se o tabuleiro for maior ou igual a 5 quadrados, nao escreve as cordenadas no eixo x
    {
        printf("  ");			 // acertar as coordenadas do eixo x
        for(i=0; i<tamanho; i++) // escrever as coordenadas nas colunas
            printf("%d", i);

        printf("\n\n");
    }

    //imprimir tabuleiro atual
    for (i = 0; i < tamanho; i++)		
    {
        if(tamanho >= 11) 				//se o tabuleiro for maior ou igual a 5 quadrados, nao escreve as cordenadas no eixo y
            printf(" ");
        else
            printf("%d ",i);			//escrever cordenadas no eixo y
		
        for (j = 0; j < tamanho; j++)     
        {
        	//verificar matriz e imprimir caracteres
            if(tabul[i][j] == 'o')
            {
                textcolor(corTabuleiro);
                printf("o");
                textcolor(7);
            }
            else if(tabul[i][j] == 'Q')
            {
                textcolor(corJogadorA);
                putchar(196);
				textcolor(7);
            }
            else if(tabul[i][j] == 'W')
            {
                textcolor(corJogadorA);
                putchar(179);
                textcolor(7);
            }
            else if(tabul[i][j] == 'Z')
            {
                textcolor(corJogadorB);
                putchar(196);
                textcolor(7);
            }
            else if(tabul[i][j] == 'X')
            {
                textcolor(corJogadorB);
                putchar(179);
                textcolor(7);
            }
            else if(tabul[i][j] ==  'A')
            {
                textcolor(corJogadorA);
                printf("%c",  nomeJogadorA[0]);
                textcolor(7);
            }
            else if(tabul[i][j] ==  'B')
            {
                textcolor(corJogadorB);
                printf("%c",  nomeJogadorB[0]);
                textcolor(7);
            }
            else
                printf(" ");
        }
        printf("\n");
    }
}

//funcao principal para jogadas
void jogadas()
{
    int x, y;

	//ciclo para fazer jogadas (enquanto houver jogadas possiveis) 
    do
    {
    	//mostrar numero jogadas possiveis
        printf("\nJogadas possiveis %d\n", jogPos);
        
        //colorir jogador atual e coordenadas x y
        if(jogadorAtual == 'A')
        	 textoPersonalizado(0,"O jogador atual e: $%d %s $7 ", corJogadorA, nomeJogadorA);	
		else
			 textoPersonalizado(0,"O jogador atual e: $%d %s $7 ", corJogadorB, nomeJogadorB);
			 
		textoPersonalizado(2,"Insira as coordenadas $%d X Y: $7 ", gerarNumero(1, 15));
		
        
        //dependendo do modo jogo, utilizador e/ou computador insere coordenadas x y
        if (mJogo == 1)
        {
            x = verificarValorIntroduzido(0, tamanho-1, 1);
            y = verificarValorIntroduzido(0, tamanho-1, 1);
            fflush(stdin);				
        }
        else if (mJogo == 2)
        {
            if (jogadorAtual == 'A')
            {
                x = verificarValorIntroduzido(0, tamanho-1, 1);
                y = verificarValorIntroduzido(0, tamanho-1, 1);
                fflush(stdin);
            }
            else
            {
            	//jogadas pc
                jogadasPC();			//procura melhor jogada pc
                x = coords[1];			//guarda valor da jogada pc na coordenada x
                y = coords[0];			//guarda valor da jogada pc na coordenada y
                printf("%d %d", x, y);  //mostra jogada pc
            }
        }
        else
        {
                jogadasPC();
                x = coords[1];
                y = coords[0];
                printf("%d %d", x, y);
        }
        puts("");

        //ver se posicoes ja tao preenchidas, senao preencher
        if ( tabul[y][x] == 'o' || tabul[y][x] == 'Q' || tabul[y][x] == 'W' || tabul[y][x] == 'Z' || tabul[y][x] == 'X' )
            printf("Jogada Invalida, tente novamente.\n");
        else
        {
            if (x % 2 == 0) 											//se resto x par, guarda no tabuleiro o W ou X (dependendo do jogador atual)
            {															//o W e o X correspondem aos traços verticais
                tabul[y][x] = (jogadorAtual == 'A') ? 'W' : 'X';		//operador ternario para verificar qual e o jogador atual e guardar letra na matriz
                verificacaoJogada(x, y, jogadorAtual);
            }
            else if (x % 2 == 1)
            {
                if (y%2 == 1)											//se resto X e Y impar, jogada invalida pois n pode jogar no centro dos quadrados
                    printf("Jogada invalida\n");
                else													//se resto x impar e Y par, guarda no tabuleiro o Q ou Z (dependendo do jogador atual)
                {														//o Q e o Z correspondem aos traços horizontais
                    tabul[y][x] = (jogadorAtual == 'A') ? 'Q' : 'Z';
                    verificacaoJogada(x, y, jogadorAtual);
                }
            }
        }
        puts("");

        if(uJogadas == 0)
            system("cls");		

        atualizarEcra();

    } while (jogPos > 0);

    if(uJogadas == 0)
        system("cls");

	//procedimento final jogo
    fimJogo();

}

//jogadas do computador
void jogadasPC()
{
    int x, y;
    coords[0] = 0; coords[1] = 0; 			//limpar as coordenadas atuais do pc

    //percorrer tabuleiro para ver se pode fechar quadrado
    for (y = 0; y < tamanho; y++)
        for (x = 0; x < tamanho; x++)
        {
            if ((tabul[y-2][x] == 'Q' || tabul[y-2][x] == 'Z') && (tabul[y-1][x-1] == 'W' || tabul[y-1][x-1] == 'X') && (tabul[y-1][x+1] == 'W' || tabul[y-1][x+1] == 'X') && tabul[y][x] ==  ' ')  // parte cima //lado esquerdo //lado direito)
            {
                coords[0] = y;				//guarda valor coordenada y
                coords[1] = x;				//guarda valor coordenada x
            }
            else if ((tabul[y+2][x] == 'Q' || tabul[y+2][x] == 'Z') && (tabul[y+1][x-1] == 'W' || tabul[y+1][x-1] == 'X') && (tabul[y+1][x+1] == 'W' || tabul[y+1][x+1] == 'X') && tabul[y][x] ==  ' ')     // parte baixo //lado esquerdo //lado direito
            {
                coords[0] = y;
                coords[1] = x;
            }
            else if ((tabul[y][x-2] == 'W' || tabul[y][x-2] == 'X') && (tabul[y-1][x-1] == 'Q' || tabul[y-1][x-1] == 'Z') && (tabul[y+1][x-1] == 'Q' || tabul[y+1][x-1] == 'Z') && tabul[y][x] ==  ' ') //lado esquerdo //parte cima //parte baixo
            {
                coords[0] = y;
                coords[1] = x;
            }
            else if ((tabul[y][x+2] == 'W' || tabul[y][x+2] == 'X') && (tabul[y-1][x+1] == 'Q' || tabul[y-1][x+1] == 'Z') && (tabul[y+1][x+1] == 'Q' || tabul[y+1][x+1] == 'Z') && tabul[y][x] ==  ' ')//lado direito //parte cima //parte baixo
            {
                coords[0] = y;
                coords[1] = x;
            }
        }

    //caso nao possa fechar quadrado gera numeros aleatorios
    if (coords[0] == 0 && coords[1] == 0)
    {
        coords[0] = verificarValorIntroduzido(0, tamanho-1, 2);
        coords[1] = verificarValorIntroduzido(0, tamanho-1, 2);
    }
}

//verificar se ta a fechar algum quadrado
void verificacaoJogada(int x, int y, char jogador)
{
    jogPos--;				//decrementar jogadas possiveis

    //trocar jogador
    if (jogadorAtual == 'A')
        jogadorAtual = 'B';
    else
        jogadorAtual = 'A';

    //verificar se a parte que esta acima da barra (-) introduzida esta preenchida
    if ((tabul[y-2][x] == 'Q' || tabul[y-2][x] == 'Z') && (tabul[y-1][x-1] == 'W' || tabul[y-1][x-1] == 'X') && (tabul[y-1][x+1] == 'W' || tabul[y-1][x+1] == 'X')) 	 //parte cima //lado esquerdo //lado direito)
    {
        tabul[y-1][x] = jogador;			//inserir caracter jogador que fecha quadrado no centro
        atribuirPontos(jogador);			//atribuir pontos jogador que fecha quadrado
    }
    
    //verificar se a parte que esta abaixo da barra (-) introduzida esta preenchida
    if ((tabul[y+2][x] == 'Q' || tabul[y+2][x] == 'Z') && (tabul[y+1][x-1] == 'W' || tabul[y+1][x-1] == 'X') && (tabul[y+1][x+1] == 'W' || tabul[y+1][x+1] == 'X'))     //parte baixo //lado esquerdo //lado direito
    {
        tabul[y+1][x] = jogador;
        atribuirPontos(jogador);
    }
    
    //verificar se o lado esquerdo da barra (|) introduzida esta preenchida
    if ((tabul[y][x-2] == 'W' || tabul[y][x-2] == 'X') && (tabul[y-1][x-1] == 'Q' || tabul[y-1][x-1] == 'Z') && (tabul[y+1][x-1] == 'Q' || tabul[y+1][x-1] == 'Z')) 	//lado esquerdo //parte cima //parte baixo
    {
        tabul[y][x-1] = jogador;
        atribuirPontos(jogador);
    }
    
    //verificar se o lado direito da barra (|) introduzida esta preenchida
    if ((tabul[y][x+2] == 'W' || tabul[y][x+2] == 'X') && (tabul[y-1][x+1] == 'Q' || tabul[y-1][x+1] == 'Z') && (tabul[y+1][x+1] == 'Q' || tabul[y+1][x+1] == 'Z'))		//lado direito //parte cima //parte baixo
    {
        tabul[y][x+1] = jogador;
        atribuirPontos(jogador);
    }
}

//atribuicao de pontos
void atribuirPontos(char jogador)
{
	//verificar jogador que fez ponto
    if(jogador == 'A')
    {
        pontJogadorA++;					//atribuir ponto
        jogadorAtual = 'A';				//quem fez ponto joga outra vez
    }
    else
    {
        pontJogadorB++;
        jogadorAtual = 'B';
    }
}

//Mostra pontuaçoes, tabuleiro como terminou e o vencedor
void fimJogo()
{
	//mostrar tabuleiro
	if(uJogadas == 0)
		printf("Tabuleiro final do jogo:\n\n");
    else
    	printf("\nTabuleiro final do jogo:\n\n");
    	
    //procedimento atualizar Ecra	
	atualizarEcra();

	//mostrar pontuacoes
    printf("\nPontuacoes:\n");
    textoPersonalizado(0,"Jogador $%d %s $7 : %d",corJogadorA, nomeJogadorA, pontJogadorA);
    textoPersonalizado(0,"Jogador $%d %s $7 : %d",corJogadorB, nomeJogadorB, pontJogadorB);
    puts("");

	//mostrar se houve vencedor e em caso afirmativo, quem foi
	if(pontJogadorA == pontJogadorB)
    	printf("O jogo terminou com empate!\n\n");
    else if(pontJogadorA > pontJogadorB)
    {
    	textoPersonalizado(1,"\n\t#%d \t\t\t\t\t\t\t\t#0 \n", corJogadorA);
    	textoPersonalizado(1,"O jogador $%d %s $7 ganhou! Parabens!!!\a", corJogadorA, nomeJogadorA);
    	textoPersonalizado(1,"\n\t#%d \t\t\t\t\t\t\t\t#0 \n", corJogadorA);
	}
    else
    {
    	textoPersonalizado(1,"\n\t#%d \t\t\t\t\t\t\t\t#0 \n", corJogadorB);
		textoPersonalizado(1,"O jogador $%d %s $7 ganhou! Parabens!!!\a\a", corJogadorB, nomeJogadorB);
		textoPersonalizado(1,"\n\t#%d \t\t\t\t\t\t\t\t#0 \n", corJogadorB);	
	}

    system("pause");

    if(uJogadas == 0)
        system("cls");
    else
    	puts("");
}

//limita os valores e caracteres introduzidos pelo utilizador
int verificarValorIntroduzido(int nMenor, int nMaior, int opcao)
{
    int nIntroduzido;

    if (opcao == 0)									//opcao para switch
    {
        while(scanf("%d", &nIntroduzido) != 1)		//verifica se numero inserido esta dentro dos parametros, se tiver continua, senao vai pedir para inserir novamente um numero
        {
            printf("Insira um inteiro: ");
            while(getchar() != '\n');				//le e descarta caracteres ate ler o \n. Serve para limpar input do buffer.
        }
    }
    else if (opcao == 1) 							//opcao para modo jogo Humano
    {
    	//repetir ate utilizador inserir numeros dentro dos parametros
        do
        {
            while(scanf("%d", &nIntroduzido) != 1)
            {
                printf("Insira um inteiro: ");
                while(getchar() != '\n');
            }

            if (nIntroduzido < nMenor || nIntroduzido > nMaior)
                printf("Insira um inteiro entre tamanho min %d e max %d! ", nMenor, nMaior);

        }while(nIntroduzido < nMenor || nIntroduzido > nMaior);
    }
    else if (opcao == 2) 								//opcao para pc
        nIntroduzido = gerarNumero(nMenor, nMaior);		//guarda numero gerado forma automatica na variavel nIntroduzido

    return nIntroduzido;								//retorna numero guardado na variavel nIntroduzido
}

//gera numeros aleatorios
int gerarNumero(int nMinimo, int nMaximo)
{
    return (rand() % (nMaximo - nMinimo + 1)) + nMinimo;		//retorna numero gerado
}

//centrar e colorir texto dentro desta funcao
void textoPersonalizado(int centro, const char *string, ...)
{
   
    char stringFormatada[250];											//string formato final

    //funções que formatam a variável string com argumentos para a variável stringFormatada
    va_list args; 														//recebe lista de argumentos (argumentos vem dos ...)
    va_start(args, string);												//argumentos vao estar no args e variavel string
    vsnprintf(stringFormatada, sizeof(stringFormatada), string, args); 	//vai converter os %s, %d, %i, por os argumentos que foi introduzido ao chamar esta função
    va_end(args); 														//depois de todos argumentos forem passados, reseta o apontador para nulo
	
	int i;
	int compEcra = 80;													//comprimento de ecra
    int compString = strlen(stringFormatada);							//comprimento da string ja formatada
    int espacamento;													//espacos necessarios para centrar texto
	
	//verificar se valor recebido na variavel centro = 1
	if (centro == 1)			  //opcao 1 centra texto
	{	
	
		//percorrer string formatada 
		for (i=0; i <= strlen(stringFormatada) ; i++)
		{
			
			//se encontrar simbolo $ ou # decrementa 3 caracteres (ex: $01) para texto ficar no centro
			if (stringFormatada[i] == '$')				
            	compString-=3;
        	if (stringFormatada[i] == '#')
            	compString-=3;
    	}

    	espacamento = (compString >= compEcra) ? 0 : (compEcra - compString) / 2;		//calcular espacemento necessario para centrar texto
    
        for (i=0; i < espacamento; i++)													//centrar texto
            printf(" ");	
	}
	
	//colorir texto dentro da funçao textoPersonalizado
    for (i=0; i <= strlen(stringFormatada); i++)
    {
    	//verifica se encontrou simbolo $ ($ -> pinta as letras)
        if (stringFormatada[i] == '$')
        {
            textcolor(stringFormatada[i + 2] == ' ' ? stringFormatada[i + 1] - '0' : (stringFormatada[i + 2] - '0') + 10);
            i+=3;
        }
        
        //verifica se encontrou simbolo # (# -> pinta a cor de fundo)
        if (stringFormatada[i] == '#')
        {
        	
        	//verifica o que encontra "duas casas" a frente do simbolo, se for espaço vazio pega no que esta antes do espaco e converte para string, 
			//senao pega no valor encontrado, converte para string e soma 10
            textbackground(stringFormatada[i + 2] == ' ' ? stringFormatada[i + 1] - '0' : (stringFormatada[i + 2] - '0') + 10); // - '0' converte caracter para inteiro
            i+=3;							 				//incrementa i 
        }
        printf("%c", stringFormatada[i]);					//pinta texto dentro da funcao textoPersonalizado
    }
    
    //verificar valor recebido na variavel centro
    if(centro != 2)
	    puts("");
}

//informaçoes sobre jogo, como jogar e regras
void infJogo()
{	
	system("cls");
	
	textoPersonalizado(1,"#15                #0                #15                #0                #15                #0 ");
	textoPersonalizado(1,"$15 Bem vindo!\n");
	textoPersonalizado(1,"#15                #0                #15                #0                #15                #0 ");
	
	//informaçoes sobre jogo
	textoPersonalizado(0,"$15Sobre o jogo: $7 \n");
    printf("Dots and Boxes e um jogo de tabuleiro!\n");
    printf("Inicialmente o tabuleiro contem somente pontos (o).\n");
    printf("Alternadamente cada jogador deve unir dois pontos vizinhos com uma linha\nhorizontal ou vertical.\n");
    printf("Quando um jogador completa um quadrado, escreve a sua inicial no \ninterior do mesmo e joga outra vez.\n");
    printf("O jogo termina quando todos os pontos estiverem ligados entre si.\n");
    printf("O vencedor do jogo e o jogador que formou o maior numero de quadrados.\n");
    printf("Caso os jogadores tenham os mesmos quadrados e considerado empate.\n\n");

    system("pause");
    
    //informaçoes sobre regras
	textoPersonalizado(0,"\n$15Como jogar: $7 \n");
    printf("No ecra inicial insira o numero 1 para comecar.\n");
    printf("Escolha o modo de jogo pretendido.\n");
    printf("Escolha quantos quadrados quer que o tabuleiro tenha no comprimento x altura.\n");
    printf("Escolha se vai desejar ver as jogadas recentes ou nao.\n");
    printf("Introduza o nome ate 7 caracteres.\nFaca a escolha das cores.\n");
    printf("Insira um numero para saber quem comeca a jogar. \n(Comeca a jogar o que tiver o numero mais proximo do numero gerado \naleatoriamente).\n");
    printf("Insira as coordenadas X e Y para realizar as jogadas.\n");
    printf("Importante: Em caso de escolha do modo de jogo PC x PC as cores, os numeros e \nas jogadas serao feitas automaticamente pelo computador.\n\n");

    system("pause");

	//informaçoes sobre como jogar
	textoPersonalizado(0,"\n$15Comandos: $7 \n");
    printf("Para jogar necessita de um teclado para inserir as coordenadas X e Y.\n\n");

    system("pause");
    system("cls");
}
