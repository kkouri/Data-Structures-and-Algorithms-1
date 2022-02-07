COMP.CS.300 Tietorakenteet ja algoritmit 1 

Game of Taxes


Tässä tiedostossa kerrotaan yleisesti ohjelman toisen vaiheen tehokkuusratkaisuista. 
Joistakin tärkeimmistä funktioista kerrotaan erikseen.

-----Päätietorakenne:

Päätietorakenteeseen on luotu uusi Road struct, joka kuvaa tietä. Structiin kuuluu kaksi Town* 
osoitinta, sekä kaupunkien välille laskettu etäisyys. Etäisyys lasketaan tietä lisätessä ja 
talletetaan structiin, jotta sitä ei tarvitse laskea uudestaan. Kaikki tiet talletetaan set 
muotoiseen tietorakenteeseen, joka järjestyy tien pituuden mukaan niin, että lyhin on ensin.
Järjestystä tarvitaan trim_road_network funktiossa, joten se on fiksua tehdä jo teitä lisätessä.
Lisäksi kaikki tiet sisältävästä setistä on nopeaa hakea tietä ja sitä on helppo iteroida.

Jokaisella Town structilla on nyt myös vectori, johon on tallennettu Road structien osoittimet.
Vectori valikoitui tietorakenteeksi sen nopean iteroimisen takia. Osoittimia on käytetty nopeata
tiedonhakemista varten.

Towniin on myös talletettu enum tyyppinen väri-indikaattori, jota tarvitaan algoritmeissa. Lisäksi 
structista löytyy muita tietotyyppejä, joista lisää myöhemmin.


-----Yleisesti ratkaisuista:

Ohjelmassa on tehokkuussyistä pyritty valitsemaan algoritmeja fiksusti. Tämän lisäksi ohjelmassa 
käytetään mahdollisimman laajasti osoittimia ja vältetään turhaa läpikäyntiä for loopissa. Useissa
funktioissa Town* osoitin talletetaan suoraan towns tietorakenteesta haun yhteydessä, jott sitä ei
tarvitse myöhemmin hakea uudelleen.Peräkkäisiä silmukoita on vähennetty muun muassa tekemällä 
poistoja ja lisäyksiä samassa silmukassa erillisten insert tai clear komentojen sijaan.


-----clear_all:

Käy läpi roads setin poistaen kaikki tiet. Käy läpi kaupungit tyhjentäen Town structeihin talletetut 
osoittimet. .clear() metodi on tehokas ja nopea.


-----all_roads:

Käy läpi for loopilla kaikki tiet ja puskee id:t vectoriin. Yhteinen tietorakenne kaikille teille
tekee funktiosta tehokkaan.


-----add_road:

Tien olemassaolo tarkastetaan yhdestä kaupungista, eikä kaikki tiet sisältävästä roads setistä, 
sillä lyhyemmän tietorakenteen iterointi on nopeampaa. Tiet on talletettu molempiin kaupunkeihin,
joten vain toinen tarvitsee tarkastaa.

Tien pituus lasketaan valmiiksi ja talletetaan päätietorakenteeseen, mikä parantaa muiden 
funktioiden tehokkuutta.


-----get_roads_from:

Kaupungista lähtevät tiet on talletettu Town* structiin vectoriin, joka on on nopea iteroida läpi
lisäten TownID:t  


-----any_route:

least_towns_route tarjoaa tehokkaimman ratkaisun tien löytämiseen kahden kaupungin välillä, joten
kutsutaan sitä funktiossa.


-----remove_road:

Tien olemassaolo tarvitsee tarkastaa vain yhdestä kaupungista find_if funktiota käyttäen. Mikäli tie
on olemassa, voidaan etsiä loputkin osoittimet ja poistaa tie ohjelmasta.


-----least_towns_route:

Funktio on toteutettu käyttäen BFS algoritmia, joka löytää suoraan reitin, jolla on vähiten 
kaupunkeja. Näin ollen löydettyjä reittejä ei tarvitse vertailla, vaan voidaan palauttaa suoraan 
ensimmäinen löydetty reitti. Funktion ajon aikana jokaiseen kaupunkiin talletetaan reitti, josta
kyseiseen kaupunkiin on päästy. Näin ollen löydetty reitti voidaan suoraan palauttaa, eikä kuljettua
reittiä tarvitse mennä takaisin lisäten kaupunkeja vektoriin, joka sitten pitäisi kääntää. 
Funktiossa käytetään std::listiä sen sopivien ominaisuuksien takia. 


-----road_cycle_route:

Funktio on toteutettu hyödyntäen DFS algoritmia, joka mahdollistaa syklien tarkastelun. Funktiossa
on hyödynnetty std::stackiä sen hyvien ominaisuuksien takia. Kuljettu reitti talletetaan suoraan 
vektoriin, josta mahdolliset löytyneet umpikujat poistetaan perältä. Näin ollen voidaan suoraan 
palauttaa kyseinen vektori syklin löytyessä.


-----shortest_route:

Funktio on toteutettu hyödyntäen A* algoritmia, sen suunntaun reitin haun takia. Funktiossa 
käytetään std::priority_queueta sen hyvien ominaisuuksien takia. Tietorakenne mahdollistaa 
kaupunkien järjestämisen etäisyyden mukaan. Etäisyytenä annetaan tien pituuden arvo negatiivisena,
sillä priority_queue järjestää alkiot suurin ensin. Näin ollen vältytään funktiota hidastavilta
lambdafunktioilta, joilla priority_queuen järjestämistä muokattaisiin.

Funktion ajon aikana jokaiseen kaupunkiin talletetaan reitti, josta kaupunkiin on päästy. Näin ollen
reitti voidaan suoraan palauttaa reitin päätekaupungin löytyessä.

Funktio hyötyy valmiiksi lasketuista reittien pituuksista.


-----trim_road_network:

Funktio on toteutettu käyttäen Kruskalin algoritmia. Funktion tietorakenteessa käytetään vectoria,
johon talletetaan unordered_settejä, jotka sisältävät Town* osoittimia. Lisäksi funktiossa luodaan
toinen vektori, johon päätietorakenteen setissä järjestetyt tiet lisätään. Päätietorakenne 
tyhjennetään kokonaisuudessaan ja uudet lyhimmät reitit lisätään takaisin. 

Kaupunkeihin talletetaan indeksi, joka vastaa vectoriin talletettua settiä. Näin kahden kaupungin
vertailu ja oikean setin löytäminen on nopeaa. Kun molemmat kaupungit on jo kerran lisätty settiin,
mutta setit ovat erilliset, tarkastetaan kumpi setti on pienempi ja kyseisen setin alkiot lisätään
suurempaan samalla indeksit muuttaen.


