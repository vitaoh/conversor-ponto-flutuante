#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define BITS_MANTISSA 23
#define BITS_EXPOENTE 8
#define POLARIZACAO 127

int validar_binario(const char *str) {
    if (strlen(str) != 32) return 0;
    for (int i = 0; i < 32; i++) {
        if (str[i] != '0' && str[i] != '1') return 0;
    }
    return 1;
}

int validar_decimal(const char *str) {
    int ponto = 0, digito = 0, i = 0;
    
    if (str[i] == '-' || str[i] == '+') i++;
    
    if (str[i] == '\0') return 0;
    
    if (str[i] == '.') {
        ponto = 1;
        i++;
    }
    
    while (str[i]) {
        if (str[i] >= '0' && str[i] <= '9') {
            digito = 1;
            i++;
        } else if (str[i] == '.') {
            if (ponto) return 0;
            ponto = 1;
            i++;
        } else {
            return 0;
        }
    }
    
    if (str[strlen(str)-1] == '.') return 0;
    
    return digito;
}

void binario_para_decimal(const char *bin) {
    printf("\nBinario: %s\n", bin);
    
    int sinal = bin[0] - '0';
    
    int exp_pol = 0;
    for (int i = 1; i <= 8; i++) {
        exp_pol = (exp_pol << 1) | (bin[i] - '0');
    }
    
    double mantissa = 1.0;
    double peso = 0.5;
    for (int i = 9; i < 32; i++) {
        if (bin[i] == '1') mantissa += peso;
        peso /= 2.0;
    }
    
    if (exp_pol == 255) {
        int mantissa_zero = 1;
        for (int i = 9; i < 32; i++) {
            if (bin[i] == '1') {
                mantissa_zero = 0;
                break;
            }
        }
        if (mantissa_zero) {
            printf("Decimal: %cinfinito\n\n", sinal ? '-' : '+');
        } else {
            printf("Decimal: NaN\n\n");
        }
        return;
    }
    
    if (exp_pol == 0) {
        mantissa -= 1.0;
        exp_pol = -126;
    } else {
        exp_pol -= POLARIZACAO;
    }
    
    double resultado = mantissa * pow(2.0, exp_pol);
    if (sinal) resultado = -resultado;
    
    printf("Decimal: %.6f\n\n", resultado);
}

void decimal_para_binario(const char *dec) {
    printf("\nDecimal: %s\n", dec);
    
    double num = atof(dec);
    
    if (num == 0.0) {
        if (dec[0] == '-') {
            printf("Binario: 10000000000000000000000000000000\n\n");
        } else {
            printf("Binario: 00000000000000000000000000000000\n\n");
        }
        return;
    }
    
    int sinal = num < 0 ? 1 : 0;
    double valor = fabs(num);
    
    int exp_real = 0;
    if (valor >= 2.0) {
        while (valor >= 2.0) {
            valor /= 2.0;
            exp_real++;
        }
    } else if (valor < 1.0) {
        while (valor < 1.0) {
            valor *= 2.0;
            exp_real--;
        }
    }
    
    valor -= 1.0;
    
    int mantissa[BITS_MANTISSA] = {0};
    for (int i = 0; i < BITS_MANTISSA; i++) {
        valor *= 2.0;
        if (valor >= 1.0) {
            mantissa[i] = 1;
            valor -= 1.0;
        }
    }
    
    int exp_pol = exp_real + POLARIZACAO;
    
    if (exp_pol >= 255) {
        printf("Binario: %d1111111100000000000000000000000 (%cinfinito)\n\n", 
               sinal, sinal ? '-' : '+');
        return;
    }
    
    if (exp_pol <= 0) {
        exp_pol = 0;
        printf("Binario: %d00000000", sinal);
        for (int i = 0; i < BITS_MANTISSA; i++) {
            printf("%d", mantissa[i]);
        }
        printf(" (desnormalizado)\n\n");
        return;
    }
    
    printf("Binario: %d", sinal);
    
    for (int i = BITS_EXPOENTE - 1; i >= 0; i--) {
        printf("%d", (exp_pol >> i) & 1);
    }
    
    printf(" ");
    for (int i = 0; i < BITS_MANTISSA; i++) {
        printf("%d", mantissa[i]);
    }
    
    printf(" = %d | ", sinal);
    for (int i = BITS_EXPOENTE - 1; i >= 0; i--) {
        printf("%d", (exp_pol >> i) & 1);
    }
    printf(" | ");
    for (int i = 0; i < BITS_MANTISSA; i++) {
        printf("%d", mantissa[i]);
    }
    printf("\n\n");
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int opcao;
    char entrada[100];
    
    while (1) {
        printf("\nMenu - CONVERSOR\n");
        printf("1. Decimal para Binario\n");
        printf("2. Binario para Decimal\n");
        printf("3. Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                printf("\nDigite o numero decimal: ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = 0;
                
                if (!validar_decimal(entrada)) {
                    printf("Erro: Numero decimal invalido!\n");
                    continue;
                }
                
                decimal_para_binario(entrada);
                break;
                
            case 2:
                printf("\nDigite o binario (32 bits): ");
                fgets(entrada, sizeof(entrada), stdin);
                entrada[strcspn(entrada, "\n")] = 0;
                
                if (!validar_binario(entrada)) {
                    printf("Erro: Binario deve ter exatamente 32 bits!\n");
                    continue;
                }
                
                binario_para_decimal(entrada);
                break;
                
            case 3:
                printf("\nPrograma encerrado.\n");
                return 0;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    }
    
    return 0;
}