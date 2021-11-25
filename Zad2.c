#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define length 4
#define colors 6

char* get_code(int index) 															//tworzenie kodu o index numerze w kolejności get_code(1)=0000, get_code(7)=0011
{
    char* code = (char*) malloc(length * sizeof(char));
    int i;
    for (i = 0; i < length; i++) 
    {
        code[length - i - 1] = index % colors;
        index /= colors;
    }
    return code;
}

bool* create_all(int n) 															//tablica, która przechowuje wszystkie możliwości na daną chwilę
{														
    bool* S = (bool*) malloc(n * sizeof(bool));
    int i = 0;
    for (i = 0; i < n; i++) 
    {
        S[i] = true;
    }
    return S;
}

void print_one( char* now) 															//drukowanie jednej możliwości
{
    int i;
    for (i = 0; i < length; i++) 
    {
        printf("[%d]", (int) now[i]+1);
    }
}

void print_all(bool* S, int n) 														//drukowanie wszystkich możliwości na dany moment
{ 
    int i;
    for (i = 0; i < n; i++) 
    {
        if (S[i]) 
        {
             char* C = get_code(i);
            print_one(C);
            free(C);
            printf(", ");
        }
    }
    printf("\n");
}

bool isin( char x,    char* code) 													//sprawdzanie czy kod sie zgadza
{
    int i;
    for (i = 0; i < length; i++) 
    {
        if (code[i] == x) return true;
    }
    return false;
}

int howmany( char x,    char* code) 												// zliczanie ile cyfr kody sie zgadza
{
    int r = 0;
    int i;
    for (i = 0; i < length; i++) 
    {
        if (code[i] == x) r++;
    }
    return r;
}

int* analyze( char* code,    char* guess)											//wyiczanie najlepszego możliwego kodu do zgadniecia (który wyeliminuje najwiecej pozostałych możliwości) 
{
    int i;
    int c = 0;
    int p = 0;
    for (i = 0; i < colors; i++) 
    {
        int gue = howmany(i, guess);
        int cod = howmany(i, code);
        if (cod > gue)
            c += gue;
        else
            c += cod;
    }
    for (i = 0; i < length; i++) 
    {
        if (isin(guess[i], code)) {
            if (guess[i] == code[i]) 
            {
				p++;
            }
        }
    }
    int* r = (int*) malloc(sizeof(int) * 2);
    r[0] = c-p;
    r[1] = p;
    return r;
}

int reduce(bool* S,    char* now, int c, int p, int n) 										// usuniecie z S elementów, które nie mogą być kodem (z podaną odpowiedzią)
{
    int x = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (S[i]) {
             char* Code = get_code(i);
            int* r = analyze(Code, now);
            free(Code);
            if (r[0] == c && r[1] == p)
            {
            	x++;
            }
            free(r);
        }
    }
    return x;
}

int fullReduce(bool* S, char* now, int n) 												// ususniecie z S dla wszystkich możliwych odpowiedzi na kod
{
    int responses[13][2] = {{0, 0}, {1, 0}, {0, 1}, {2, 0}, {1, 1}, {0, 2}, {3, 0}, {2, 1}, {1, 2}, {0, 3}, {4, 0}, {3, 1}, {2, 2}};
    int x = 0;
    int index = 0;
    int i;
    for (i = 0; i < 13; i++) 
    {
        int y = reduce(S, now, responses[i][0], responses[i][1], n);
        if (y > x) {
            x = y;
            index = i;
        }
    }
    return x;
}

void SetReduce(bool* S, char* now, int c, int p, int n)													// zamienienie tych kodów, które nie mogą być na false
 {
    // int j = 0;
    int i;
    for (i = 0; i < n; i++)
     {
        char* Code = get_code(i);
        int* r = analyze(Code, now);
        free(Code);
        if (r[0] != c || r[1] != p)
        {
            S[i] = false;
        }
        free(r);
    }
}

char* BestMove(bool* S, int n)																			//wskazanie najlepszego kodu
{
    int best = 0;
    char* Code = get_code(0);
    float bestR = fullReduce(S, Code, n);
    free(Code);
    int i;
    for (i = 1; i < n; i++)
    {
        if (S[i]) 
        {
            Code = get_code(i);
            int x = fullReduce(S, Code, n);
            free(Code);
            // printf("Code, Value: ");
            // print_one(S[i], length);
            // printf(" %d\n", x);
            if (x < bestR) {
	best = i;
	bestR = x;
            }
        }    
    }
    return get_code(best);
}



int main() {
 
    int n = 1296;
    int i;
    bool* S = create_all(n);
    char* move = get_code(7); 
    int newN = n;
    while (n > 0) {
        printf("%d Pozostało\n", newN);
        //print_all(S, n);
        print_one(move);
        printf("\n");
        int c;
        int p;
        printf("Białe: ");
        scanf("%d", &c);
        printf("Czerwone: ");
        scanf("%d", &p);
        if (c == 0 && p == 4) {
            printf("Kod został Odgadnięty!\n");
            break;
        }
        newN = reduce(S, move, c, p, n);
        if (newN == 0) {
            printf("Musiała zajść pomyłka...\n");
            break;
        }
        SetReduce(S, move, c, p, n);
        free(move);
        move = BestMove(S, n);
    }
    free(S);
    free(move);
    return 0;
}
