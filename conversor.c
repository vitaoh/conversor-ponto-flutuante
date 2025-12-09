#include <stdio.h>
#include <string.h>

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
    while (str[i]) {
        if (str[i] >= '0' && str[i] <= '9') { digito = 1; i++; }
        else if (str[i] == '.') {
            if (ponto) return 0;
            ponto = 1;
            i++;
        }
        else return 0;
    }
    return digito;
}

void binario_para_decimal(const char *bin) {
    printf("\nBinario: %s\n", bin);
    
    int sinal = bin[0] - '0';
    int exp_pol = 0;
    for (int i = 1; i <= 8; i++) {
        exp_pol = (exp_pol << 1) | (bin[i] - '0');
    }
    
    float mantissa = 1.0f, peso = 0.5f;
    for (int i = 9; i < 32; i++) {
        if (bin[i] == '1') mantissa += peso;
        peso /= 2.0f;
    }
    
    int exp_real = exp_pol - POLARIZACAO;
    float resultado = mantissa;
    
    if (exp_real > 0) {
        for (int i = 0; i < exp_real; i++) resultado *= 2.0f;
    } else if (exp_real < 0) {
        for (int i = 0; i < -exp_real; i++) resultado /= 2.0f;
    }
    
    if (sinal) resultado = -resultado;
    
    printf("Decimal: %.10f\n\n", resultado);
}

void decimal_para_binario(const char *dec) {
    printf("\nDecimal: %s\n", dec);
    
    float num = 0.0f, divisor = 1.0f;
    int sinal = 0, i = 0;
    
    if (dec[0] == '-') { sinal = 1; i++; }
    else if (dec[0] == '+') i++;
    
    while (dec[i] && dec[i] != '.') {
        if (dec[i] >= '0' && dec[i] <= '9') {
            num = num * 10.0f + (dec[i] - '0');
        }
        i++;
    }
    
    if (dec[i] == '.') {
        i++;
        while (dec[i]) {
            if (dec[i] >= '0' && dec[i] <= '9') {
                divisor *= 10.0f;
                num += (dec[i] - '0') / divisor;
            }
            i++;
        }
    }
    
    if (sinal) num = -num;
    
    if (num == 0.0f) {
        printf("Binario: 0 00000000 00000000000000000000000\n\n");
        return;
    }
    
    float valor = num < 0 ? -num : num;
    int exp_real = 0;
    
    if (valor >= 2.0f) {
        while (valor >= 2.0f) { valor /= 2.0f; exp_real++; }
    } else if (valor < 1.0f) {
        while (valor < 1.0f && exp_real > -126) { valor *= 2.0f; exp_real--; }
    }
    
    valor -= 1.0f;
    
    int mantissa[BITS_MANTISSA] = {0};
    for (int i = 0; i < BITS_MANTISSA; i++) {
        valor *= 2.0f;
        if (valor >= 1.0f) { mantissa[i] = 1; valor -= 1.0f; }
    }
    
    int exp_pol = exp_real + POLARIZACAO;
    
    printf("Binario: %d ", sinal);
    for (int i = BITS_EXPOENTE - 1; i >= 0; i--) printf("%d", (exp_pol >> i) & 1);
    printf(" ");
    for (int i = 0; i < BITS_MANTISSA; i++) printf("%d", mantissa[i]);
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
        printf("\n1. Decimal para Binario\n");
        printf("2. Binario para Decimal\n");
        printf("3. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        if (opcao == 1) {
            printf("\nDecimal: ");
            fgets(entrada, sizeof(entrada), stdin);
            entrada[strcspn(entrada, "\n")] = 0;
            
            if (!validar_decimal(entrada)) {
                printf("Entrada invalida.\n");
                continue;
            }
            
            decimal_para_binario(entrada);
            
        } else if (opcao == 2) {
            printf("\nBinario (32 bits): ");
            fgets(entrada, sizeof(entrada), stdin);
            entrada[strcspn(entrada, "\n")] = 0;
            
            if (!validar_binario(entrada)) {
                printf("Entrada invalida.\n");
                continue;
            }
            
            binario_para_decimal(entrada);
            
        } else if (opcao == 3) {
            printf("\nFim.\n");
            break;
            
        } else {
            printf("Opcao invalida.\n");
        }
    }
    
    return 0;
}