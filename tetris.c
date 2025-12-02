#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Necessário para a função rand()

// --- Definição da Estrutura da Peça (Struct) ---
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L', etc.
    int id;    // Identificador único da ordem de criação
} Peca;

// --- Configurações da Fila ---
#define CAPACIDADE_MAXIMA 5 // Fila de peças fixada em 5 elementos
Peca fila_pecas[CAPACIDADE_MAXIMA]; // O vetor que armazena as peças

// Variáveis de controle da Fila Circular
int frente = 0;     // Índice da primeira peça (próxima a ser jogada)
int fim = -1;       // Índice da última peça inserida
int contador = 0;   // Número atual de elementos na fila
int proximo_id = 0; // Contador global para gerar IDs únicos (ordem de criação)

// --- Protótipos das Funções ---
Peca gerarPeca();
void inicializarFila();
void exibirFila();
void enqueue(Peca nova_peca);
Peca dequeue();
int menu();

// --- Implementação das Funções Auxiliares ---

/**
 * @brief Gera uma nova peça com tipo e ID únicos.
 * Peças geradas são aleatórias entre 'I', 'O', 'T', 'L'.
 * @return Peca: A nova peça gerada.
 */
Peca gerarPeca() {
    Peca nova;
    // Tipos de peças disponíveis
    const char tipos[] = {'I', 'O', 'T', 'L'}; 
    
    // Escolhe um tipo de forma aleatória
    int indice_tipo = rand() % 4; 
    nova.nome = tipos[indice_tipo];
    
    // Atribui e incrementa o ID global
    nova.id = proximo_id++; 
    
    return nova;
}

/**
 * @brief Inicializa a fila de peças com 5 elementos iniciais.
 */
void inicializarFila() {
    srand(time(NULL)); // Inicializa a semente para a geração de números aleatórios
    printf(" Inicializando a fila de peças futuras com %d elementos...\n", CAPACIDADE_MAXIMA);
    for (int i = 0; i < CAPACIDADE_MAXIMA; i++) {
        // Enfileira as peças iniciais, garantindo que a fila comece cheia
        Peca p = gerarPeca();
        enqueue(p);
    }
    printf(" Fila inicializada. Pronto para jogar!\n");
}

// --- Implementação das Funções da Fila ---

/**
 * @brief Adiciona uma nova peça ao final da fila (Enqueue).
 * Utiliza a lógica da Fila Circular.
 * @param nova_peca: A peça a ser inserida.
 */
void enqueue(Peca nova_peca) {
    // 1. Verifica se a fila está cheia
    if (contador == CAPACIDADE_MAXIMA) {
        printf("\n Fila Cheia! Não é possível inserir a peça [%c %d].\n", nova_peca.nome, nova_peca.id);
        return;
    }
    
    // 2. Move o ponteiro 'fim' para a próxima posição (utilizando o operador módulo para circularidade)
    fim = (fim + 1) % CAPACIDADE_MAXIMA;
    
    // 3. Insere o elemento na nova posição 'fim'
    fila_pecas[fim] = nova_peca;
    
    // 4. Incrementa o contador de elementos
    contador++;

    // A mensagem de sucesso é opcional, mas ajuda na usabilidade
    if (contador < CAPACIDADE_MAXIMA) {
        printf("\n Enqueue: Peça [%c %d] adicionada ao final da fila.\n", nova_peca.nome, nova_peca.id);
    }
}

/**
 * @brief Remove e retorna a peça da frente da fila (Dequeue).
 * Utiliza a lógica da Fila Circular.
 * @return Peca: A peça removida, ou uma peça vazia se a fila estiver vazia.
 */
Peca dequeue() {
    Peca peca_removida;
    
    // 1. Verifica se a fila está vazia
    if (contador == 0) {
        printf("\n Fila Vazia! Não há peças para jogar.\n");
        // Retorna uma peça com ID -1 para sinalizar erro
        peca_removida.id = -1; 
        return peca_removida;
    }
    
    // 2. Pega a peça na posição 'frente' (a próxima a ser jogada)
    peca_removida = fila_pecas[frente];
    
    // 3. Move o ponteiro 'frente' para a próxima posição (circularidade)
    frente = (frente + 1) % CAPACIDADE_MAXIMA;
    
    // 4. Decrementa o contador de elementos
    contador--;
    
    printf("\n Dequeue: Jogada a peça [%c %d].\n", peca_removida.nome, peca_removida.id);
    return peca_removida;
}

/**
 * @brief Exibe o estado atual da fila de peças.
 * Mostra o tipo (nome) e o ID de cada peça na ordem correta (da frente para o fim).
 */
void exibirFila() {
    printf("\n\n===  FILA DE PEÇAS FUTURAS (Próximas %d) ===\n", contador);
    printf("   Capacidade: %d | Ocupação: %d | Livre: %d\n", 
           CAPACIDADE_MAXIMA, contador, CAPACIDADE_MAXIMA - contador);

    if (contador == 0) {
        printf("   A Fila está VAZIA.\n");
        printf("============================================\n");
        return;
    }

    printf("   Fila (Frente -> Fim): ");
    
    // O loop começa em 'frente' e percorre 'contador' vezes
    for (int i = 0; i < contador; i++) {
        // Calcula o índice atual utilizando a lógica circular
        int indice_atual = (frente + i) % CAPACIDADE_MAXIMA; 
        
        printf("[%c %d]", fila_pecas[indice_atual].nome, fila_pecas[indice_atual].id);
        
        // Adiciona uma seta entre as peças, exceto na última
        if (i < contador - 1) {
            printf(" -> ");
        }
    }
    printf("\n============================================\n");
}


// --- Função Principal e Menu de Interação ---

/**
 * @brief Exibe o menu de opções e processa a escolha do jogador.
 * @return int: A opção escolhida.
 */
int menu() {
    int escolha;
    
    printf("\n--- Opções de Ação ---\n");
    printf("1.  Jogar próxima peça (DEQUEUE)\n");
    printf("2.  Inserir nova peça (ENQUEUE)\n");
    printf("0.  Sair\n");
    printf("Escolha uma opção: ");
    
    // Garante que apenas um inteiro seja lido, evitando loop infinito em caso de erro
    if (scanf("%d", &escolha) != 1) {
        while (getchar() != '\n'); // Limpa o buffer de entrada
        return -1; // Retorna valor inválido
    }
    return escolha;
}

int main() {
    int opcao;

    // Inicialização do ambiente de jogo
    inicializarFila();
    
    do {
        // Requisito: Exibir o estado atual da fila antes de cada ação
        exibirFila(); 
        
        opcao = menu();
        
        if (opcao == 1) {
            // Ação 1: Jogar/Remover peça (DEQUEUE)
            Peca jogada = dequeue();
            // A nova peça é inserida automaticamente para repor a fila
            if (jogada.id != -1) { 
                 enqueue(gerarPeca());
            }
        } else if (opcao == 2) {
            // Ação 2: Inserir nova peça (ENQUEUE)
            enqueue(gerarPeca());
        } else if (opcao == 0) {
            printf("\n O jogo terminou. Até a próxima rodada!\n");
        } else {
            printf("\n Opção inválida. Tente novamente.\n");
        }
        
    } while (opcao != 0);

    return 0;
}+
