#include <stdio.h>
#include <stdlib.h>
#include <aclapi.h>
#include <string.h>
#include <windows.h>
#define Menu 4
#define CenaN 20
#define CenaU
#define MAX 400

/// Liga pilkarska

void gotoxy(int x, int y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

typedef struct list;


typedef struct rezerwacja
{
    char imie[100];
    char nazwisko[100];
    int  iloscN;
    int  iloscU;
    char mecz[100];

} rezerwacja;

typedef struct Baza
{
   int id;

   rezerwacja baza[MAX];
} Baza;

Baza Baza_danych;




typedef struct druzyna {
    int wygrane;
    int remisy;
    int przegrane;
    int pozycjaTabela;
    char nazwa[256];
    char liga[256];


} druzyna;

typedef struct zawodnik {
    int wiek;
    int bramki;
    int asysty;
    char imie[256];
    char nazwisko[256];

} zawodnik;


typedef union liga {
    zawodnik *Z;
    druzyna *D;


} liga;

typedef enum {
    Node, Druzyna, Zawodnik
} Typ;

typedef struct node {
    struct list *L;
    int value;
    struct node *next, *prev;
    liga Q;
    Typ typ;
} node;

typedef struct list {
    node *first, *last;
    int size;
} list;

void list_init(list *L) {
    (*L).size = 0;/// L -> size
    L->first = NULL;
    L->last = NULL;
}

node *create_node(int value) {
    node *N;
    N = malloc(sizeof(node));
    N->next = NULL;
    N->prev = NULL;
    N->value = value;
    N->L = NULL;
    N->typ = Node;
    return N;
}


node *create_zawodnik(int value, char imie[40], char naziwsko[40], int wiek, int bramki, int asysty) {
    node *N = create_node(value);
    N->typ = Zawodnik;
    zawodnik *Z = malloc(sizeof(zawodnik));
    strcpy(Z->imie, imie);
    strcpy(Z->nazwisko, naziwsko);
    Z->wiek = wiek;
    Z->bramki = bramki;
    Z->asysty = asysty;


    N->Q.Z = Z;
    return N;
}

node *
creat_druzyna(int value, char nazwa[40], char liga[40], int pozycjaTabela, int wygrana, int remisy, int przegrane) {

    node *N = create_node(value);
    N->typ = Druzyna;
    druzyna *D = malloc(sizeof(druzyna));
    strcpy(D->nazwa, nazwa);
    strcpy(D->liga, liga);
    D->pozycjaTabela = pozycjaTabela;
    D->wygrane = wygrana;
    D->remisy = remisy;
    D->przegrane = przegrane;

    N->Q.D = D;

    return N;
}


void add2end(list *L, node *N) {
    N->L = L;
    if (L->size == 0) {
        L->size = 1;
        L->first = N;
        L->last = N;
    } else {
        L->size++;

        (L->last)->next = N;

        N->prev = L->last;

        L->last = N;
    }
};


void add2begin(list *L, node *N) {
    N->L = L;
    if (L->size == 0) {
        L->size = 1;
        L->first = N;
        L->last = N;
    } else {
        L->size++;

        L->first->prev = N;

        N->next = L->first;

        L->first = N;
    }
};

node *znajdz(list L, int v) {
    node *temp;
    for (temp = L.first; temp; temp = temp->next)
        if (temp->value == v) return temp;
    return NULL;
}

void printZawodnik(zawodnik Z) {
    printf("Zawodnik:\n>Imie: %s\n>Nazwisko: %s\n>Wiek: %d\n>Bramki: %d\n>Asysty: %d\n", Z.imie, Z.nazwisko, Z.wiek,
           Z.bramki, Z.asysty);

}

void printDruzyna(druzyna D) {
    printf("Druzyna o nazwie: %s\n> grajacy w lidze: %s\n> Pozycja: %d\n> Wygrane: %d\n> Remisy: %d\n> Przegrane: %d\n",
           D.nazwa, D.liga, D.pozycjaTabela, D.wygrane, D.remisy, D.przegrane);
}


void save2file_node(FILE *F, node *N) {
    fprintf(F, "node\n");
    if (N->typ == Node) {
        fprintf(F, "Node \nvalue: %d\n", N->value);
    } else if (N->typ == Druzyna) {

        fprintf(F, "Druzyna \nvalue: %d\nnazwa: %s\nliga: %s\nPozycja: %d\nwygrane: %d\nremisy: %d\nprzegrana: %d\n ",
                N->value, N->Q.D->nazwa, N->Q.D->liga, N->Q.D->pozycjaTabela, N->Q.D->wygrane, N->Q.D->remisy,
                N->Q.D->przegrane);
    } else if (N->typ == Zawodnik) {

        fprintf(F, "Zawodnik \nvalue: %d\nimie: %s\nnazwisko: %s\nwiek: %d\nbramki: %d\nasysty: %d",
                N->value, N->Q.Z->imie, N->Q.Z->nazwisko, N->Q.Z->wiek, N->Q.Z->bramki, N->Q.Z->asysty);
    } else { printf("%d !!!!!", N->typ); };


}


void save2file_list(list L) {
    FILE *F;
    F = fopen("baza_1.txt", "w");
    fprintf(F, "list: %d\n", L.size);
    node *temp;
    temp = L.first;
    while (temp) {
        save2file_node(F, temp);
        temp = temp->next;
    }
    fclose(F);
}

void print_node(node N) {
    printf(" v:%d\n", N.value);
    switch (N.typ) {
        case Node:
            break;
        case Druzyna:
            printDruzyna(*(N.Q.D));
            break;
        case Zawodnik:
            printZawodnik(*(N.Q.Z));


    };
}


void print_lista(list L) {
   // printf("To jest lista, ma elementow %d\n", L.size);
    system("cls");
    node *temp;

    temp = L.first;
    while (temp) {
        print_node((*temp));
        temp = temp->next;
    }

}


node *openfile_node(FILE *F) {

    char text[256];
    fscanf(F, "%s", &text);
    if (strcmp(text, "Node") == 0) {
        fscanf(F, "%s", &text);
        if (strcmp(text, "value:") == 0) {
            int v;
            fscanf(F, "%d", &v);
            return create_node(v);

        } else {
            printf("%s", text);
            exit(200);
        }
    } else if (strcmp(text, "Druzyna") == 0) {
        int value, wygrane, remis, przegrane, pozycjaTabeli;
        char nazwa[40];
        char liga[40];


        fscanf(F, "%s", &text);
        if (strcmp(text, "value:") == 0) {
            fscanf(F, "%d", &value);
        } else exit(101);

        fscanf(F, "%s", &text);
        if (strcmp(text, "nazwa:") == 0) {
            fscanf(F, "%s", &nazwa);
        } else exit(102);


        fscanf(F, "%s", &text);
        if (strcmp(text, "liga:") == 0) {
            fscanf(F, "%s", &liga);
        } else exit(1032);


        fscanf(F, "%s", &text);
        if (strcmp(text, "Pozycja:") == 0) {
            fscanf(F, "%d", &pozycjaTabeli);
        } else exit(104);


        fscanf(F, "%s", &text);
        if (strcmp(text, "wygrane:") == 0) {
            fscanf(F, "%d", &wygrane);
        } else exit(105);

        fscanf(F, "%s", &text);
        if (strcmp(text, "remisy:") == 0) {
            fscanf(F, "%d", &remis);
        } else exit(106);

        fscanf(F, "%s", &text);
        if (strcmp(text, "przegrana:") == 0) {
            fscanf(F, "%d", &przegrane);
        } else exit(107);

        return creat_druzyna(value, nazwa, liga, pozycjaTabeli, wygrane, remis, przegrane);

    } else if (strcmp(text, "Zawodnik") == 0) {

        int wiek, bramki, asysty, value;

        char imie[40], nazwisko[40];

        fscanf(F, "%s", &text);
        if (strcmp(text, "value:") == 0) {
            fscanf(F, "%d", &value);
        } else exit(101);
        fscanf(F, "%s", &text);
        if (strcmp(text, "imie:") == 0) {
            fscanf(F, "%s", &imie);
        } else exit(102);
        fscanf(F, "%s", &text);
        if (strcmp(text, "nazwisko:") == 0) {
            fscanf(F, "%s", &nazwisko);
        } else exit(1031); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        fscanf(F, "%s", &text);
        if (strcmp(text, "wiek:") == 0) {
            fscanf(F, "%d", &wiek);
        } else exit(104);
        fscanf(F, "%s", &text);
        if (strcmp(text, "bramki:") == 0) {
            fscanf(F, "%d", &bramki);
        } else exit(105);
        fscanf(F, "%s", &text);
        if (strcmp(text, "asysty:") == 0) {
            fscanf(F, "%d", &asysty);
        } else exit(106);
        return create_zawodnik(value, imie, nazwisko, wiek, bramki, asysty);
    } else { printf("!!!!!!!!!!!"); }

}

void openfile_list(list *L) {
    FILE *F;
    F = fopen("baza_1.txt", "r");
    char text[256];
    fscanf(F, "%s", &text);
    if (strcmp(text, "list:") != 0)exit(100);
    int s;
    fscanf(F, "%d", &s);
    for (; s > 0; s--) {
        fscanf(F, "%s", &text);
        //printf("::%s::",text);

        if (strcmp(text, "node") == 0) {
            add2end(L, openfile_node(F));
        } else {
            printf("%s", text);
            exit(200);
        }
    }
    fclose(F);
}


void free_list(list *L) {
    node *temp, *del;
    temp = L->first;
    while (temp) {
        del = temp;
        temp = temp->next;
        free(del);
    }
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
}


void menu(int aktywny)
{
    void cos(int i)
    {

        if (i==aktywny){printf(">");}else{printf(" ");}
    }

    cos(0);printf("0. Wyjscie\n");
    cos(1);printf("1. Druzyna\n");
    cos(2);printf("2. Zawodnicy\n");
    cos(3);printf("3. Mecze\n");

}


void menu_sub1(int nr)
{
    void cos(int i)
    {
        gotoxy(20,2+i);
        if (i==nr){printf(">");}else{printf(" ");}
    }


    cos(0);printf("0. Wyjscie\n");
    cos(1);printf("1. Dodaj Zawodnika\n");
    cos(2);printf("2. Wyswietl Zawodnikow\n\n");
    cos(3);printf("3. Wyszukaj Zawodnika\n");
    cos(4);printf("4. Sortuj (Po Imieniu)\n");
    cos(5);printf("5. Sortuj (Po Naziwsku)\n");
 //  cos(6);printf("6. Wyswietl Zawodnikow\n");



}

void menu_sub2(int nr)
{
    void cos(int i)
    {
        gotoxy(20,2+i);
        if (i==nr){printf(">");}else{printf(" ");}
    }


    cos(0);printf("0. Wyswietl rezerwacje\n");
    cos(1);printf("1. Rezerwacja miejsca na mecz\n");
    cos(2);printf("2. Usuwanie Rezerwacji\n");
    cos(3);printf("3. Najlepsi Strzelcy sezonu\n");
    cos(4);printf("4. Tabela\n");
    cos(5);printf("5. Wyswietl cala baze Ligi\n");
    cos(6);printf("6. Liga hiszpanska\n");
    cos(7);printf("7. Liga Gdynia\n");




}


void menu_sub(int nr)
{

    void cos(int i)
    {
        gotoxy(20,2+i);
        if (i==nr){printf(">");}else{printf(" ");}
    }

     cos(0);printf("0. Wyjscie\n");
     cos(1);printf("1. Dodaj Druzyne\n");
     cos(2);printf("2. Wyswietl Druzyne\n");
     cos(3);printf("3. Wyszukaj Druzyne\n");
     cos(4);printf("4. Sortuj Druzyne\n");
     //cos(5);printf("6. Wyswietl Druzyne\n");

}

void menu_clr()
{

    int i;
    for(i=0;i<=8;i++)
      {gotoxy(20,2+i);
       printf("                              ");
      }
}

void malowanie(int Old, int New)
{
   gotoxy(0,Old);
   printf(" ");
   gotoxy(0,New);
   printf(">");
   gotoxy(0,menu+1);
}

void malowanie_sub(int Old, int New)
{
   gotoxy(20,Old+2);
   printf(" ");
   gotoxy(20,New+2);
   printf(">");
   gotoxy(20,menu+1+2);
}

void strzalki(int sub_menu,int * sub_opcja)
{
                               int zakoncz_wybor=0;
                               int old_sub_menu=0;

                               int zakoncz_sub_menu=-1;
                               menu_sub(sub_menu);

                               while (zakoncz_sub_menu)
                               {


                                while (!_kbhit()){};
                                          char s = _getch();
                                          if (s == -32)
                                             {

                                                 ///przycisk funkcyjny
                                                 s = _getch();
                                                 switch (s)
                                                 {
                                                     case 80:
                                                         sub_menu++;
                                                         if (sub_menu>=3) {sub_menu=0;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 72:
                                                         sub_menu--;
                                                         if (sub_menu<0) {sub_menu=2;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 75:
                                                          printf("wybor to:");
                                                           zakoncz_sub_menu=0;
                                                           menu_clr();

                                                     break;
                                                 }
                                             }
                                             if (s==13){
                                                      zakoncz_wybor=1;
                                                      zakoncz_sub_menu=0;
                                                      * sub_opcja = sub_menu;


                                             }

                               }
}

int menu_strzalki(int * pocztakowa_pozyjca, int * sub_opcja)
{
    int opcja_menu = * pocztakowa_pozyjca;



    menu(opcja_menu);



    int zakoncz_wybor=0;
    int old=opcja_menu;
      while (zakoncz_wybor!=1)
      {

          while (!_kbhit()){};
          char c = _getch();
          if (c == -32)
             {
                 ///przycisk funkcyjny
                 c = _getch();
                 switch (c)
                 {
                     case 80: opcja_menu++;
                     if (opcja_menu>=Menu) {opcja_menu=0;}
                     malowanie(old,opcja_menu);old=opcja_menu;
                     break;
                     case 72: opcja_menu--;if (opcja_menu<0) {opcja_menu=Menu-1;}
                     malowanie(old,opcja_menu);old=opcja_menu;
                     break;
                     case 77:
                         if (opcja_menu==1 )
                         {
                               int sub_menu=0;
                               int old_sub_menu=0;

                               int zakoncz_sub_menu=-1;
                               menu_sub(sub_menu);

                               while (zakoncz_sub_menu)
                               {


                                while (!_kbhit()){};
                                          char s = _getch();
                                          if (s == -32)
                                             {

                                                 ///przycisk funkcyjny
                                                 s = _getch();
                                                 switch (s)
                                                 {
                                                     case 80:
                                                         sub_menu++;
                                                         if (sub_menu>=5) {sub_menu=0;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 72:
                                                         sub_menu--;
                                                         if (sub_menu<0) {sub_menu=4;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 75:
                                                           zakoncz_sub_menu=0;
                                                           menu_clr();

                                                     break;
                                                 }
                                             }
                                             if (s==13){
                                                      zakoncz_wybor=1;
                                                      zakoncz_sub_menu=0;
                                                      * sub_opcja = sub_menu;


                                             }

                               }

                         }
                        else   if (opcja_menu==2 )
                         {
                             //menu_sub1
                               int sub_menu=0;
                               int old_sub_menu=0;

                               int zakoncz_sub_menu=-1;
                               menu_sub1(sub_menu);

                               while (zakoncz_sub_menu)
                               {


                                while (!_kbhit()){};
                                          char s = _getch();
                                          if (s == -32)
                                             {

                                                 ///przycisk funkcyjny
                                                 s = _getch();
                                                 switch (s)
                                                 {
                                                     case 80:
                                                         sub_menu++;
                                                         if (sub_menu>5) {sub_menu=0;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 72:
                                                         sub_menu--;
                                                         if (sub_menu<0) {sub_menu=5;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 75:
                                                           zakoncz_sub_menu=0;
                                                           menu_clr();

                                                     break;
                                                 }
                                             }
                                             if (s==13){
                                                      zakoncz_wybor=1;
                                                      zakoncz_sub_menu=0;
                                                      * sub_opcja = sub_menu;


                                             }

                               }

                         }

                        else   if (opcja_menu==3 )
                         {
                             //menu_sub1
                               int sub_menu=0;
                               int old_sub_menu=0;

                               int zakoncz_sub_menu=-1;
                               menu_sub2(sub_menu);

                               while (zakoncz_sub_menu)
                               {


                                while (!_kbhit()){};
                                          char s = _getch();
                                          if (s == -32)
                                             {

                                                 ///przycisk funkcyjny
                                                 s = _getch();
                                                 switch (s)
                                                 {
                                                     case 80:
                                                         sub_menu++;
                                                         if (sub_menu>7) {sub_menu=0;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 72:
                                                         sub_menu--;
                                                         if (sub_menu<0) {sub_menu=7;}
                                                          malowanie_sub(old_sub_menu,sub_menu);
                                                          old_sub_menu=sub_menu;
                                                          break;
                                                      case 75:
                                                           zakoncz_sub_menu=0;
                                                           menu_clr();

                                                     break;
                                                 }
                                             }
                                             if (s==13){
                                                      zakoncz_wybor=1;
                                                      zakoncz_sub_menu=0;
                                                      * sub_opcja = sub_menu;


                                             }

                               }

                         }



                     break;
                 }
             }
          if (c == 13) { if (opcja_menu!=2) zakoncz_wybor=1;};


      }
      * pocztakowa_pozyjca = opcja_menu;
}

void switch_menu(list M)
{

//WyswietlanieTabeli

   int opcja_menu;
   int zakoncz=0;
   opcja_menu=0;
   while (zakoncz!=1)
   {

   int sub_opcja;
      menu_strzalki(&opcja_menu, &sub_opcja);

      if(opcja_menu==3){

      switch(sub_opcja)
      {
          case 0:   system("cls");
                    wypiszRezerwacje();
                    system("pause");
              break;
          case 1: system("cls");
              dodawanieRezerwacji(M);
                  system("pause");
             break;
          case 2: system("cls");
                  usunRezerwacje();
                  system("pause");
             break;
          case 3: sort_list_strzelcy(M);
                  save2file_list(M);
                  print_lista_Zawodnikow(M);
                  system("pause");
             break;
          case 4: WyswietlanieTabeli(M);
                  print_lista_Druzyny(M);
                  system("pause");
                   break;
          case 5: panelAdministracyjny(M);
                  system("pause");
                   break;
          case 6: wyswietlenieLigi1(M);
                  system("pause");
             break;
          case 7: wyswietlenieLigi2(M);
                 system("pause");
             break;
          default:
             printf("bledna opcja\n");
             system("pause");


      };
}



if(opcja_menu==2){

      switch(sub_opcja)
      {
          case 0: system("exit");
              break;
          case 1: system("cls");
              dodawanieMenuZaowdnika(M);
                  system("pause");
             break;
          case 2: print_lista_Zawodnikow(M);
                  system("pause");
             break;
          case 3: WyszukiwanieZawodnikaPoImieniu(M);
                  system("pause");
             break;
          case 4:
                  sort_list_zawodnicy_imie(M);
                 // save2file_list(M);
                  print_lista_Zawodnikow(M);
                  system("pause");

                 break;
           case 5:
                sort_list_zawodnicy_nazwisku(M);
                print_lista_Zawodnikow(M);
                  system("pause");
             break;
         // case 6: print_lista_Zawodnikow(M);
          //        system("pause");
         //    break;
          default:
             printf("bledna opcja\n");
             system("pause");


      };
}



if(opcja_menu==1){

      switch(sub_opcja)
      {
          case 0: system("exit");
              break;
          case 1: system("cls");
              dodawanieMenuDruzyny(M);
                  system("pause");
             break;
          case 2: print_lista_Druzyny(M);
                  system("pause");
             break;
          case 3: WyszukiwanieDruzynyPoNazwie(M);
                  system("pause");
             break;
          case 4: sort_list_druzyna_nazwa(M);
                  save2file_list(M);
                  print_lista_Druzyny(M);
                  system("pause");
                   break;
         // case 5: print_lista_Druzyny(M);
         //         system("pause");

          //   break;
          default:
             printf("bledna opcja\n");
             system("pause");


      };
}








    system("cls");


   }




}


void dodawanieMenuZaowdnika(list M)
{

    char imie[40],nazwisko[40];
    int wiek,bramki,asysty,i=1;
    printf("Podaj imie zawodnika\n");
    scanf("%s",&imie);
    printf("Prosze podac naziwsko zawodnika\n");
    scanf("%s",&nazwisko);
    printf("Wiek zadownika\n");
    scanf("%d",&wiek);
    printf("Zdobyte Bramki\n");
    scanf("%d",&bramki);
    printf("Zdobyte Asysty\n");
    scanf("%d",&asysty);


    add2end(&M,create_zawodnik(i,imie,nazwisko,wiek,bramki,asysty));
    save2file_list(M);
    system("cls");
    switch_menu(M);

}


void dodawanieMenuDruzyny(list M)
{

    char nazwa[40],liga[40];
    int pozycja,wygrane,remisy,i,przegrane;
    int k=1;
    printf("Podaj nazwe druzyny\n");
    scanf("%s",&nazwa);
    printf("Podaj lige\n");
    scanf("%s",&liga);
    printf("Pozycja w tabeli\n");
    scanf("%d",&pozycja);
    printf("Wygrane\n");
    scanf("%d",&wygrane);
    printf("Remisy\n");
    scanf("%d",&remisy);
    printf("Przegrane\n");
    scanf("%d",&przegrane);


    add2end(&M,creat_druzyna(k,nazwa,liga,pozycja,wygrane,remisy,przegrane));
    save2file_list(M);
    system("cls");
    switch_menu(M);

}

void WyszukiwanieZawodnikaPoImieniu(list L)
{
    system("cls");
    char c[40];
    printf("Wpisz imie zawodnika\n");
    scanf("%s",&c);
    node * temp;
    int i=0;
    //printf("%d",L.size);

   for(temp=L.first; temp;temp=temp->next)
    {




      if(strcmp(temp->Q.Z->imie,c)==0)
        {



                printf("Imie: %s\n",temp->Q.Z->imie);
                printf("Nazwisko: %s\n",temp->Q.Z->nazwisko);
                printf("Wiek: %d\n",temp->Q.Z->wiek);
                printf("Bramki: %d\n",temp->Q.Z->bramki);
                printf("Asysty: %d\n",temp->Q.Z->asysty);

                break;
        }



    }

}

void WyszukiwanieZawodnikaPoNazwisku(list L)
{
    system("cls");
    char c[40];
    printf("Wpisz Nazwisko zawodnika\n");
    scanf("%s",&c);
    node * temp;
    int i=0;
    //printf("%d",L.size);

   for(temp=L.first; temp;temp=temp->next)
    {




      if(strcmp(temp->Q.Z->imie,c)==0)
        {



                printf("Imie: %s\n",temp->Q.Z->imie);
                printf("Nazwisko: %s\n",temp->Q.Z->nazwisko);
                printf("Wiek: %d\n",temp->Q.Z->wiek);
                printf("Bramki: %d\n",temp->Q.Z->bramki);
                printf("Asysty: %d\n",temp->Q.Z->asysty);

                break;
        }



    }

}


  void print_node_Zawodnik(node N) {
    //printf(" v:%d\n", N.value);
    switch (N.typ) {
        case Node:
            break;
        case Zawodnik:
        printZawodnik(*(N.Q.Z));


    };
}


  void print_node_Druzyna(node N) {
    //printf(" v:%d\n", N.value);
    switch (N.typ) {
        case Node:
            break;
         case Druzyna:
            printDruzyna(*(N.Q.D));


    };
}



void print_lista_Zawodnikow(list L) {
    //printf("To jest lista, ma elementow %d\n", L.size);
    system("cls");
    node *temp;

    temp = L.first;
    while (temp) {
        print_node_Zawodnik((*temp));
        temp = temp->next;
    }

}


void print_lista_Druzyny(list L) {
    //printf("To jest lista, ma elementow %d\n", L.size);
    system("cls");
    node *temp;

    temp = L.first;
    while (temp) {
        print_node_Druzyna((*temp));
        temp = temp->next;
    }

}



void WyszukiwanieDruzynyPoNazwie(list L)
{
    system("cls");
    char c[40];
    printf("Wpisz nazwe druzyny\n");
    scanf("%s",&c);
    node * temp;
    int counter=0;
    //printf("%d",L.size);

   for(temp=L.first; temp;temp=temp->next)
    {




      if(strcmp(temp->Q.D->nazwa,c)==0)
        {



                printf("Nazwa: %s\n",temp->Q.D->nazwa);
                printf("Liga: %s\n",temp->Q.D->liga);
                printf("Pozycja: %d\n",temp->Q.D->pozycjaTabela);
                printf("wygrane: %d\n",temp->Q.D->wygrane);
                printf("remisy: %d\n",temp->Q.D->remisy);
                 printf("przegrane: %d\n",temp->Q.D->przegrane);
                counter ++;
                break;
        }



    }


    if(counter==0)
    {
        printf("Brak klubu w Bazie danych ligi");
    }



}


void WyswietlanieTabeli(list *L)
{



    node *i, *j;
    for(i = L->first;i;i=i->next)

        for (j=i->next;j;j=j->next)/// j=i+1; j<size;j++
    {
        if (i->Q.D->pozycjaTabela > j->Q.D->pozycjaTabela  && i->typ==Druzyna && j->typ==Druzyna)
        {
            int temp;
            char temp1[40];
            char temp2[40];
            int temp3;
            int temp4;
            int temp5;
            temp=i->Q.D->wygrane; ///BRRAMKI --> wygrane

            strcpy(temp1,i->Q.D->nazwa); ///imie -> nazwa

            strcpy(temp2,i->Q.D->liga);///nazwisko -> liga


            temp3=i->Q.D->remisy;///wiek -> remisy
            temp4=i->Q.D->pozycjaTabela;/// asysty -> pozycja w tabelii
            temp5=i->Q.D->przegrane;


            i->Q.Z->bramki=j->Q.Z->bramki;
            strcpy(i->Q.D->nazwa,j->Q.D->nazwa);

           strcpy(i->Q.D->liga,j->Q.D->liga);

            i->Q.D->remisy=j->Q.D->remisy;
            i->Q.D->pozycjaTabela=j->Q.D->pozycjaTabela;
            i->Q.D->przegrane = j->Q.D->przegrane;




            j->Q.Z->bramki=temp;
             strcpy(j->Q.D->nazwa,temp1);

            strcpy(j->Q.D->liga,temp2);

            j->Q.D->remisy=temp3;
            j->Q.D->pozycjaTabela=temp4;
            j->Q.D->przegrane = temp5;

        }
    }


}

void sort_list_strzelcy(list *L)
{

    node *i, *j;
    for(i = L->first;i;i=i->next)

        for (j=i->next;j;j=j->next)/// j=i+1; j<size;j++
    {
        if (i->Q.Z->bramki < j->Q.Z->bramki && i->typ==Zawodnik && j->typ==Zawodnik)
        {
            int temp;
            char temp1[40];
            char temp2[40];
            int temp3;
            int temp4;
            temp=i->Q.Z->bramki;\
            strcpy(temp1,i->Q.Z->imie);
            //temp1=i->Q.Z->imie;
            strcpy(temp2,i->Q.Z->nazwisko);
            //temp2=i->Q.Z->nazwisko;

            temp3=i->Q.Z->wiek;
            temp4=i->Q.Z->asysty;



            i->Q.Z->bramki=j->Q.Z->bramki;
            strcpy(i->Q.Z->imie,j->Q.Z->imie);
           /// i->Q.Z->imie= j->Q.Z->imie;
           strcpy(i->Q.Z->nazwisko,j->Q.Z->nazwisko);
           /// i->Q.Z->nazwisko=j->Q.Z->nazwisko;
            i->Q.Z->wiek=j->Q.Z->wiek;
            i->Q.Z->asysty=j->Q.Z->asysty;

            j->Q.Z->bramki=temp;
             strcpy(j->Q.Z->imie,temp1);
           /// j->Q.Z->imie=temp1;
            strcpy(j->Q.Z->nazwisko,temp2);
            ///j->Q.Z->nazwisko=temp2;
            j->Q.Z->wiek=temp3;
            j->Q.Z->asysty=temp4;

        }
    }


}


void sort_list_zawodnicy_imie(list *L)
{

    node *i, *j;
    for(i = L->first;i;i=i->next)

        for (j=i->next;j;j=j->next)/// j=i+1; j<size;j++
    {
        if (strcmp(i->Q.Z->imie,j->Q.Z->imie) ==1 && i->typ==Zawodnik && j->typ==Zawodnik)
        {
            int temp;
            char temp1[40];
            char temp2[40];
            int temp3;
            int temp4;
            temp=i->Q.Z->bramki;\

            strcpy(temp1,i->Q.Z->imie);

            strcpy(temp2,i->Q.Z->nazwisko);


            temp3=i->Q.Z->wiek;
            temp4=i->Q.Z->asysty;



            i->Q.Z->bramki=j->Q.Z->bramki;
            strcpy(i->Q.Z->imie,j->Q.Z->imie);

           strcpy(i->Q.Z->nazwisko,j->Q.Z->nazwisko);

            i->Q.Z->wiek=j->Q.Z->wiek;
            i->Q.Z->asysty=j->Q.Z->asysty;

            j->Q.Z->bramki=temp;
             strcpy(j->Q.Z->imie,temp1);

            strcpy(j->Q.Z->nazwisko,temp2);

            j->Q.Z->wiek=temp3;
            j->Q.Z->asysty=temp4;

        }
    }


}


void sort_list_zawodnicy_nazwisku(list *L)
{

    node *i, *j;
    for(i = L->first;i;i=i->next)

        for (j=i->next;j;j=j->next)/// j=i+1; j<size;j++
    {
        if (strcmp(i->Q.Z->nazwisko,j->Q.Z->nazwisko) ==1 && i->typ==Zawodnik && j->typ==Zawodnik)
        {
            int temp;
            char temp1[40];
            char temp2[40];
            int temp3;
            int temp4;
            temp=i->Q.Z->bramki;\

            strcpy(temp1,i->Q.Z->imie);

            strcpy(temp2,i->Q.Z->nazwisko);


            temp3=i->Q.Z->wiek;
            temp4=i->Q.Z->asysty;



            i->Q.Z->bramki=j->Q.Z->bramki;
            strcpy(i->Q.Z->imie,j->Q.Z->imie);

           strcpy(i->Q.Z->nazwisko,j->Q.Z->nazwisko);

            i->Q.Z->wiek=j->Q.Z->wiek;
            i->Q.Z->asysty=j->Q.Z->asysty;

            j->Q.Z->bramki=temp;
             strcpy(j->Q.Z->imie,temp1);

            strcpy(j->Q.Z->nazwisko,temp2);

            j->Q.Z->wiek=temp3;
            j->Q.Z->asysty=temp4;

        }
    }


}


void sort_list_druzyna_nazwa(list *L)
{

    node *i, *j;
    for(i = L->first;i;i=i->next)

        for (j=i->next;j;j=j->next)/// j=i+1; j<size;j++
    {
        if (strcmp(i->Q.D->nazwa,j->Q.D->nazwa) ==1 && i->typ==Druzyna && j->typ==Druzyna)
        {
            int temp;
            char temp1[40];
            char temp2[40];
            int temp3;
            int temp4;
            int temp5;
            temp=i->Q.D->wygrane; ///BRRAMKI --> wygrane

            strcpy(temp1,i->Q.D->nazwa); ///imie -> nazwa

            strcpy(temp2,i->Q.D->liga);///nazwisko -> liga


            temp3=i->Q.D->remisy;///wiek -> remisy
            temp4=i->Q.D->pozycjaTabela;/// asysty -> pozycja w tabelii
            temp5=i->Q.D->przegrane;


            i->Q.Z->bramki=j->Q.Z->bramki;
            strcpy(i->Q.D->nazwa,j->Q.D->nazwa);

           strcpy(i->Q.D->liga,j->Q.D->liga);

            i->Q.D->remisy=j->Q.D->remisy;
            i->Q.D->pozycjaTabela=j->Q.D->pozycjaTabela;
            i->Q.D->przegrane = j->Q.D->przegrane;




            j->Q.Z->bramki=temp;
             strcpy(j->Q.D->nazwa,temp1);

            strcpy(j->Q.D->liga,temp2);

            j->Q.D->remisy=temp3;
            j->Q.D->pozycjaTabela=temp4;
            j->Q.D->przegrane = temp5;

        }
    }


}


void wyswietlenieLigi1(list L)
{

    system("cls");

    node * temp;
    int counter=0;
    //printf("%d",L.size);

   for(temp=L.first; temp;temp=temp->next)
    {




      if(strcmp(temp->Q.D->liga,"Liga_hiszpanska")==0  )
        {



                printf("Nazwa: %s\n",temp->Q.D->nazwa);
                printf("Liga: %s\n",temp->Q.D->liga);
                printf("Pozycja: %d\n",temp->Q.D->pozycjaTabela);
                printf("wygrane: %d\n",temp->Q.D->wygrane);
                printf("remisy: %d\n",temp->Q.D->remisy);
                 printf("przegrane: %d\n",temp->Q.D->przegrane);
                 printf("*******************************\n");
                counter ++;

        }



    }


    if(counter==0)
    {
        printf("Brak klubu w Bazie danych ligi");
    }




}


void wyswietlenieLigi2(list L)
{

    system("cls");

    node * temp;
    int counter=0;
    //printf("%d",L.size);

   for(temp=L.first; temp;temp=temp->next)
    {




      if(strcmp(temp->Q.D->liga,"Gdynia")==0 )
        {



                printf("Nazwa: %s\n",temp->Q.D->nazwa);
                printf("Liga: %s\n",temp->Q.D->liga);
                printf("Pozycja: %d\n",temp->Q.D->pozycjaTabela);
                printf("wygrane: %d\n",temp->Q.D->wygrane);
                printf("remisy: %d\n",temp->Q.D->remisy);
                printf("przegrane: %d\n",temp->Q.D->przegrane);
                printf("*******************************\n");
                counter ++;

        }



    }


    if(counter==0)
    {
        printf("Brak klubu w Bazie danych ligi");
    }




}


/// REZERWACJA ///


void Odczyt_z_pliku_Rezerwacja()
{
    FILE * F;
    if ((F = fopen("rezerwacje.txt","r")) == NULL )
   {
       printf("problem z plikiem");
       exit(112);
   }
  int ile;
   char text[256];
   fscanf(F,"%s %d", text,&ile);
   if (strcmp(text,"ile:")!=0)
   {
       printf("problem z plikiem 'ile':: %s",text );
       exit(113);
   }

   int os;
   Baza_danych.id=0;
   for (os=0;os<ile;os++)
   {
       rezerwacja o;
       char temp[256];

       fscanf(F,"%s %s", temp,&o.imie);
       if (strcmp(temp,"imie:")!=0)
           {
               printf("problem z plikiem 'imie':: %s",temp );
               exit(114);
           }
      fscanf(F,"%s %s", temp,&o.nazwisko);
       if (strcmp(temp,"nazwisko:")!=0)
           {
               printf("problem z plikiem 'typ':: %s",temp );
               exit(115);
           }

        fscanf(F,"%s %d", temp,&o.iloscN);
       if (strcmp(temp,"iloscN:")!=0)
           {
               printf("problem z plikiem 'iloscN':: %s",temp );
               exit(116);
           }
         fscanf(F,"%s %d", temp,&o.iloscU);
       if (strcmp(temp,"iloscU:")!=0)
           {
               printf("problem z plikiem 'iloscU':: %s",temp );
               exit(116);
           }

        fscanf(F,"%s %s", temp,&o.mecz);

       if (strcmp(temp,"mecz:")!=0)
           {
               printf("problem z plikiem 'nazwa':: %s",temp );
               exit(114);
           }

    dodaj_do_bazy(o);
   }



 fclose(F);

   }


int dodaj_do_bazy(rezerwacja o)
{
    if (Baza_danych.id < MAX)
    {
        Baza_danych.baza[Baza_danych.id] =o;
        Baza_danych.id++;

        return 1;
    }
    else
    {
        printf("baza pelna");
        return 0;
    }
}


void wypiszRezerwacje()
{
    printf("Wszystkich rezerwacji jest: %d . Sa to:\n",Baza_danych.id);

printf("\n");
    int i;
      printf("*Imie-Nazwisko-IloscN-IloscU-Mecze*****\n");
      printf("\n");
    for (i=0;i<Baza_danych.id;i++)
    {
        printf("%d. %s  %s   %d   %d  %s\n",i+1,Baza_danych.baza[i].imie,Baza_danych.baza[i].nazwisko,Baza_danych.baza[i].iloscN,Baza_danych.baza[i].iloscU,Baza_danych.baza[i].mecz);
        printf("\n");
        printf("*******************************************\n");

    }
    printf("\n");
}

void Zapisz_do_pliku_rezerwacji()
{
    FILE * F;
    if ((F = fopen("rezerwacje.txt","w")) == NULL )
   {
       printf("problem z plikiem");
       exit(112);
   }

   int i;
   fprintf(F,"ile: %d\n",Baza_danych.id);
   for(i=0;i<Baza_danych.id;i++)
   {
   fprintf(F,"\n");
   fprintf(F,"imie: %s\n",Baza_danych.baza[i].imie);
   fprintf(F,"nazwisko: %s\n",Baza_danych.baza[i].nazwisko);
   fprintf(F,"iloscN: %d\n",Baza_danych.baza[i].iloscN);
   fprintf(F,"iloscU: %d\n",Baza_danych.baza[i].iloscU);
   fprintf(F,"mecz: %s\n",Baza_danych.baza[i].mecz);
   }
   fclose(F);

}

void dodawanieRezerwacji(list L)
{
 node * temp1;

 int i;

   rezerwacja temp;

   printf("Podaj imie:\n");

   scanf("%s",&temp.imie);

   printf("Podaj nazwisko:\n");

   scanf("%s",&temp.nazwisko);

   printf("Podaj iloscN:\n");

   scanf("%d",&temp.iloscN);

   printf("Podaj iloscU:\n");

   scanf("%d",&temp.iloscU);

   printf("Podaj mecz:\n");

   scanf("%s",&temp.mecz);








    int counter =0;

   for(temp1=L.first; temp1;temp1=temp1->next)
    {

      if(strcmp(temp1->Q.D->nazwa,temp.mecz)==0)
        {

                printf("dodano pomyslnie\n");
                counter ++;
                break;
        }

     }


    if(counter==0)
    {
        printf("Brak klubu w Bazie danych ligi\n");
    }


Zapisz_do_pliku_rezerwacji();
}


void usunRezerwacje()
{

     int i, j;
    char text[100];

        printf("Podaj nazwisko: ");
        scanf("%s", text);
        for (i = 0; i < Baza_danych.id; i++)
        {
            if (strcmp(Baza_danych.baza[i].nazwisko, text) == 0)
            {
                for (j = i; j < Baza_danych.id; j++)
                {
                    Baza_danych.baza[j] = Baza_danych.baza[j+1];
                }
                Baza_danych.id--;



            }

        }

     Zapisz_do_pliku_rezerwacji();
}

void panelAdministracyjny(list M)
{
    system("cls");
    int i;
    char login[10] = "admin";
    char loginWeryfikacyjny[10];
    char haslo[10] ="admin";
    char hasloWeryfikacyjne[10];
    char p[10];
    printf("Prosze podac login i haslo\n");

    printf("LOGIN:");
    scanf("%s",&loginWeryfikacyjny);
    printf("HASLO:");
    for (i = 0; i < strlen(haslo); i++) {

        p[i] = getch();
        printf("*");
    }

    if(strcmp(login,loginWeryfikacyjny)==0 && strcmp(haslo,p)==0 )
    {
        system("cls");
       print_lista(M);

    }
    else{ system("cls"); panelAdministracyjny(M);}



}

int main() {

    Odczyt_z_pliku_Rezerwacja();
    list M;
    list_init(&M);

    openfile_list(&M);

    save2file_list(M);

    switch_menu(M);

    free_list(&M);


    return 0;
}
