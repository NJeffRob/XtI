# XYZ to Input (XtI)
XtI is a command line tool to generate input files from XYZ coordinate files for several computational chemistry programs. As of June 2025, the following electronic structure programs are supported:

- ORCA
- Gaussian
- FHI-aims
- Gamess (US)

Please see [Contributing](#contributing) if you'd like to get involved.


## Table of Contents
- [About](#about)
- [Dependencies](#dependencies)
- [Testing](#testing)
- [Installation](#installation)
- [Use](#use)
- [Examples](#examples)
- [Contributing](#contributing)


### About
DFT has become a crucial tool for calculating reaction pathways, predicting spectral properties, and analyzing molecular properties. Despite its prevalence, the manual process of generating input files for DFT calculations remains cumbersome and time-consuming. This leaves researchers to manage file conversions and submissions manually, wasting valuable time that could be spent on other tasks. To address this, we present XYZ to Input (XtI), a command-line tool designed to streamline the generation of input files for molecular DFT calculations. XtI is currently compatible on Linux (Ubuntu/Debian tested), with plans in the future to bring it to macOS and Windows. XtI emphasizes ease of use and portability, and can be installed with a simple shell script from a pre-compiled binary.

**Note** It is also possible to compile the program from source via Makefile, but this has not been rigorously tested on all architectures. If you compile it on your own system, we'd love to hear from you!


### Dependencies
XtI was written in Lua 5.4.7 and C99. Due to the use of `<const>` in the Lua code, Lua 5.4 is required as a minimum.

GCC is used in the Makefile to compile the C programs. To have compatibility with C99, a GCC version of >= 4.5 is required.

### Installation
To install XtI, perform the following:
1. Clone the XtI project from GitHub or download and unzip it in a relevant directory
2. Type ```chmod +x install_xti.sh``` to turn it into an executable
3. Type ```source ~/.bashrc``` to refresh your terminal
4. Type ```xti -h``` to ensure that the program has been installed correctly.

Should you want to compile the program from scratch, the following distributions/architectures have been tested and work:
- AMD Ryzen 7, x86_64, (K)Ubuntu 24.04.2
- Intel Haswell, x86-64 PopOS (Ubuntu 22.04)

##### Ubuntu
1. Download XtI into a folder of your choosing. We recommend `~/.local/share/applications` or `~/.local/share/bin`. Note that you may have to create the latter depending on your distribution. 
2. Run `make` to install XtI, and `make test` to ensure it's functioning properly.
3. Once compiled, perform the same general steps above to execute the ```install_xti.sh``` script.


### Testing
Integration tests on C input handling with the [Check](https://libcheck.github.io/check/).

Dependencies are check, and pkg-config, installed with `sudo apt install check pkg-config`

Run `make` and `make test`

The file `scripts/generate_test.py` will automatically create the boilerplate for a C test and update the Makefile. The script can be ran with `python3 scripts/generate_test.py <test_name>.c`


### Use
XtI comes with the following options:

- `-i`: Specifies generation of an input file from an XYZ file.
- `-o`: Specifies generation of an XYZ file from an output file.
- `-s`: Generates a job submission script for submitting calculations to a scheduler.
- `-h`: Displays a help message on how to use the program.

XtI can be used as follows:

`xti [options] [quantum chemistry program] [job type] [file]`

By default, XtI will assume the `[job type]` parameter is "single point" (sp). The other options for it are "opt" or "freq". 

The currently supported list of `[programs]` can be seen in the table below. Their usage is demonstrated in brackets.

|         Electronic Structure Program           |
|--------------------|
| 		ORCA (`orca`)                    |
|       Gaussian16 (`gaussian`)           |
|       FHI-aims (`fhiaims`)             |
|       Gamess (US) (`gamess`)     |

XtI allows for custom or defaults for the input files generated. The default parameters are as follows:

| Parameter   | Default     |
| ----------- | ----------- |
| Functional  | PBE0        |
| Basis Set   | def2-SVP    |
| Dispersion  | D3BJ        |
| cpus        | 4           |
| memory      | 2GB         |
| charge      | 0           |
| mult        | 1           |

These defaults were chosen with molecular geometries in mind. You can modify these defaults by changing the constants in the `output_to_xyz_converter`, `sh_generator` and/or `xyz_to_input_converter` Lua files. Of note, you'll have to change some of the defaults to add in your user account if you're planning on using this on a High Performance Cluster.

It is encouraged that you double check that your initial files are correct before using the script repeatedly.


### Examples
1. Generate an ORCA input file (.inp) and submission script (.sh) from a .xyz coordinate file of water:

```
input: xti -is orca water.xyz
output: water.inp, water.sh
```

2. Generate a .xyz coordinate file from a Gaussian output (.log) file of water:

```
input: xti -o gaussian water.log
output: water.xyz
```

3. Generate a submission script (.sh) for FHI-AIMS for a file called water:

```
input: xti -is fhiaims water
output: water.sh
```

4. Generate a Gaussian frequency calculation input (.gjf) and submission script (.sh) from a .xyz coordinate file of water:

```
input: xti -si gaussian freq water.xyz
output: water-freq.gjf, water-freq.sh
```


### Contributing
If you'd like to contribute to XtI, please feel free! If you're unsure how to contribute, the following are things that would be helpful:

1. Adding more electronic structure programs.
2. Adding more input options for existing programs.
3. Compilation testing for Windows and macOS executables.
4. If you use XtI and encounter a bug, please report it to us.

To help with contributing, the `test/` and `scripts/` directories contain helpful testing tools.
