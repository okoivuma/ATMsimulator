/*
***
*** Oskari Koivumaki
*** Ohjelmointi 1
***
*/

#include <stdio.h>
#include <string.h>

int lueKokonaisluku(void);
void lueMerkkijono(char merkkijono[], int pituus);
void lueRoskat(void);
void palaaValikkoon(void);
void saldoToiminto(double saldo);
void lopetaohjelma(void);
double nostoToiminto(double saldo);
double talletusToiminto(double saldo);
double kirjauduSisaan(void);
void rahanjako(int nosto);

int main(void){

    int valinta = 0;
    double saldo = 0;

    printf("\n\nTervetuloa!\n");

    saldo = kirjauduSisaan();
  
    do{
        printf("\n\n**********************************\n");
        printf("\n Valikko\n");
        printf("\n\nValitse toiminto\n Saldo > 1\n Nosto > 2\n Talletus > 3\n Lopeta > 4\n\n >");
        valinta = lueKokonaisluku();

        switch (valinta){
        case 1:
            saldoToiminto(saldo);
            break;
        case 2:
            saldo = nostoToiminto(saldo);
            break;
        case 3:
            saldo = talletusToiminto(saldo);
            break;
        case 4:
            lopetaohjelma();
            break;
        default:
            printf ("\nTee valinta syottamalla 1,2,3 tai 4\n");
            break;
    }
    } while (valinta != 4);
    
    return (0);
}

/*
 * kirjauduSisaan - lukee käyttäjän syottämän tilinumeron ja avaa sen perusteella tilitiedoston,
 * mikäli vastaava löytyy. Jos tilinumeroa vastaavaa tiedostoa ei löydy, niin ohjelma kehoittaa
 * syöttämään tilinumeron uudestaan. 
 * 
 * Funktio lukee tilitiedostosta oikean pin koodin ja kysyy pin koodia käyttäjältä. Jos pin koodit 
 * täsmäävät funktio lukee tiedostosta tilin saldon ja tallentaa sen saldomuuttujaan. Muuten funktio kysyy
 * pin koodia käyttäjältä, kunnes se on oikein.
 * 
 * Saldon tallennuksen jälkeen funktio sulkee tiedoston ja palauttaa main funktiolle saldon arvon.
 * 
 * - ei parametrejä, palauttaa arvon saldo (double) käytettäväksi muualla ohjelmassa.
 */
double kirjauduSisaan(void){
    char tilinro[50];
    int pin = 0;
    int oikeaPin;
    double saldo;
    FILE *tili;
    do{
        printf("\nSyota tilinumero >");
        lueMerkkijono(tilinro, 50);
        strcat(tilinro, ".acc");
        if ((tili = fopen(tilinro, "r")) == NULL){
            printf("\nTilia ei voitu avata\n");
        } 
    } while (tili == NULL);

    fscanf(tili, "%d", &oikeaPin);

    printf("\nSyota pin koodi >");
    do{
        pin = lueKokonaisluku();

        if (pin != oikeaPin){
            printf("\nVaara pin koodi, yrita uudestaan >");
        }
        
    } while (pin != oikeaPin);
    
    fscanf(tili, "%lf", &saldo);
    fclose(tili);

    printf("\nKirjauduit sisaan");
    return saldo;
}

/* 
 * palaaValikkoon - funktio mahdollistaa toiminnallisuuden hallitun sulkemisen, jolloin
 * käyttäjä voi itse päättää, milloin siirtyy takaisin valikkoon tapahutuman ja sen 
 * tarkastelun jälkeen.
 * 
 * - Funktio ei ota vastaan, eikä palauta arvoja. 
 */
void palaaValikkoon(void){
    int poistu = 0;
    do
    {
        printf("\nValitse 1 palataksesi valikkoon >");
        poistu = lueKokonaisluku();
    } while (poistu != 1);

    printf ("\n\npalataan valikkoon");
    printf ("\n\n");
}

/*
 * saldotoiminto - Tulostaa ruudulle käyttäjän senhetkisen saldon.
 * Funktio kutsuu palaaValikkoon sulkeakseen toiminnon käyttäjän ehdoilla.
 *
 * Parametrit:
 * - saldomuuttuja (double): reaaliluku, joka tulostetaan ruudulle.
 * 
 * Ohjelma ei palauta arvoja.
 */
void saldoToiminto(double saldo){
    printf("\n\n**********************************\n");
    printf("\nSaldo\n");
    printf("\nSaldosi on %.2lf Euroa", saldo);
    palaaValikkoon();
}

/*
 * nostotoiminto - Kysyy käyttäjältä nostettavan summan, jos tilillä on
 * saldoa riittävästi (20e tai enemmän). Mikäli saldoa ei ole tarpeeksi,
 * tulostetaan ruudulle siitä käyttäjälle ilmoitus ja ohjelma palaa main funktioon.
 * 
 * Rahan nostamiseen on rajoitteita, sillä käytössä on vain 20e ja 50e setelit.
 * Nostettava summa voi olla 20e, 50e tai enemmän kymmenen euron välein 1000 euroon asti.
 * 
 * Jos käyttäjä yrittää nostaa näistä eriävän summan, tai nosto ylittäisi tilin saldon
 * tulostuu virheilmoitus ja funktio kysyy nostettavaa summaa, kunnes se on kelvollinen.
 * 
 * Onnistuneen nostotapahtuman jälkeen tulostetaan ilmoitus nostosta ja kutsutaan funktiota rahanjako()
 * seteleihin jaon tekemiseen.
 * 
 * Saldo päivitetään noston jälkeen ajan tasalle.
 * 
 * Funktiossa kutsutaan palaaValikkoon() hallitun main funktioon paluun mahdollistamiseksi.
 * 
 * Parametrit:
 * - saldo (double), jonka arvo on saatu ohjelman käyttöön tilitiedostosta ja päivitetään ohjelman aikana.
 * 
 * Paluuarvot;
 * - saldo (double), saldomuuttuja, johon päivitetty nostotapahtuman vaikutus
 */
double nostoToiminto(double saldo){

    int nosto = 0;   
    printf("\n\n**********************************\n");
    printf("\nNosto\n");

    if (saldo < 20){
        printf("\nSaldosi ei riita miniminostoon.\n");
        return saldo;
    }
    
    do{
        printf("\nPaljonko haluaisit nostaa? (syota kokonaisluku) >");
      
        nosto = lueKokonaisluku();

        if (nosto < 20){
            printf("\nPienin nostettava maara on 20 Euroa.\n");
        }
        else if (nosto > saldo){
            printf("\nNosto ei voi ylittaa saldoasi, saldosi on %.2lf Euroa\n", saldo);
        }     
        else if (nosto == 30 || (nosto % 10) != 0){
            printf("\nNostettavan summan taytyy olla 20e, 40e tai enemman kymmenen euron valein.\n");
        }
        else if (nosto > 1000){
            printf("\nSuurin sallittu kertanosto on 1000 Euroa.\n");
        }
    } while (nosto < 20 || nosto > saldo || nosto > 1000 || nosto == 30 || ((nosto % 10) != 0));
      
    printf("\n%d Euroa nostettu, ole hyva", nosto);
    rahanjako(nosto);
    saldo -= nosto;
    palaaValikkoon();
    return saldo;
}

/*
 * talletustoiminto - kysyy käyttäjältä talletettavan summan ja tarkistaa sen oikeellisuuden.
 * Syöte on kelvollinen, kun se on positiivinen kokonaisluku. Mikäli syöte on väärä, tulostuu virheilmoitus
 * ja summa kysytään uudelleen käyttäjältä.
 * 
 * Onnistunen talletuksen jälkeen tulostuu ilmoitus siitä ja
 * lisätään talletettu summa saldoon, jolloin saldo pysyy ajantasalla.
 * 
 * Parametrit:
 * - saldo (double), jonka arvo on saatu ohjelman käyttöön tilitiedostosta ja päivitetään ohjelman aikana.
 * 
 * Paluuarvot:
 * - saldo (double), johon päivitetty talletuksen vaikutus.
 * 
 * Funktiossa kutsutaan palaaValikkoon() hallitun main funktioon paluun mahdollistamiseksi.
 */
double talletusToiminto(double saldo){

    int talletus = 0;
    printf("\n\n**********************************\n");
    printf("\nTalletus\n");
    printf("\nPaljonko haluaisit tallettaa? (syota kokonaisluku) >");
    talletus = lueKokonaisluku();
    
    while (talletus < 0){
        printf("\nNegatiivista lukua ei voi tallettaa.");
        printf("\nSyota positiivinen kokonaisluku >");
        talletus = lueKokonaisluku();
    }
    printf("\n%d Euroa talletettu", talletus);
    saldo += talletus;
    palaaValikkoon();
    return saldo;
}

/*
 * luekokonaisluku - lukee käyttäjän syöttämän kokonaisluvun ja tarkistaa syötteen oikeellisuuden.
 * käytetään kun halutaan lukea muuttujaan kokonaisluku näppäimistöltä turvallisesti.
 * Jos syöte on väärää tietotyyppiä, tulostetaan virheilmoitus ja tyhjennetään puskuri lueroskat() funktiolla, jonka
 * jälkeen kysytään uudestaan lukua käyttäjältä.
 * 
 * - Ei parametrejä
 * 
 * Paluuarvot:
 * - luku (int) palauttaa käyttäjän syöttämän luvun, joka tallennetaan haluttuun muuttujaan.
 */
int lueKokonaisluku(void){

   int luku = 0;
   char mki =' ';
   int status = 0;

   while (((status = scanf("%d%c", &luku, &mki)) == 0)  || (2 == status && mki != '\n')){
      lueRoskat();
      printf("Et syottanyt kokonaislukua > ");
   }

   return luku;
}

/*
 * lueRoskat - tyhjentää näppäinpuskurin.
 * kutsutaan yleensä virheellisen syötteen jälkeen.
 * 
 * Ei ota vastaan eikä palauta arvoja.
 */
void lueRoskat(void){

   // Tyhjennetaan puskuri
   while( fgetc(stdin) != '\n');
}


/*
 * rahanjako - jakaa nostosumman, joka saadaan nostoToiminto funktiosta 50e ja 20e seteleihin.
 *
 * Tulostaa seteleiden määrät näytölle.
 *
 * Parametrit:
 * - nosto (int), nostettava summa, joka kysytään käyttäjältä nostoToiminto funktiossa.
 * 
 * Ei paluuarvoja
 */
void rahanjako(int nosto){

    int viiskympit = 0;
    int kakskympit = 0;

    while (nosto >= 50){
        viiskympit++;
        nosto -= 50;
    }

    if ((nosto % 20) == 0){
        kakskympit = nosto / 20;
    }
    else{
        while (viiskympit > 0){
            nosto += 50;
            viiskympit--;

            while (nosto >= 20){
                kakskympit++;
                nosto -= 20;
            }

            if (nosto == 0){
                break;
            }   
        } 
    }

    printf("\n%d kpl 50e seteleita, %d kpl 20e seteleita", viiskympit, kakskympit);
}

/*
 * lopetaohjelma - tulostaa ruudulle ilmoituksen ohjelman loppumisesta.
 *
 * Ei ota vastaan, eikä palauta arvoja.
 */
void lopetaohjelma(void){
    printf("\n\n**********************************\n");
    printf("\n\nKiitos kaynnista!\nOlet kirjautunut ulos\n\n");
}

/*
 * lueMerkkijono - lukee näppäimistöltä merkkijonon ja tarkistaa syötteen.
 * Mikäli syöte on jotakin muuta tietotyyppiä, kuin merkkijono, tyhjennetään
 * puskuri lueRoskat() avulla ja kysytään merkkijonoa uudestaan.
 * 
 * Parametrit:
 * - merkkjono[] (char), merkkijono, johon halutaan lukea tieto näppäimistöltä
 * - pituus (int), merkkijonon maksimipituus
 * 
 * Ei paluuarvoja
 */
void lueMerkkijono(char merkkijono[], int pituus){

   fgets( merkkijono, pituus, stdin );

   // Jos merkkijonon lopussa, ennen lopetusmerkkia on ylimaarainen
   // rivinvaihto korvataan se lopetusmerkillä
   if( merkkijono[ strlen(merkkijono)-1 ] == '\n')
      merkkijono[ strlen(merkkijono)-1 ] = '\0';
   else
      lueRoskat();

}