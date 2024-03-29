# EPIC - Efficient Power Index Computation

[[_TOC_]]

# Compilation

## Prerequisites
C++ Compiler, [GNU make](https://www.gnu.org/software/make/), [GMP (Gnu Multiple Precision Arithmetic Library)](https://gmplib.org/), [Git](https://git-scm.com/) (optional)

If you need help to install the prerequisites, see the [install prerequisites section](#install-prerequisites) below.

## Compile
1. If not already done, download the repository. This can be done in two different ways:
    1. Clone the repository using git (`git clone https://github.com/jhstaudacher/EPIC.git`)
    1. Download the repository as ZIP archive using the download dropdown menu on the Repository overview. The archive must get extracted after download.
1. Go to the cloned or extracted directory (`cd <path_to>/epic`) (in MSYS2 the drive `C:` can get accessed via `cd c/`).
2. If the GMP library was manually downloaded and installed (not covered in this manual), add its installation directory with the `-L` option to the `LDFLAGS` variable in the `makefile`.
3. Compile the application using make (`make all`). Checkout all available targets using `make help`.
4. The compiled application `epic` will appear in the top-level directory

## Cleanup
If you want to remove all compilation products including the application itself, use `make clean`

## Install prerequisites

### Windows

* Download and install MSYS2 according to the instructions on the [website](https://www.msys2.org/).
* Install the required packages using `pacman -S msys2-devel make git gmp gmp-devel`
* Now continue with the instructions in the [Compile section](#compile) running the commands in the MSYS2-Terminal.

### Linux
All the needed tools can be installed using the distributions package manager (e.g. for apt package manager run: `apt install gcc git make libgmp-dev`).

### MacOS

* Download XCode's command line tools by running `xcode-select --install` in Terminal
* Through [HomeBrew](https://brew.sh) install the other dependencies by running `brew install git gmp`

# Usage
First open a terminal and go to the `epic/` directory: `cd <path_to>/epic`. After that the program can get called by typing `./epic` (`./epic.exe` on Windows) followed by the options listed below. Additional to the lists below you can always get this information by calling `./epic --help`.

## Options
| required? | option | argument | description |
| -------- | ------ | -------- | ----------- |
| yes | `-i` / `--index` | index shortcut | the index to compute |
| no | `-p` / `--precoalitions` | has to be set when working with precoalitions | the structure of the precoalitions (*1)|
| yes | `-w` / `--weights` | .csv file | the player weights for the weighted voting mGame (*2) |
| yes | `-q` / `--quota` | integer or float | the quota for the weighted voting mGame (*3)|
| no | `-v` / `--verbose` | no argument | adds extra output |
| no | `--quiet` | no argument | reduces output to a minimum |
| no | `-f` / `--filter-null` | no argument | excludes null player from the calculation (not compatible with all indices!) |
| no | `--gmp` | no argument | enforces the use of GMP-classes for large integer calculations |
| no | `--primes` | no argument | enforces the chinese lint theorem for large integer calculations |
| no | `--float` | no argument | reads the weights and the quota as floating point numbers |
| no | `--csv` | no argument | write the results into a .csv file |

(*1) Has to be before -w. When setting the precoalitions to `0`, the precoalitions will be read from the weightsfile. This also means that the weightsfile has to be written accordingly. If defined in the weightsfile, the players in the same precoalition should all be in the same line and seperated by ",". A new line is a new precoalition. If precoalitions are defined in the command line, playernumbers in a precoalition will be connected with a "+" and seperated with a ",". Playernumbers are from 1 to n. For example: "-p 1+3,2+4,5" would be a preacoalition between player 1 and 3, player 2 and 4 while player 5 is alone. 

(*2) The .csv file must contain one weight per line. The weights must all be integers unless you set the --float flag.

(*3) When setting the quota to `0`, the first line of the weights file will be interpreted as the quota to use. This might be useful for automated program calls.


## Available indices

| shortcut | name |
| -------- | ---- |
| `ABZ` | Absolute Banzhaf |
| `APG` | Absolute Public Good |
| `APIG` | Absolute Power Index G (based on the Dubey-Shapley identity) |
| `BZ` | Banzhaf |
| `BO` | Banzhaf Owen |
| `BOB` | Banzhaf Owen from below |
| `BZB` | Banzhaf from below |
| `CC` | Coleman Collective |
| `CI` | Coleman Initiative |
| `CP` | Coleman Preventive |
| `DP` | Deegan Packel |
| `FT` | Felsenthal |
| `HN` | Harsanyi Nash |
| `J` | Johnston |
| `KB` | Koenig Braeuninger (based on the Dubey-Shapley identity) |
| `KBPH` | Koenig Braeuninger (based on Raw Public Help Theta) |
| `N` | Nevison (based on the Dubey-Shapley identity)|
| `NPH` | Nevison (based on Raw Public Help Theta) |
| `O` | Owen |
| `OPGI` | Owen Extended PGI |
| `OB` | Owen from below |
| `PG` | Public Good |
| `PHT` | Public Help Theta (based on the Dubey-Shapley identity) |
| `PHTPH` | Public Help Theta (based on Raw Public Help Theta) |
| `PHX` | Public Help Xi |
| `PIF` | Power Index F |
| `PIG` | Power Index G (based on the Dubey-Shapley identity)|
| `PIGPH` | Power Index G (based on Raw Public Help Theta) |
| `RA` | Rae |
| `RBZ` | Raw Banzhaf |
| `RBZB` | Raw Banzhaf from below |
| `RDP` | Raw Deegan Packel |
| `RFT` | Raw Felsenthal |
| `RJ` | Raw Johnston |
| `RPG` | Raw Public Good |
| `RPHT` | Raw Public Help Theta |
| `RPHTSD` | Raw Public Help Theta (based on the Dubey-Shapley identity) |
| `RPIG` | Raw Power Index G |
| `RPIF` | Raw Power Index F |
| `RSH` | Raw Shapley Shubik |
| `SCB` | Symmetric Coalitional Banzhaf |
| `SCBB` | Symmetric Coalitional Banzhaf from below |
| `RSHB` | Raw Shapley Shubik from below |
| `SH` | Shapley Shubik |
| `SHB` | Shapley Shubik from below |
| `SPGI` | Solidarity Public Good Index |
| `T` | Tijs |
| `TPGI1` | Threat Public Good Index 1 |
| `TPGI2` | Threat Public Good Index 2 |
| `TPGI3` | Threat Public Good Index 3 |
| `UPGI` | Union Public Good Index |
| `W` | Number of winning coalitions|
| `WM` | Number of minimal winning coalitions |
| `WS` | Number of minimal winning coalitions of smallest cardinality |

## Example

For a simple example we want to calculate the Banzhaf index for a simple 5-player mGame with the weights (9, 5, 3, 1, 1) and quota 11. Therefore create the file `weights.csv` inside the `epic/` directory and add the weights (one per line): 9, 5, 3, 1, 1.

Then call the program: `./epic -i BZ -w ./weights.csv -q 11 -v`

By adding the `-v` we get extra output like the raw Banzhaf values and the total number of swings (sum over the raw Banzhaf values).


# For developer

Everything described in this section was only tested on Linux (Ubuntu). To use the mentioned tools and procedures may need some modifications to run them with MSYS2 on Windows.

## Documentation generation

### Prerequisites
[doxygen](https://www.doxygen.nl/download.html), [graphviz](https://graphviz.org/download/) (for generating graphs), [LaTeX](https://www.latex-project.org/get/) (only for pdf documentation), [GNU make](https://www.gnu.org/software/make/) (only for pdf documentation)

### Generate
1. running `doxygen Doxyfile` generates the `docs` directory
1. For the html documentation open `docs/html/index.html`
1. For the pdf documentation:
	1. goto the `docs/latex` directory
	1. run `make`
	1. open `docs/latex/refman.pdf`


## Code-Formatting

### Prerequisites
clang-format (`apt install clang-format`)

### Format
The style is defined in the `.clang-format` file.
1. **manual**: `clang-format -style=file -i <file(s)>`
1. **script**: run the `format.sh` script to format all .h and .cpp files in the directory
