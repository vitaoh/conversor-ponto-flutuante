#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    unsigned int sinal : 1;
    unsigned int expoente : 8;
    unsigned int mantissa : 23;
} IEEE754;

float parseFloatManual(char *s) {
    float result = 0.0f;
    int sinal = 1, i = 0;
    int depoisPonto = 0;
    float divisor = 1.0f;
    
    if (s[i] == '-') { sinal = -1; i++; }
    else if (s[i] == '+') { i++; }
    
    while (s[i]) {
        if (s[i] == '.') {
            depoisPonto = 1;
            i++;
            continue;
        }
        if (s[i] >= '0' && s[i] <= '9') {
            if (depoisPonto) {
                divisor *= 10.0f;
                result += (s[i] - '0') / divisor;
            } else {
                result = result * 10.0f + (s[i] - '0');
            }
        }
        i++;
    }
    return sinal * result;
}

void decimalParaBinario(float num) {
    IEEE754 ieee;
    unsigned int *ptr = (unsigned int *)&num;
    unsigned int bits = *ptr;
    
    ieee.sinal = (bits >> 31) & 1;
    ieee.expoente = (bits >> 23) & 0xFF;
    ieee.mantissa = bits & 0x7FFFFF;
    
    printf("\n=== CONVERSAO DECIMAL -> BINARIO ===\n");
    printf("Numero decimal: %.10f\n\n", num);
    
    printf("Sinal (1 bit): %u\n", ieee.sinal);
    printf("Expoente (8 bits): ");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (ieee.expoente >> i) & 1);
    }
    printf(" (valor: %u, bias: 127, real: %d)\n", ieee.expoente, ieee.expoente - 127);
    
    printf("Mantissa (23 bits): ");
    for (int i = 22; i >= 0; i--) {
        printf("%d", (ieee.mantissa >> i) & 1);
    }
    printf("\n\n");
    
    printf("Formato IEEE 754 completo:\n");
    printf("%u | ", ieee.sinal);
    for (int i = 7; i >= 0; i--) printf("%d", (ieee.expoente >> i) & 1);
    printf(" | ");
    for (int i = 22; i >= 0; i--) printf("%d", (ieee.mantissa >> i) & 1);
    printf("\n");
}

int ehBinarioValido(char *bin) {
    if (strlen(bin) != 32) return 0;
    for (int i = 0; i < 32; i++) {
        if (bin[i] != '0' && bin[i] != '1') return 0;
    }
    return 1;
}

float binarioParaDecimal(char *bin) {
    if (!ehBinarioValido(bin)) {
        printf("Erro: O numero binario deve ter exatamente 32 bits com 0 e 1!\n");
        return 0.0f;
    }
    
    IEEE754 ieee;
    ieee.sinal = bin[0] - '0';
    ieee.expoente = 0;
    for (int i = 1; i <= 8; i++) {
        ieee.expoente = (ieee.expoente << 1) | (bin[i] - '0');
    }
    ieee.mantissa = 0;
    for (int i = 9; i < 32; i++) {
        ieee.mantissa = (ieee.mantissa << 1) | (bin[i] - '0');
    }
    
    printf("\n=== CONVERSAO BINARIO -> DECIMAL ===\n");
    printf("Numero binario: %s\n\n", bin);
    printf("Sinal: %u\n", ieee.sinal);
    printf("Expoente: %u (bias: 127, real: %d)\n", ieee.expoente, ieee.expoente - 127);
    printf("Mantissa: %u\n\n", ieee.mantissa);
    
    unsigned int bits = 0;
    bits |= (ieee.sinal << 31);
    bits |= (ieee.expoente << 23);
    bits |= ieee.mantissa;
    float *res = (float *)&bits;
    printf("Numero decimal: %.10f\n", *res);
    return *res;
}

int validaDecimal(char *s) {
    int i = 0, ponto = 0;
    if (s[i] == '-' || s[i] == '+') i++;
    if (s[i] == '\0') return 0;
    while (s[i]) {
        if (s[i] == '.') {
            if (ponto) return 0;
            ponto = 1;
        } else if (s[i] < '0' || s[i] > '9') {
            return 0;
        }
        i++;
    }
    return 1;
}

int main() {
    int op;
    char input[100];
    
    printf("=========================================================\n");
    printf(" CONVERSOR DE PONTO FLUTUANTE IEEE 754 (32 bits)\n");
    printf("=========================================================\n\n");
    
    while (1) {
        printf("\n----------------------------------------- \n");
        printf(" MENU PRINCIPAL\n");
        printf("----------------------------------------- \n");
        printf(" 1. Decimal -> Binario (IEEE 754)\n");
        printf(" 2. Binario (IEEE 754) -> Decimal\n");
        printf(" 3. Sair\n");
        printf("----------------------------------------- \n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n');
            printf("Opcao invalida! Tente novamente.\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (op) {
            case 1:
                printf("\nDigite o numero decimal (ex: 12.5, -3.14): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                if (!validaDecimal(input)) {
                    printf("Erro: Numero decimal invalido!\n");
                    break;
                }
                float num = parseFloatManual(input);
                decimalParaBinario(num);
                break;
                
            case 2:
                printf("\nDigite o numero binario de 32 bits (ex: 01000001010010000000000000000000): ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                binarioParaDecimal(input);
                break;
                
            case 3:
                printf("\nPrograma encerrado.\n");
                printf("Obrigado por utilizar!\n");
                return 0;
                
            default:
                printf("Opcao invalida! Digite 1, 2 ou 3.\n");
        }
    }
    return 0;
}