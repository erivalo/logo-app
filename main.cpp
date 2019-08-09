#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <math.h>
#include <time.h>

char REPETIR[] = "repetir", AVANZAR[] = "avanzar", RETROCEDER[] = "retroceder", DERECHA[] = "derecha", IZQUIERDA[] = "izquierda";
char VERDE[] = "92", AMARILLO[] = "34", CYAN[] = "96";
float PI = 3.14159265;

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while(goal > clock());
}

/******************************************/
/******************************************/
struct Nodo
{
    void * Entity;
    Nodo * Next;
};

void ConstructorNodo(Nodo** thisNodo)
{
    *thisNodo = (Nodo *) malloc(sizeof(Nodo));
}

void DestructorNodos(Nodo * thisNodo)
{
    if(thisNodo != nullptr)
    {
        DestructorNodos(thisNodo->Next);
        free(thisNodo->Next);
        free(thisNodo->Entity);
    }
}

template <class T> void InsertNodo(Nodo *miLista, T * thisEntity)
{

    if(miLista->Entity == nullptr)
    {
        miLista->Entity = (T *) malloc(sizeof(T));
        memcpy(miLista->Entity, thisEntity, sizeof(T));
    }
    else if(miLista->Next == nullptr)
    {
        Nodo * next;
        ConstructorNodo(&next);
        next->Entity = (T *) malloc(sizeof(T));
        memcpy(next->Entity, thisEntity, sizeof(T));
        miLista->Next = next;
    }
    else
    {
        InsertNodo(miLista->Next, thisEntity);
    }
}
/******************************************/
/******************************************/
struct Tortuga{
    char symbol;
    char* color;
    float posX;
    float posY;
    int grados;
    Nodo* listaC;
    int(*EjecutarTortuga)(Tortuga*,int resultado);
};
/******************************************/
/******************************************/
void gotoxy(int x, int y){
    printf("\x1B[%d;%df", y, x);
    fflush(stdout);
}
/******************************************/
/******************************************/
struct Comando{
    char *tipo;
    void *comando;
    Comando *(*CComando) (Comando*, void *myComando);
    Comando *(*UltimoComando) ();
};

void CComando(Comando** miComando, char *tipo, void *miComandoRecibido)
{
    *miComando = (Comando*) malloc (sizeof(Comando));
    (*miComando)->tipo = tipo;
    (*miComando)->comando = miComandoRecibido;
}

Comando* UltimoComando()
{
    return nullptr;
}
/******************************************/
/******************************************/
struct ComandoA{ //Comando AVANZAR
    int distancia;
    int (*EjecutarComandoAvanzar)(ComandoA*, Tortuga* miTortuga);
};

ComandoA* CComandoA(ComandoA **miComandoA, int distancia){
    *miComandoA = (ComandoA *) malloc (sizeof(ComandoA));
    (*miComandoA)->distancia = distancia;
    return *miComandoA;
}

Comando* AvanzarComando(int distancia){
    ComandoA* miComandoA;
    miComandoA = CComandoA(&miComandoA, distancia);

    Comando* miComando;
    CComando(&miComando, AVANZAR, miComandoA);
    return miComando;
}

void EjecutarComandoAvanzar(ComandoA* MiComandoA, Tortuga* miTortuga)
{
    //printf("\nAvanzo %d espacios\n", MiComandoA->distancia);
    for(int i=0; i <= ((MiComandoA->distancia)-1);i++)
    {
        gotoxy(round(miTortuga->posX) , round(miTortuga->posY) );
        //printf("\033[1;44m%c\033[0m",miTortuga->symbol);
        printf("\033[%sm%c\033[0m", miTortuga->color, miTortuga->symbol);
        delay(3000);

        float x = cos ( miTortuga->grados * PI / 180.0 );
        miTortuga->posX += x;
        float y = sin ( (miTortuga->grados * PI)/180);
        miTortuga->posY +=  y;
    }
    gotoxy(round(miTortuga->posX),round(miTortuga->posY));
    //printf("%c", 'T');
    printf("\033[%sm%c\033[0m", miTortuga->color, 'T');
}
/******************************************/
/******************************************/
struct ComandoV{ //Comando RETROCEDER|VOLVER
    int distancia;
    int (*EjecutarComandoRetroceder)(ComandoV*, Tortuga*);
};

ComandoV *CComandoV(ComandoV **miComandoV, int distancia){
    *miComandoV = (ComandoV *) malloc (sizeof(ComandoV));
    (*miComandoV)->distancia = distancia;
    return *miComandoV;
}

Comando* RetrocederComando(int distancia){
    ComandoV* miComandoV;
    miComandoV = CComandoV(&miComandoV, distancia);

    Comando* miComando;
    CComando(&miComando, RETROCEDER, miComandoV);
    return miComando;
}

void EjecutarComandoRetroceder(ComandoV* MiComandoV, Tortuga* miTortuga)
{
    for(int i=0; i <= ((MiComandoV->distancia)-1);i++)
    {
        gotoxy(round(miTortuga->posX) , round(miTortuga->posY) );
        //printf("\033[1;44m%c\033[0m",miTortuga->symbol);
        printf("\033[%sm%c\033[0m", miTortuga->color, miTortuga->symbol);

        float x = cos ( miTortuga->grados * PI / 180.0 );
        miTortuga->posX -= x;
        float y = sin ( (miTortuga->grados * PI)/180);
        miTortuga->posY -= y;
    }
    gotoxy(round(miTortuga->posX),round(miTortuga->posY));
    //printf("%c", 'T');
    printf("\033[%sm%c\033[0m", miTortuga->color, 'T');

}
/******************************************/
/******************************************/
struct ComandoD{ //Comando DERECHA
    int grados;
    int (*EjecutarComandoDerecha)(ComandoD*, Tortuga*);
};

ComandoD *CComandoD(ComandoD **miComandoD, int grados){
    *miComandoD = (ComandoD *) malloc (sizeof(ComandoD));
    (*miComandoD)->grados = grados;
    return *miComandoD;
}

Comando* DerechaComando(int grados){
    ComandoD* miComandoD;
    miComandoD = CComandoD(&miComandoD, grados);

    Comando* miComando;
    CComando(&miComando, DERECHA, miComandoD);
    return miComando;
}

void EjecutarComandoDerecha(ComandoD* miComandoD, Tortuga* miTortuga)
{
    //printf("\nMe muevo %d grados a la derecha\n", miComandoD->grados);
    miTortuga->grados = miTortuga->grados + miComandoD->grados;
    if(miTortuga->grados >= 360)
    {
        miTortuga->grados = miTortuga->grados - 360;
    }
}
/******************************************/
/******************************************/
struct ComandoI{ //Comando IZQUIERDA
    int grados;
    int (*EjecutarComandoIzquierda)(ComandoI*, Tortuga*);
};

ComandoI *CComandoI(ComandoI **miComandoI, int grados){
    *miComandoI = (ComandoI *) malloc (sizeof(ComandoI));
    (*miComandoI)->grados = grados;
    return *miComandoI;
}

Comando* IzquierdaComando(int grados){
    ComandoI* miComandoI;
    miComandoI = CComandoI(&miComandoI, grados);

    Comando* miComando;
    CComando(&miComando, IZQUIERDA, miComandoI);
    return miComando;
}

void EjecutarComandoIzquierda(ComandoI* miComandoI, Tortuga* miTortuga)
{
    //printf("\nMe muevo %d grados a la izquierda\n", MiComandoI->grados);
    miTortuga->grados = miTortuga->grados - miComandoI->grados;
    if(miTortuga->grados < 0)
    {
        miTortuga->grados = miTortuga->grados + 360;
    }
}
/******************************************/
/******************************************/
struct ComandoR{ //Comando REPETIR
    int noRepeticiones;
    Nodo *listaComandos;
    ComandoR *(*CComandoR) (ComandoR*, int noRepeticiones);
    void (*EjecutarComandoRepetir)(ComandoR*);
};
ComandoR *CComandoR(ComandoR** miComandoR, int noRepeticiones)
{
    *miComandoR = (ComandoR*) malloc (sizeof(ComandoR));
    (*miComandoR)->noRepeticiones = noRepeticiones;
    ConstructorNodo(&(*miComandoR)->listaComandos);
    return *miComandoR;
}

Comando* RepetirComando(int noRepeticiones, ...)
{
    int control = 0, contador = 0;
    ComandoR *miComandoR;
    miComandoR = CComandoR(&miComandoR, noRepeticiones);
    va_list argsList;
    va_start(argsList, noRepeticiones);
    while (control != -1){
        if(contador > 0){
            Comando *miComando;
            miComando = va_arg(argsList, Comando*);
            if(miComando != nullptr){
                InsertNodo<Comando>(miComandoR->listaComandos, miComando);
            } else{
                control = -1;
            }
        }
        contador++;
    }
    va_end(argsList);
    Comando* miComandoAux;
    CComando(&miComandoAux, REPETIR, miComandoR);
    return miComandoAux;
}

int EjecutarComandoRepetir(ComandoR *miComandoR, Tortuga* miTortuga, int noRepeticiones = 0)
{
    //int noRepeticionesAux = miComandoR->noRepeticiones;
    if(noRepeticiones < miComandoR->noRepeticiones){
        if(noRepeticiones == 50){ miTortuga->color = VERDE;}
        if(noRepeticiones == 100){ miTortuga->color = CYAN;}
        if(noRepeticiones == 150){ miTortuga->color = AMARILLO;}
        Nodo *lista = miComandoR->listaComandos;
        Comando* miComandoVariable;
        while(lista != nullptr){
            miComandoVariable = (Comando *) lista->Entity;
            if(miComandoVariable != nullptr){
                InsertNodo<Comando>(miTortuga->listaC, miComandoVariable);
                char *tipoComando = miComandoVariable->tipo;
                if(strcmp(tipoComando, REPETIR) == 0){
                    ComandoR *miComandoRAnidado = (ComandoR*) miComandoVariable->comando;
                    EjecutarComandoRepetir(miComandoRAnidado, miTortuga);
                } else if(strcmp(tipoComando, AVANZAR) == 0){
                    ComandoA *miComandoA = (ComandoA*) miComandoVariable->comando;
                    EjecutarComandoAvanzar(miComandoA, miTortuga);
                } else if(strcmp(tipoComando, DERECHA) == 0){
                    ComandoD *miComandoD = (ComandoD*) miComandoVariable->comando;
                    EjecutarComandoDerecha(miComandoD, miTortuga);
                } else if(strcmp(tipoComando, IZQUIERDA) == 0){
                    ComandoI *miComandoI = (ComandoI*) miComandoVariable->comando;
                    EjecutarComandoIzquierda(miComandoI, miTortuga);
                } else if(strcmp(tipoComando, RETROCEDER) == 0){
                    ComandoV *miComandoV = (ComandoV*) miComandoVariable->comando;
                    EjecutarComandoRetroceder(miComandoV, miTortuga);
                }
            }
            lista = lista->Next;
        }
        noRepeticiones++;
        EjecutarComandoRepetir(miComandoR, miTortuga, noRepeticiones);
    }
    return 0;
}
/******************************************/
/******************************************/
Tortuga *CTortuga(Tortuga **miTortuga, char symbol){
    *miTortuga = (Tortuga*) malloc (sizeof(Tortuga));
    (*miTortuga)->symbol = symbol;
    (*miTortuga)->grados = 0;
    (*miTortuga)->posX = 50;
    (*miTortuga)->posY = 70;
    (*miTortuga)->color = CYAN;
    ConstructorNodo(&(*miTortuga)->listaC);
    return *miTortuga;
}

int EjecutarTortuga(Tortuga *miTortuga, /*Comando *miComando,*/ ...){
    int control = 0, contador = 0;
    va_list argsList;
    va_start(argsList, miTortuga);
    while (control != -1){
        if(contador > 0){
            Comando *miComandoVariable;
            miComandoVariable = va_arg(argsList, Comando*);
            if(miComandoVariable != nullptr){
                InsertNodo<Comando>(miTortuga->listaC, miComandoVariable);
                char *tipoComando = miComandoVariable->tipo;
                if(strcmp(tipoComando, REPETIR) == 0){
                    ComandoR *miComandoR = (ComandoR*) miComandoVariable->comando;
                    EjecutarComandoRepetir(miComandoR, miTortuga);
                } else if(strcmp(tipoComando, AVANZAR) == 0){
                    ComandoA *miComandoA = (ComandoA*) miComandoVariable->comando;
                    EjecutarComandoAvanzar(miComandoA, miTortuga);
                } else if(strcmp(tipoComando, DERECHA) == 0){
                    ComandoD *miComandoD = (ComandoD*) miComandoVariable->comando;
                    EjecutarComandoDerecha(miComandoD, miTortuga);
                } else if(strcmp(tipoComando, RETROCEDER) == 0){
                    ComandoV *miComandoV = (ComandoV*) miComandoVariable->comando;
                    EjecutarComandoRetroceder(miComandoV, miTortuga);
                }
            } else{
                control = -1;
            }
        }
        contador++;
    }
    va_end(argsList);
    return 0;
}
/******************************************/
/******************************************/
struct Logo{
    int ancho;
    int alto;
    Nodo *listaT;// Lista de tortugas
    Tortuga *(*CrearTortugaLogo)(Logo*, char symbol);
    void (*LimpiarLogo) (Logo*);
};

void DibujarLogo(Logo* miLogo)
{
    /*for(int i=1;i <= miLogo->alto;i++)
    {
        for(int j=1;j <= miLogo->ancho;j++)
        {
            gotoxy(i,j);
            if(i%2 == 0)
            printf("-");
            else
            printf("+");
        }
    }*/
}

Logo* CLogo (Logo **miLogo, int ancho, int alto){
    *miLogo = (Logo*) malloc (sizeof(Logo));
    ConstructorNodo(&(*miLogo)->listaT);
    (*miLogo)->ancho = ancho;
    (*miLogo)->alto = ancho;
    DibujarLogo(*miLogo);
    return *miLogo;
}

void DLogo(Logo *miLogo)
{
    Tortuga *miTortuga = (Tortuga*) miLogo->listaT->Entity;
    DestructorNodos(miTortuga->listaC);
    DestructorNodos(miLogo->listaT);
    free(miLogo);
}

Tortuga *CrearTortugaLogo(Logo *miLogo, char symbol){
    Tortuga *miTortuga;
    miTortuga = CTortuga(&miTortuga, symbol);
    InsertNodo<Tortuga>(miLogo->listaT, miTortuga);
    return miTortuga;
}

void LimpiarLogo(Logo* miLogo)
{
    for(int i = 0; i < miLogo->alto; i++){
        printf("\n");
    }
    DibujarLogo(miLogo);
}
/******************************************/
/******************************************/
void TestCase01()
{
    int ancho = 100;
    int alto = 100;
    Logo *logo;
    logo = CLogo(&logo, ancho, alto);
    Tortuga *tortuga;
    tortuga = CrearTortugaLogo(logo, '*');
    EjecutarTortuga
    (
        tortuga,
        RepetirComando
        (
            4,
            AvanzarComando(10),
            IzquierdaComando(90),
            UltimoComando()
        ),
        UltimoComando()
    );

    getchar();

    LimpiarLogo(logo);
    tortuga->color = AMARILLO;
    EjecutarTortuga
    (
        tortuga,
        RepetirComando
        (
            8,
            AvanzarComando(5),
            RetrocederComando(5),
            IzquierdaComando(45),
            UltimoComando()
        ),
        UltimoComando()
    );
    getchar();
    DLogo(logo);
}

void TestCase02()
{
    int ancho = 150;
    int alto = 150;
    Logo *logo;
    logo = CLogo(&logo, ancho, alto);
    Tortuga *tortuga;
    tortuga = CrearTortugaLogo(logo, '*');
    tortuga->color = AMARILLO;
    EjecutarTortuga
    (
        tortuga,
        AvanzarComando(20),
        RepetirComando
        (
            10,
            AvanzarComando(1),
            IzquierdaComando(45),
            RepetirComando
            (
                180,
                AvanzarComando(1),
                IzquierdaComando(2),
                UltimoComando()
            ),
            UltimoComando()
        )
    );
    getchar();

    DLogo(logo);
}

int main()
{
    //TestCase01();
    TestCase02();
    /*gotoxy(5, 7);
    printf("M");
    getchar();*/
    getchar();
    return 0;

}
