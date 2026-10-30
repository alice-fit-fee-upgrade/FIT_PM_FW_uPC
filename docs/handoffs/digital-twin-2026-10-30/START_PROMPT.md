# Start prompt — PM12 digital twin

Kontynuuj projekt FIT PM12 ATxmega128A3U w nowym wątku digital twin.
Najpierw przeczytaj `/home/codex-hil/AGENTS.md`, projektowe AGENTS.md i
`/home/codex-hil/fit-pm12-pll-c/docs/handoffs/digital-twin-2026-10-30/HANDOFF.md`.
Repo: https://github.com/alice-fit-fee-upgrade/FIT_PM_FW_uPC,
gałąź `development/dac-c-simulation`, firmware checkpoint
`f0b2c7ef68e3ac737a1db17239a1af6f3a5f0ef1`; późniejszy commit przekazania dodaje
wyłącznie dokumentację/skrypty. Nie zaczynaj odzyskiwania firmware od nowa.

Najpierw zweryfikuj manifesty i zbuduj offline na Dellu skryptem
`tools/handoff/dell_build.sh` w izolowanym checkout. Przeczytaj wcześniej
`/home/codex-hil/docs/toolchains/heavy-builds-remote-handoff.md`.
Nie programuj, nie resetuj ani nie odpytuj sprzętu: użytkownik czeka na gotowy
symulator. Nie zmieniaj golden reference ani exact_asm. `make exact-check`
musi dawać0 różnic dla baseline; kandydaty development celowo mają inne SHA.

Cel: odtwarzalny digital twin CPU i peryferiów do różnicowego testowania
oryginału i kolejnych implementacji C, zaczynając od istniejących testów AVR.
Oddziel fakty fizyczne, testy programowe, modele i hipotezy. Prywatny ABI,
SREG, R1/banki,3-bajtowy stos powrotu, MMIO/SPI/UART i kolejność IRQ są częścią
kontraktu. Nie uznawaj istniejącego interpretera za model cycle-accurate.

Priorytet: reset/PGOOD/ADT7311 i współdzielenie PA1 między bitbang SPI oraz I2C,
bo próba PLL C przeszła odczyt PLL, lecz poległa na systemowym odczycie ADT
(ID00) i została wycofana. Przyczyna nieustalona. Kandydaty DAC/scaling mają
2108416 historycznych porównań PASS, lecz walidacja analogowa/HW NOT_RUN.
Nie powtarzaj kosztownych macierzy bez zmiany bajtów lub nowej potrzeby.

Najpierw sprawdź kompletność modeli i pokrycie istniejących testów, następnie
zaproponuj najmniejszy uruchamialny scenariusz twin z jawnymi ograniczeniami.
Zachowaj osobne, wersjonowane obrazy original/default-development/DAC-candidate
oraz logi i SHA256. Oczekiwany pierwszy wynik: reprodukowalny software-only
runner, raport braków modeli i scenariusz regresji; żadnych automatycznych
operacji na PM12. Schemat/mapa fizycznego symulatora nadal wymagają weryfikacji.
