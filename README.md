# sysopki
Przykłady napisane na potrzeby zajęć z systemów operacyjnych


Celem tego repo nie jest udostępnienie gotowych rozwiązań, ale skrócenie czasu potrzebnego na napisanie swoich własnych implementacji zadań.
Mam nadzieje, że dzieki temu nie będziecie musieli pisać ulepów, ale będziecie mieli czas skupić sie na jakośc kodu.
Poszczególne foldery zawierają skonfigurowane(cmake) projekty z przykładami mechanizmów wykorzystywanych w danym labie.

Aby zbudować przykłady z danego laba wywołaj:
```
$ cd lab1
$ cmake .
$ make all
```


Jeśli chcecie znaleźć coś konkretnego w plikach to polecam:
```
$ grep -rine "funkcja_ktorej_szukam"
```



## Spis treści
Poszczególne zestawy prezentują:
* lab1:
    * struktura projektu
    * cmake
    * pomiary czasu
* lab2:
    * getopt(czytanie lini poleceń)
    * getrandom
    * nftw
    * readdir
* lab6:
    * kolejki komunikatów system V i POSIXowa
    * prosta obsługa konsoli
    * podstawowe wątki(pthread_create) 



## Kontakt
Jeśli macie pytania to piszczie albo zakładajcie issue lub pull requesty

Jak to skończe to i tak pewnie wyląduje na WIETowym gitlabie

**Krzysztof Kolasa** - [kolasak](kyskolasa2@gmail.com)
