COMP.CS.300 Tietorakenteet ja algoritmit 1 

Game of Taxes


Tässä tiedostossa kerrotaan yleisesti ohjelman tehokkuusratkaisuista. 
Joistakin funktioista, jotka eroavat muista on kerrottu tarkemmin erikseen.


-----Päätietorakenne:

Projektin tietorakenne on unordered_map, jonka avaimina on TownID ja arvoina 
Town* osoittimia.

Unordered_map valikoitui siksi, että se tarjoaa kohtuullisen nopean haun 
string muotoisella avaimella, jota funktiot käyttävät parametreinaan.
Unordered_mappia on myös yksinkertaista iteroida, mikä on erittäin tärkeää 
koko ohjelman toiminnan kannalta. Unordered_mappia ei myöskään ole ohjelman
kannalta tarvetta pitää järjestyksessä.

Yksittäinen kaupunki on luotu Town structina, jossa on sisällä tiedot id, 
nimi, koordinaatit, vero, etäisyys origosta, vasallien osoittimet vektorissa 
sekä osoitin master-kaupunkiin. 

Kaupunki Town toteutettiin structina, koska structista on nopeaa hakea 
osoittimella haluamansa arvot. Nopeuden vuoksi myös master ja vasallit ovat
osoittimina, jolloin päätietorakenteesta ei tarvitse turhaan etsiä kaupunkien 
välisiä suhteita. Päätietorakenteesta riittää hakea vain kerran tieto 
käsiteltävästä kaupungista, jonka jälkeen voidaan nopeasti siirtyä vasalli- 
ja mastersuhteiden mukaan kaupungista toiseen osoittimilla.

Vector valikoitui vasallien tallettamiseen sen tehokkuuden ja 
helppokäyttöisyyden vuoksi.

Etäisyys origosta lasketaan lisätään tietorakenteeseen, sillä se auttaa suoraan kolmen
funktion toiminnassa. Towns_distance_increasing, min_distance ja max_distance
käyttävät suoraan tietoa kaupunkien etäisyydestä origoon. Siksi tämä arvo
talletetaan jo kaupunkia talletettaessa tietorakenteeseen.


-----Yleisesti funktioiden ratkaisusta:

Osoittimia pitkin liikkuminen on huomattavasti nopeampaa, kuin 
tietorakenteesta kaupungin haku, joten kaikissa funktioissa on pyritty 
välttämään turhaa päätietorakenteesta hakemista. 

Funktioiden alussa tarvitsee usein selvittää löytyykö id:tä tietorakenteesta. 
.find() -funktio palauttaa osoittimen kohtaan, josta hakuavaimella etsitty 
tietorakenteen jäsen löytyy. Osoitin talletetaan muuttujaan ja sitä käytetään
koko funktion suorituksen ajan, jolloin erillistä uutta hakua 
päätietorakenteesta ei tarvitse tehdä.

Vectoreita käytettäessä on pyritty aina kun mahdollista allokoimaan 
tarvittava määrä muistia etukäteen reserve() metodilla, jotta push_back() 
metodia käytettäessä dataa ei tarvitsisi kopioida paikasta toiseen 
uudelleenallokoinnin yhteydessä.

Etäisyyden laskemisessa pow()-funktion käytön sijasta tehdään suora 
kertolasku, joka on tehokkaampi kuin kyseisen funktion käyttö. Koska 
etäisyyksiä lasketaan ohjelmassa paljon, on kyseinen hyöty huomattava.

STL:n algoritmikirjaston algoritmeja on pyritty käyttämään usein ja 
monipuolisesti.


-----Get funktioista:

Kaikista useiten kutsuttavat funktiot on toteutettu mahdollisimman 
yksinkertaisesti ja tehokkaasti kompromisseja tekemättä. Päätietorakenne 
suunniteltiin näitä funktioita ajatellen.


-----towns_alphabetically:

Unordered_mappia ei pysty sorttaamaan, joten on luotava uusi tietorakenne. 
Testeissä nopeimmaksi tietorakenteeksi valikoitui vectori, johon talletetaan 
Town* pointtereita. Vectorille varataan valmiiksi tilaa muistista.

Town* pointterit sortataan sort()-algoritmilla, jossa käytetään 
lambdafunktiota vertailemaan kaupunkien nimien järjestystä. Kaupunkien nimiin 
viitataan nopeasti osoittimilla. 

Lopuksi luodaan paluuvectori, johon sortatuista Town* pointtereista voidaan 
lisätä TownID:t. Paluuvectorillekin varataan valmiiksi tilaa muistista, jotta 
vältytään turhalta muistin uudelleenallokoinnilta.


-----towns_distance_increasing:

Etäisyyden mukaan sortatessa nopeimmaksi lisätietorakenteeksi osoittautui 
vector, johon talletetaan Town* -pointtereita. Vectorille varataan valmiiksi
muistia.

Vectori järjestetään lambdafunktiota hyödyntävällä sort() algoritmilla. 
Päätietorakenteen vuoksi kaupunkien etäisyy origosta on jo laskettu, joten
vertailu on nopeaa.

Lisäksi luodaan paluuvectori, jolle varataan tilaa valmiiksi muistista. 
Järjestetty vector iteroidaan läpi uudelleen tallettaen TownID:t 
paluuvektoriin.


-----min_distance ja max_distance:

Minimi- ja maksimietäisyyksien etsimiseen käytetään STL:n min_element ja 
max_element algoritmeja lambdafunktiota hyödyntäen. Algoritmit voidaan 
toteuttaa suoraan päätietorakenteeseen, jolloin minimin ja maximin 
määrittäminen on tehokasta. Funktiot hyötyvät päätietorakenteeseen 
talletetuista kaupunkien etäisyyksistä origoon.


-----taxer_path:

Päätietorakenteeseen talletettujen osoittimien avulla on mahdollista 
toteuttaa for silmukka, joka iteroi nopeasti läpi kaupungin masterien ja 
masterien masterien tallettaen välissä jokaisen kaupungin TownID:n vectoriin.


-----towns_nearest:

Funktion toteutus tehokkaasti vaatii uuden structin määrittämisen. Structiin
tallennetaan kaupungin id ja etäisyys pisteestä. Structit talletetaan 
vectoriin, jolle varataan valmiiksi muistia. Vectori järjestetään 
sort()-algoritmilla, johon hyödynnetään lambdafunktiota. Järjestettyjen 
structien sisältä talletetaan TownID:t paluuvektoriin, jolle on varattu tilaa
muistista. Lopuksi structit tuhotaan.


-----longest_vassal_path, total_net_tax:

Molemmat funktiot on toteutettu rekursiivisesti päätietorakenteen 
puurakenteen vuoksi. Rekursio on toteutettu niin, että jokainen alkio puusta
käydään läpi kerran, jolloin asymptoottinen tehokkuus on saatu lineaariseksi.

Molemmat funktiot kutsuvat omia rekursiivisia apufunktioitaan. 





