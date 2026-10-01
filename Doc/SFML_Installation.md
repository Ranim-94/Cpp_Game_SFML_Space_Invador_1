# SFML

## Table of Contents

- [SFML](#sfml)
  - [Table of Contents](#table-of-contents)
  - [Context](#context)
  - [Installation using `apt`](#installation-using-apt)
    - [Verification for `apt`](#verification-for-apt)
  - [Installing from source](#installing-from-source)
  - [Global verification](#global-verification)
    - [Configuring SFML in Eclipse](#configuring-sfml-in-eclipse)

## Context 

Step by step guide to install and verfiy **SFML**
under Linux.

## Installation using `apt`

To install all `SFML`  modules (Graphics, Audio, Window, System, Network) at once, run the following command

<pre>sudo apt install libsfml-dev</pre>

### Verification for `apt`
To verify all modules are installed by `apt`, we use `dpkg` command :

<pre>dpkg -l | grep sfml</pre>

<u>Note</u>: the `ii` we see at the start means the package is installed and working.

## Installing from source

Since `apt` is behind latest version, because **SFML** is in version 3 now (there are some differences between 2nd and 3rd version), so its preferrable to download it from their [weibsite](https://www.sfml-dev.org/download/)

Once the package is downloaded, we proceed to the following commands:

1) Extract the files 
<pre>tar -xzf SFML-3.1.0-linux-gcc-64-bit.tar.gz</pre>

2) go inside the directory

<pre>cd SFML-3.1.0</pre>

3) Since this libraries is donwloaded by us and not a package manager, we  copy headers (`include`) and libraries (`lib`) into `usr/local`

<pre>sudo cp -a include/ lib/ /usr/local/</pre>

4) Finally Update Linux’s shared-library cache

<pre>sudo ldconfig</pre>

<u>Note:</u> `ldconfig` updates Linux’s cache of shared libraries
When **SFML** is installed in `/usr/local/`, the files may exist, but Linux may not yet know where to find them when your program starts. Running `sudo ldconfig` deos the following:

- Scans configured library directories such as `usr/lib` and `usr/local/lib`
- Finds shared libraries such as `libsfml-graphics.so`
- Updates the cache used by the dynamic linker

## Global verification

Now once we finish installation in our system, we can 
verify its installation:

<pre>ldconfig -p | grep -i sfml</pre>


### Configuring SFML in Eclipse

To link the libraries dynamically in `Eclipse`, right click on the project, click `Properties` -> `C/C++ general` -> `Path and Symbols`-> `Libraries`, as shown in [Figure 1](#fig1)

<div id="fig1">
<img src="Figures_VsCode/eclipse_sfml_config.png" alt="Widget Types" width="450">
<p><strong>Figure 1:</strong> Configuring SFML in Eclipse</p>
</div>

Then add the following linker libraries:

<pre> "sfml-graphics" "sfml-audio" "sfml-window" "sfml-system" </pre>