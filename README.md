# Émulateur & Interpréteur CHIP-8

Un interpréteur CHIP-8 moderne et léger écrit en **C++20**, doté d'un rendu graphique monochrome accéléré matériellement grâce à **Raylib**.

---

## Présentation

Le CHIP-8 est un langage de programmation interprété développé au milieu des années 70 pour les micro-ordinateurs 8 bits. Ce projet émule fidèlement le matériel virtuel d'origine, gérant la mémoire, l'état des registres, la pile d'appels, les timers et les entrées clavier.

### Spécifications techniques implémentées

* **Mémoire :** 4096 octets (4 Ko) avec un point d'entrée standard des programmes à l'adresse `0x200`.
* **Registres :** 16 registres de données 8 bits à usage général (`V0` à `VF`), `VF` servant de drapeau de retenue/collision (carry flag).
* **Registre d'Adresse (`I`) :** 16 bits, utilisé pour l'indexation de la mémoire et la lecture des sprites.
* **Program Counter (`PC`) & Pile :** Pile d'appels à 16 niveaux avec son pointeur de pile 8 bits (`SP`).
* **Timers :** Timers indépendants pour le délai et le son, cadencés à 60 Hz.
* **Affichage :** Grille de pixels monochromes 64x32 rendue via l'opération XOR avec détection de collision des sprites.
* **Fréquence d'Horloge :** Cycle d'exécution CPU dynamique (~600 Hz) découplé de la boucle de rafraîchissement vidéo (60 FPS).

---

## Contrôles & clavier

Le CHIP-8 d'origine utilisait un clavier hexadécimal de 16 touches. Les contrôles sont ici remappés directement pour un clavier d'ordinateur standard (AZERTY/QWERTY) :

| Clavier CHIP-8 | Clavier PC (AZERTY/QWERTY) |
| :---: | :---: |
| `1` `2` `3` `C` | `1` `2` `3` `4` |
| `4` `5` `6` `D` | `A` `Z` `E` `R` |
| `7` `8` `9` `E` | `Q` `S` `D` `F` |
| `A` `0` `B` `F` | `W` `X` `C` `V` |

---

## Compilation & installation

Ce projet utilise **CMake** avec la gestion automatique des dépendances via `FetchContent`. **Aucune installation manuelle de Raylib n'est requise**, le script se charge de la télécharger et de la compiler.

### Prérequis

* Un compilateur compatible C++20 (`g++ >= 10`, `clang >= 11`, ou MSVC 2019+)
* CMake 3.16+
* Git

### Instructions de build

```bash
# 1. Cloner le dépôt
git clone https://github.com/gabinlsc/chip8-emulator.git
cd chip8-emulator

# 2. Configurer l'environnement (télécharge Raylib automatiquement)
cmake -B build

# 3. Compiler l'exécutable
cmake --build build
```

---

## Utilisation

Lancez l'émulateur compilé en lui passant le chemin vers n'importe quelle ROM `.ch8` valide en argument :

```bash
# Linux / macOS
./build/chip8_emu roms/pong.ch8

# Windows
.\build\Debug\chip8_emu.exe roms\pong.ch8
```

---

## Architecture du Projet

```text
.
├── CMakeLists.txt      # Configuration CMake & auto-téléchargement de Raylib
├── include/
│   └── chip8.hpp       # Définitions du CPU, architecture mémoire et registres
├── roms/               # Dossier contenant les programmes de test et jeux rétro
├── src/
│   ├── chip8.cpp       # Boucle Fetch-Decode-Execute et décodeur d'opcodes
│   └── main.cpp        # Fenêtre, boucle de rendu Raylib et gestion du clavier
└── README.md
```

---

## Licence

Distribué sous la licence MIT. Voir le fichier `LICENSE` pour plus d'informations.
