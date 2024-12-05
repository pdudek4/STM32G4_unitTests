
[![pipeline status](https://gitlab.ekoenergetyka.com.pl/przemyslaw.dudek/stm32g4_unittests/badges/main/pipeline.svg)](https://gitlab.ekoenergetyka.com.pl/przemyslaw.dudek/stm32g4_unittests/-/commits/main)

[![Latest Release](https://gitlab.ekoenergetyka.com.pl/przemyslaw.dudek/stm32g4_unittests/-/badges/release.svg)](https://gitlab.ekoenergetyka.com.pl/przemyslaw.dudek/stm32g4_unittests/-/releases)


## Testowy projekt Unit testów STM32 Ceedling i GTest

Zaimplementowano dwa rodzaje środowisk testowych:
1) [Ceedling](https://github.com/ThrowTheSwitch/Ceedling) w wersji 0.31,
2) [GTest](https://github.com/google/googletest) wraz z Gmock.

Pliki testowe znajdują sie w katalogu `tests`.
Projekt oparty na kodzie IMD6CH Measure Bootloader.
Ceedling wymaga instalacji Ruby'iego co może odrzucać, jednak instalacja GTestów na Linuxie też nie jest bezproblemowa.

### Cechy Ceedling
- cały kod projektu nie musi się kompilować w GCC_x64, wystarczy dany moduł i jego zależności jeżeli chcemy z nich korzystać. Pozwala na częściowe testowanie kodu.
- wspiera tylko kod w C.
- banalnie proste tworzenie mocków; każdy przypadek testowy może używać innych mocków,
- łatwe używanie mocków,
- nie można łatwo debugować; niby można jakoś VSCode ustawić, ale nie robiłem tego,
- częściowa integracja z STM32CubeIDE,
- uruchamianie z IDE lub konsoli (lepsze rozwiązanie) - brak konfiguracji,
- błędy budowania tylko w konsoli na zewnątrz IDE,
- można printować na konsole przy uruchamianiu kodu i testów,
- można uruchamiać pojedyncze pliki testowe,
- runtime error od razu widać, bo nie zobaczymy wyniku.

### Cechy GTest
- cały nasz kod projektu musi się kompilować w GCC lub G++.
- możliwe użycie C i C++ w jednym projekcie.
- duży problem z mockami, trzeba pisać wszystko ręcznie; trzeba używac mocków wszędzie,
- trudność w używaniu mocków,
- można debugować cały kod projektowy i produkcyjny w GDB i w STM32CubeIDE,
- pełna integracja z STM32CubeIDE, ale wymaga to dość sporo pracy - poradniki na dole;
- uruchamianie z IDE (lepsze rozwiązanie) lub konsoli,
- błędy kompilacji w IDE okienko "Problems" tak jak w zwykłym budowaniu projektu,
- nie można nic printowac na konsole,
- uruchamianie wszystkich testów na raz; pojedyncze uruchamianie tylko jako argument budowania - dość problematyczne,
- jeżeli jest runtime error, to ciężko znaleźć, trzeba uważać czy testy wykonały się wszystkie czy jest błąd w trakcie, łatwo pominąć.

## Instalacja Ceedling
Wymagane jest posiadanie pakietu [Ruby](https://www.ruby-lang.org/en/) na swoim komputerze.
Narzędzie Ceedling w wersji stabilnej (master) 0.31 na 11.24r. współpracuje tylko z wersjami Ruby poniżej 3.0.0.
Najnowsza wersja Ruby to 3.3.6. Popularna wersja Ruby 2.7.1 dobrze wspólpracuje z wieloma narzędziami i to ją polecam zainstalować.

Obecnie rozwijana jest wersja Ceedling 1.00, która ukaże się niebawem. Posiada o wiele więcej opcji. Wymaga Ruby w wersji >3.0.0.
Można ją przetestować ściągając z gita podanego powyżej jako branch 0.32_rc lub jako [obraz dockera](https://hub.docker.com/r/throwtheswitch/madsciencelab).

### Instalacja Ruby Linux
Najłatwiej jest zainstalować ruby za pomocą rbenv. Jest to manager wersji ruby.
Ma dużo opcji, dla nas najważniejsze jest to że instaluje wszystie potrzebne zależności. Miejsce na dysku około 80 MB.
[Poradnik instalacji rbenv](https://www.techiediaries.com/install-ruby-2-7-rails-6-ubuntu-20-04/).
Polecana jest instalacja taka jak w poradniku przez `git clone`, ponieważ wersja z `apt install rbenv` jest stara i nieaktualna.
Uwaga! W poradniku dochodzimy do instalacji `gem bundler` włącznie. Zatrzymujemy się na *Installing Ruby on Rails 6*, bo tego nie potrzebujemy.

### Instalacja Ruby Windows
Wymagane jest posiadanie pakietu [MSYS2](https://www.msys2.org/), którego większość programistów posiada. Miejsce na dysku około 800 MB.
Ruby pobieramy [stąd](https://rubyinstaller.org/downloads/archives/) w sekcji **RubyInstallers** wyszukujemy `Ruby 2.7.1 (x64)`.
Uwaga! Nie instaluj Ruby w ścieżce, gdzie są spacje np. *"C:\Program Files"*. Upewnij się, że zaznaczyłeś *"Add Ruby executables to your PATH"* podczas instalacji.
Jeżeli nie, zrób to ręcznie.

### Instalacja pakietu Ceedling
W terminalu wykonujemy polecenie `gem install ceedling`. Po tym komenda `ceedling version` powinna zwrócić `Ceedling:: 0.31.1`.
Koniec.

## Instalacja GTest i GMock
Wymagane jest posiadanie pakietu [CMake](https://cmake.org/).
GTest wymaga pobrania repozytorium z gita i skompilowanie na własnym komputerze natywnym kompilatorem.
Następnie skompilowane biblioteki oraz pliki nagłówkowe należy skopiowac do odpowiednich lokalizacji.

### Instalacja GTest i GMock Linux
#### GTest
[Poradnik YouTube](https://www.youtube.com/watch?v=6dqFqh3UbPY).
#### GMock
Instalacja GMock Linux. Polecenia podobne jak w przypadku GTest\
`sudo apt install libgmock-dev`\
`cd /usr/src/googletest/googlemock`\
`sudo cmake -Bbuild`\
`sudo cmake --build build`

Utowrzyły się dwie biblioteki statyczne:\
static library lib/libgmock.a\
static library lib/libgmock_main.a

Kopiowanie bibliotek do /usr/lib\
`sudo cp ./build/lib/libgmock* /usr/lib`

### Instalacja GTest i GMock Windows
Wymagane posiadanie MinGw (git), Python3, GNU Make, CMake.\
[Poradnik YouTube](https://www.youtube.com/watch?v=3zUqJEilhnM)\
GMock podobnie jak wyżej\
lub\
[Poradnik](https://gist.github.com/motchy869/22d873415722a1c10bc77d3f761339dc)



## Użycie Ceedling w Eclipse / STM32CubeIde
Ten projekt jest skonfigurowany według poniższych zaleceń.
https://www.throwtheswitch.org/eclipse

Gdzie jest dany GEM?\
`gem which ceedling`\
~/.rbenv/versions/2.7.1/lib/ruby/gems/2.7.0/gems/\
~/.rbenv/versions/2.7.1/lib/ruby/gems/2.7.0/gems/ceedling-0.31.1/bin/ceedling

Eclipse ma błąd, że "/usr/bin/env: ‘ruby’: No such file or directory"
to trzeba dodać mu symlinka.

Gdzie ruby\
`which ruby`\
/home/przemyslaw/.rbenv/shims/ruby /usr/bin/ruby

Zrób soft symlinka do tego\
sudo ln -s /home/przemyslaw/.rbenv/shims/ruby /usr/bin/ruby

Scieżka do unity\
/home/przemyslaw/.rbenv/versions/2.7.1/lib/ruby/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/

## Użycie GTest w Eclipse / STM32CubeIde
Ten projekt jest skonfigurowany według poniższych zaleceń.\
https://michael.kafarowski.com/blog/unit-testing-with-stm32cubeide/ \
https://stackoverflow.com/questions/3951808/using-googletest-in-eclipse-how

