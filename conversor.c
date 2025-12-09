#include <stdio.h>
#include <math.h>

int tamanhoString(char *s)
{
    int tam = 0;
    while (s[tam] != '\0')
        tam++;
    return tam;
}

float parseFloatManual(char *s)
{
    float resultado = 0.0f;
    int sinal = 1, i = 0;
    int depoisDoPonto = 0;
    float divisor = 1.0f;

    if (s[i] == '-')
    {
        sinal = -1;
        i++;
    }
    else if (s[i] == '+')
        i++;

    while (s[i])
    {
        if (s[i] == '.')
        {
            depoisDoPonto = 1;
            i++;
            continue;
        }
        if (s[i] >= '0' && s[i] <= '9')
        {
            if (depoisDoPonto)
            {
                divisor *= 10.0f;
                resultado += (s[i] - '0') / divisor;
            }
            else
                resultado = resultado * 10.0f + (s[i] - '0');
        }
        i++;
    }
    return sinal * resultado;
}

void decimalParaBinario(float numero)
{
    printf("\n  CONVERSAO: DECIMAL -> BINARIO\n\n");
    printf("Numero decimal: %.10f\n\n", numero);

    int sinal = 0;
    if (numero < 0)
    {
        sinal = 1;
        numero = -numero;
    }

    int parteInteira = (int)numero;
    float parteFracionaria = numero - parteInteira;

    int binInt[50], tamInt = 0;
    int temp = parteInteira;
    if (temp == 0)
    {
        binInt[0] = 0;
        tamInt = 1;
    }
    else
    {
        int aux[50], k = 0;
        while (temp > 0)
        {
            aux[k++] = temp % 2;
            temp /= 2;
        }
        for (int i = 0; i < k; i++)
            binInt[i] = aux[k - 1 - i];
        tamInt = k;
    }

    int binFrac[23];
    int tamFrac = 0;
    float frac = parteFracionaria;
    for (int i = 0; i < 23 && frac > 0; i++)
    {
        frac *= 2;
        if (frac >= 1)
        {
            binFrac[i] = 1;
            frac -= 1;
        }
        else
            binFrac[i] = 0;
        tamFrac++;
    }

    int expoenteReal = 0;
    int mantissaBits[23] = {0};
    int posMantissa = 0;

    if (parteInteira >= 1)
    {
        expoenteReal = tamInt - 1;
        for (int i = 1; i < tamInt && posMantissa < 23; i++)
            mantissaBits[posMantissa++] = binInt[i];
        for (int i = 0; i < tamFrac && posMantissa < 23; i++)
            mantissaBits[posMantissa++] = binFrac[i];
    }
    else
    {
        int pos = 0;
        while (pos < tamFrac && binFrac[pos] == 0)
            pos++;
        expoenteReal = -(pos + 1);
        for (int i = pos + 1; i < tamFrac && posMantissa < 23; i++)
            mantissaBits[posMantissa++] = binFrac[i];
    }

    int expoenteBiased = expoenteReal + 127;

    unsigned int mantissa = 0;
    for (int i = 0; i < 23; i++)
        mantissa = (mantissa << 1) | mantissaBits[i];

    printf("Sinal: %d %s\n", sinal, sinal ? "(negativo)" : "(positivo)");
    printf("Expoente: ");
    for (int i = 7; i >= 0; i--)
        printf("%d", (expoenteBiased >> i) & 1);
    printf(" (valor: %d, real: %d)\n", expoenteBiased, expoenteReal);
    printf("Mantissa: ");
    for (int i = 22; i >= 0; i--)
        printf("%d", (mantissa >> i) & 1);
    printf("\n\nIEEE 754 (32 bits):\n");
    printf("%d|", sinal);
    for (int i = 7; i >= 0; i--)
        printf("%d", (expoenteBiased >> i) & 1);
    printf("|");
    for (int i = 22; i >= 0; i--)
        printf("%d", (mantissa >> i) & 1);
    printf("\n");
}

float binarioParaDecimal(char *binario)
{
    int tamanho = tamanhoString(binario);
    if (tamanho != 32)
    {
        printf("\nErro: precisa ter 32 bits!\n");
        return 0.0f;
    }
    for (int i = 0; i < 32; i++)
    {
        if (binario[i] != '0' && binario[i] != '1')
        {
            printf("\nErro: use apenas 0 e 1!\n");
            return 0.0f;
        }
    }

    printf("\n  CONVERSAO: BINARIO -> DECIMAL\n\n");
    printf("Binario: %s\n\n", binario);

    int sinal = binario[0] - '0';
    int expoente = 0;
    for (int i = 1; i <= 8; i++)
        expoente = (expoente << 1) | (binario[i] - '0');
    unsigned int mantissa = 0;
    for (int i = 9; i < 32; i++)
        mantissa = (mantissa << 1) | (binario[i] - '0');

    int expoenteReal = expoente - 127;

    printf("Sinal: %d %s\n", sinal, sinal ? "(negativo)" : "(positivo)");
    printf("Expoente: %d (real: %d)\n", expoente, expoenteReal);
    printf("Mantissa: %u\n\n", mantissa);

    float valorMantissa = 1.0f;
    float peso = 0.5f;
    for (int i = 22; i >= 0; i--)
    {
        if ((mantissa >> i) & 1)
            valorMantissa += peso;
        peso /= 2.0f;
    }

    float resultado = valorMantissa;
    if (expoenteReal > 0)
    {
        for (int i = 0; i < expoenteReal; i++)
            resultado *= 2;
    }
    else if (expoenteReal < 0)
    {
        for (int i = 0; i < -expoenteReal; i++)
            resultado /= 2;
    }

    if (sinal)
        resultado = -resultado;

    printf("Decimal: %.10f\n\n", resultado);

    return resultado;
}

int validaDecimal(char *texto)
{
    int i = 0, temPonto = 0;
    if (texto[i] == '-' || texto[i] == '+')
        i++;
    if (texto[i] == '\0')
        return 0;
    while (texto[i])
    {
        if (texto[i] == '.')
        {
            if (temPonto)
                return 0;
            temPonto = 1;
        }
        else if (texto[i] < '0' || texto[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

int main()
{
    int opcao;
    char entrada[100];

    printf("   CONVERSOR IEEE 754 - PONTO FLUTUANTE (32 bits)\n\n");

    while (1)
    {
        printf("  1 - Decimal para Binario\n");
        printf("  2 - Binario para Decimal\n");
        printf("  3 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            scanf("%*[^\n]");
            scanf("%*c");
            printf("\nOpcao invalida!\n");
            continue;
        }
        scanf("%*[^\n]");
        scanf("%*c");

        switch (opcao)
        {
        case 1:
            printf("\nDigite o numero decimal: ");
            scanf("%s", entrada);
            if (!validaDecimal(entrada))
            {
                printf("\nNumero invalido!\n");
                break;
            }
            float numeroDecimal = parseFloatManual(entrada);
            decimalParaBinario(numeroDecimal);
            break;

        case 2:
            printf("\nDigite o binario (32 bits): ");
            scanf("%s", entrada);
            binarioParaDecimal(entrada);
            break;

        case 3:
            printf("\nFechando o Sistema!\n\n");
            return 0;

        default:
            printf("\nOpcao invalida!\n");
        }
    }

    return 0;
}