
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 1000            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

// Funkcja stworzona przez Dr Inż. Robert Muszyński
// Oryginalny kod można znaleźć: https://kcir.pwr.edu.pl/~mucha/PProg/PProg_lab_03/index.html (odczyt.c)
/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] szarosci liczba odcieni szarosci					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawid�owy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien by� P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    } 
  }
  printf("Prawidlowo wczytano obraz\n"); /*wyswietlenie informacjio prawidlowym wczytaniu*/
  return *wymx**wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

/*Definicja funckji zapisujacej powstaly obraz do pliku */
int zapis(FILE *plik_wy,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) 
{
  int i,j; 
/*Sprawdzenie czy podano prawidlowy uchwyt pliku */
  if (plik_wy==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }
fprintf(plik_wy,"P2\n"); 
fprintf(plik_wy,"%d %d %d\n",*wymx,*wymy,*szarosci); /*zapisanie wymiarow obrazu i liczby odcieniow szarosci do wksazanego pliku */
/* zapisanie obrazu z tablicy obraz_pgm */
for (i=0;i<*wymy;i++) {   
    for (j=0;j<*wymx;j++) {
      
	fprintf(plik_wy,"%d\n",obraz_pgm[i][j]);
    }
}
	return(0);
}

/*Definicja funckji negatyw*/
// Funkcja ta odwraca wartości pikseli obrazu, tworząc efekt negatywu.
int negatyw(int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci) 
{
  int i,j; 
  /*przetworzenie obrazu, kolejnym pikselom przypisujemy wartosc rowna roznicy liczby odcieni szarosci i poczatkowej wartosci dla danego piksela*/
    for (i=0;i<*wymy;i++) {
        for (j=0;j<*wymx;j++) {
            obraz_pgm[i][j]=*szarosci-obraz_pgm[i][j];
        }
    } 
    printf("Obraz przetworzono funkcja negatyw, jesli chcesz zobaczy efekt zapis obraz,a nastepnie go wyswietl\n");
    return 0;
}

/*definicja funkcji obliczjacej wartosc bezwzdgledna z liczy */
int wartosc(int x)
{ 
  if (x<0) 
  x=x*(-1);
  return x;
}

/*Definicja funckji kontur*/
/*Funckja zwraca obraz będący konturem podanego obrazu */
int kontur(int obraz_pgm[][MAX],int *wymx,int *wymy,int *szarosci) /*Definicja funckji kontur*/
{
  int i,j; 
  /*przetwarzanie obrazu, piksele które różnią się od otoczenia zostaną rozjaśnione,a piksele, które są podobne  do  otoczenia,  przyciemnione */
    for (i=0;i<*wymy;i++) {
        for (j=0;j<*wymx;j++) {
            if((j<*wymx-1)&&(i<*wymy-1))/*sprawdzenie czy piksel nie jest z ostatniego wiersza badz ostatniej kolumny*/
                obraz_pgm[i][j]=(wartosc(obraz_pgm[i+1][j]-obraz_pgm[i][j])+wartosc(obraz_pgm[i][j+1]-obraz_pgm[i][j]))/2; /*wyliczenie wartosci poszczegolnych pikseli (wyliczona wartosc dzielona jest przez 2 aby nie przekroczyc liczby szarosci 
*/
            else obraz_pgm[i][j]=0; /*pozostalym wartosciom przepisanie wartosci 0*/

        }
    }
    printf("Obraz przetworzono funkcja kontur, jesli chcesz zobaczy efekt zapis obraz,a nastepnie go wyswietl\n"); /*wyswietlenie informacji o przeprowadzeniu operacji */
  return 0;
}
/*Definicja funckji progowanie */
/*Operacja progowania polega na klasyfikacji piksela do jednej z dwóch klas – zbioru punktów czarnych lub zbioru punktów białych.*/
int progowanie(int obraz_pgm[][MAX], int *wymx,int *wymy, int *szarosci) 
{
  
    int prog,i,j,p; 
    printf("Podaj wartosc progu:");
    scanf("%d",&p);
    if((p>0)&&(p<100)){ /*sprawdzenie czy wczytana wartosc znajduje sie  wprzedziale (0,100) */
        prog=(*szarosci*p)/100; /*obliczenie wartosci progu */
        for (i=0;i<*wymy;i++) {
            for (j=0;j<*wymx;j++) {
                if(obraz_pgm[i][j]<=prog) /*sprawdzenie czy obecna wartosc piksela jest mniesza lub rowna od progu */
                    obraz_pgm[i][j]=0; /*jesli warunek jest spelniony przypisanie pikselowi wartosci 0*/
                else 
                    obraz_pgm[i][j]=*szarosci; /*w przeciwnym przypadku przypisanie pikselowi wartosci liczby szarosci */
            }
        } 
        printf("Obraz przetworzono funkcja progowanie, jesli chcesz zobaczy efekt zapis obraz,a nastepnie go wyswietl\n");/*wyswietlenie informacji o przeprowadzeniu operacji */
    } 
    else printf("Podano nie prawidlowa wartosc progu\n"); /*w przypadku gdypodana wartosc jet niepoprawna, wyswietlenia komunikatu */
    return 0;
}
/*Definicja funckji lustrzane odbicie */

int lustro(int obraz_pgm[][MAX], int *wymx,int *wymy, int *szarosci) /*Definicja funckji lustrzane odbicie */
{
    int i,j, obr[*wymy][*wymx];
    /*przetwarzanie obrazu,do pomocniczej tablicy przypisywane sa wartosci pikseli wedlug zasady: pikselom z pierwszego wiersza sa przypisywane pierwotne wartosci pikseli z ostatniego wiersza itd. */
    for (i=0;i<*wymy;i++) {
        for (j=0;j<*wymx;j++) {      
            obr[i][j]=obraz_pgm[*wymy-i][j];   
    }
    }/*przypisanie pikselom obrazu kolejnych wartosci z pomocniczej tablicy */
    for (i=0;i<*wymy;i++) {
        for (j=0;j<*wymx;j++) {
            obraz_pgm[i][j]=obr[i][j];
        }
    } 
    printf("Obraz przetworzono funkcja lustzrane odbicie, jesli chcesz zobaczy efekt zapis obraz,a nastepnie go wyswietl\n");/*wyswietlenie informacji o przeprowadzeniu operacji */
    return 0;
}

// Funkcja stworzona przez Dr Inz. Robert Muszyński
// Oryginalny kod można znalezs: https://kcir.pwr.edu.pl/~mucha/PProg/PProg_lab_03/index.html (odczyt.c)
/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}


int main() {
  int obraz[MAX][MAX] ; 
  int wymx,wymy,odcieni;
    char wybor[2]=" ";
  int odczytano = 0;
  FILE *plik;
  char nazwa[100];
  /*petla w ktorej znajduje sie menu */
    do 
    { 
        printf("Opcje menu:\n 1.Wczytaj \n 2.zapis \n 3.Negatyw \n 4.Kontur\n 5.Progowanie\n 6.Lustrzane odbicie\n 7.Wyswietl(plik musi byc wczesniej zapisany)\n 8.Koniec\n"); /*wypisanie menu*/
        printf("twoj wybor: ");
        scanf("%1s",wybor);
        
        switch(wybor[0])
        {
            case '1':
                {
                /* Wczytanie zawartosci wskazanego pliku do pamieci */
                printf("Podaj nazwe pliku:\n");
                scanf("%s",nazwa);
                plik=fopen(nazwa,"r");

                if (plik != NULL) {      
                    odczytano = czytaj(plik,obraz,&wymx,&wymy,&odcieni);
                    fclose(plik);
                }
                else 
                    fprintf(stderr,"Blad: Nie podano prawidlowago uchwytu do pliku\n"); /* w wypadku niepowodzenia wyswietlenie komunikatu o bledzie */

                /* Wyswietlenie poprawnie wczytanego obraza zewnetrznym programem */
                if(odczytano!=0)
                    wyswietl(nazwa);
                    break;
            }
            case '2': {
                if(plik==NULL){ /*sprawdzenie czy plik zostal wczensiej wczytany */
                    printf("Blad: należy najpierw wczytac plik\n");} /* informacja o nie wczytanym pliku */
                else {
                    /*zapisanie pliku pod podana nazwa */
                    printf("Podaj nazwe pliku do zapisu:\n");
                    scanf("%s",nazwa);
                    plik=fopen(nazwa,"w");
                    odczytano = zapis(plik,obraz,&wymx,&wymy,&odcieni);
                    fclose(plik); 
                }
            }
            break;
            case '3': negatyw(obraz,&wymx,&wymy,&odcieni); /*wywolanie funckji negatyw */
            break;
            case '4': kontur(obraz,&wymx,&wymy,&odcieni);/*wywolanie funckji kontur */
            break;
            case '5': progowanie(obraz,&wymx,&wymy,&odcieni);/*wywolanie funckji progowanie */
            break;
            case '6': lustro(obraz,&wymx,&wymy,&odcieni);/*wywolanie funckji lustro */
            break;
            case '7':wyswietl(nazwa);/*wywolanie funckji wyswietl */
            break;
            case '8': printf("Koniec dzialania programu\n"); /*wyswietlenie informacji o oncu programu */
            break;
            default: 
            printf("Nie ma takiej opcji w menu\n"); /*wyswietlenie informacji o blednie wprowadzonym wyborze */ 
            break;
        }

}
while (wybor[0]!='8'); /*warunek konczacy petle */
}




