ftrest / ftrestd - klient / server aplikácia na prenos súborov.

Popis:
-------
Program využívá protokol HTML a rozhranie RestAPI a umožňuje základnú manipuláciu so súbormi a adresármi pomocou konzolovej aplikácie. 
Umožňuje vytvoriť, alebo zrušíť adresár, prípadene vypísať jeho obsah. Podporuje nahratie súbora na vzdialený server, stiahnutie súbora a prípadné vymazanie vzdialeného súboru.

Ovládanie:
------
**ftrest** COMMAND REMOTE-PATH [LOCAL-PATH]
* **COMMAND** je príkaz z množiny: 
  * **get** - stiahnutie súboru
  * **put** - nahratie súboru
  * **del** - vymazanie súboru
  * **lst** - vypísanie obsahu adresára
  * **mkd** - vytvorenie adresára
  * **rmd** - vymazanie adresára
* **REMOTE-PATH** je cesta k súboru alebo adresári na serveri
* **LOCAL-PATH** je cesta v lokálnom súborovom systéme, povinné pre put

**ftrestd** [-r ROOT-FOLDER] [-p PORT]
* **-r ROOT-FOLDER** špecifikuje koreňový adresár, kde budú ukladané súbory pre jednotlivých užívateľov, defaultná hodnota je aktuálna
* **-p PORT** špecifikuje port, na ktorom bude server naslúchať, implicitne 6677

Návratová hodnota:
-------
Klient vráti 0 pri úspešnom vykonaní operácie. V prípade chyby klient vráti 1 a na štandartný chybový výstup vypíše krátky popis chyby.

Autor:
------
(c) Juraj Kubiš <xkubis15@stud.fit.vutbr.cz>
