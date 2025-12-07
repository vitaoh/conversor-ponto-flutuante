#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct
{
    unsigned int sinal : 1;
    unsigned int expoente : 8;
    unsigned int mantissa : 23;
} IEEE754;

void decimalParaBinario(float numero)
{
    IEEE754 ieee;
    unsigned int *ptr = (unsigned int *)&numero;
    unsigned int bits = *ptr;

    ieee.sinal = (bits >> 31) & 1;
    ieee.expoente = (bits >> 23) & 0xFF;
    ieee.mantissa = bits & 0x7FFFFF;

    printf("\n=== CONVERSAO DECIMAL -> BINARIO ===\n");
    printf("Numero decimal: %.10f\n\n", numero);

    printf("Representacao binaria completa (32 bits):\n");
    printf("Sinal (1 bit): %u\n", ieee.sinal);
    printf("Expoente (8 bits): ");
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (ieee.expoente >> i) & 1);
    }
    printf(" (valor: %u, bias: 127, real: %d)\n", ieee.expoente, ieee.expoente - 127);

    printf("Mantissa (23 bits): ");
    for (int i = 22; i >= 0; i--)
    {
        printf("%d", (ieee.mantissa >> i) & 1);
    }
    printf("\n\n");

    printf("Formato IEEE 754 completo:\n");
    printf("%u | ", ieee.sinal);
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (ieee.expoente >> i) & 1);
    }
    printf(" | ");
    for (int i = 22; i >= 0; i--)
    {
        printf("%d", (ieee.mantissa >> i) & 1);
    }
    printf("\n");
}

float binarioParaDecimal(char *binario)
{
    if (strlen(binario) != 32)
    {
        printf("Erro: O numero binario deve ter exatamente 32 bits!\n");
        return 0.0f;
    }

    for (int i = 0; i < 32; i++)
    {
        if (binario[i] != '0' && binario[i] != '1')
        {
            printf("Erro: O numero binario deve conter apenas 0 e 1!\n");
            return 0.0f;
        }
    }

    IEEE754 ieee;

    ieee.sinal = binario[0] - '0';

    ieee.expoente = 0;
    for (int i = 1; i <= 8; i++)
    {
        ieee.expoente = (ieee.expoente << 1) | (binario[i] - '0');
    }

    ieee.mantissa = 0;
    for (int i = 9; i < 32; i++)
    {
        ieee.mantissa = (ieee.mantissa << 1) | (binario[i] - '0');
    }

    printf("\n=== CONVERSAO BINARIO -> DECIMAL ===\n");
    printf("Numero binario: %s\n\n", binario);

    printf("Sinal: %u\n", ieee.sinal);
    printf("Expoente: %u (bias: 127, real: %d)\n", ieee.expoente, ieee.expoente - 127);
    printf("Mantissa: %u\n\n", ieee.mantissa);

    unsigned int bits = 0;
    bits |= (ieee.sinal << 31);
    bits |= (ieee.expoente << 23);
    bits |= ieee.mantissa;

    float *resultado = (float *)&bits;

    printf("Numero decimal: %.10f\n", *resultado);

    return *resultado;
}

int validarDecimal(char *str)
{
    int i = 0;
    int temPonto = 0;

    if (str[i] == '-' || str[i] == '+')
    {
        i++;
    }

    if (str[i] == '\0')
        return 0;

    while (str[i] != '\0')
    {
        if (str[i] == '.')
        {
            if (temPonto)
                return 0;
            temPonto = 1;
        }
        else if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
        i++;
    }

    return 1;
}

int main()
{
    int opcao;
    char entrada[100];

    printf("=========================================================\n");
    printf("  CONVERSOR DE PONTO FLUTUANTE IEEE 754 (32 bits)\n");
    printf("=========================================================\n\n");

    while (1)
    {
        printf("\n-----------------------------------------\n");
        printf("           MENU PRINCIPAL\n");
        printf("-----------------------------------------\n");
        printf(" 1. Decimal -> Binario (IEEE 754)\n");
        printf(" 2. Binario (IEEE 754) -> Decimal\n");
        printf(" 3. Sair\n");
        printf("-----------------------------------------\n");
        printf("\nEscolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("\nOpcao invalida! Tente novamente.\n");
            continue;
        }

        while (getchar() != '\n')
            ;

        switch (opcao)
        {
        case 1:
        {
            printf("\nDigite o numero decimal (ex: 12.5, -3.14): ");
            fgets(entrada, sizeof(entrada), stdin);
            entrada[strcspn(entrada, "\n")] = 0;

            if (!validarDecimal(entrada))
            {
                printf("\nErro: Numero decimal invalido!\n");
                break;
            }

            float numero = atof(entrada);
            decimalParaBinario(numero);
            break;
        }

        case 2:
        {
            printf("\nDigite o numero binario de 32 bits (ex: 01000001010010000000000000000000): ");
            fgets(entrada, sizeof(entrada), stdin);
            entrada[strcspn(entrada, "\n")] = 0;

            binarioParaDecimal(entrada);
            break;
        }

        case 3:
            printf("\n=========================================\n");
            printf("  Encerrando o programa...\n");
            printf("  Obrigado por utilizar!\n");
            printf("=========================================\n\n");
            return 0;

        default:
            printf("\nOpcao invalida! Escolha 1, 2 ou 3.\n");
        }
    }

    return 0;
}