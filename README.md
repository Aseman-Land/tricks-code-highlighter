# Tricks Code Highlighter

It's a tools, makes it easy to create and share beautiful images of your source code like Carbon.
It's fast, easy and written using C++/Qt and [KSyntaxHighlighter](https://github.com/KDE/syntax-highlighting).

![](https://aseman.io/projects/tch/tch-dark.png)

## Build

To build tricks-code-highlighter you needs Git, Ideal C++ compiler, Qt5 and KSyntaxtHighlighter installed on your system. So:

```bash
sudo apt-get install git g++ qtbase5-dev libkf5syntaxhighlighting-dev
```

and Then start building:

```bash
git clone git@github.com:Aseman-Land/tricks-code-highlighter.git
cd tricks-code-highlighter
mkdir build && cd build
make -j2
sudo make install
```

## Usage

### GUI

You can run editor dialog using below command:

```bash
tricks-code-highlighter
```

![](https://aseman.io/projects/tch/dialog.png)

### Terminal

It's also available on the terminal. this is available commands:

```bash
  -h, --help            Displays help on commandline options.
  --help-all            Displays help including Qt specific options.
  -v, --version         Displays version information.
  -p                    Show progress during copy
  -i, --input <file>    Input source code file. (Required)
  -o, --output <file>   Output image file path. (Required)
  -w, --width <pixels>  Width of output image
  --font-family <name>  Editor font family name.
  --font-size <pixels>  Editor font size in pixels.
  --theme <theme>       Select highlighter theme
  --frame <theme>       Select frame theme
  --line-numbers        Visible line numbers
  --print-themes        Print all available themes.
  --print-definitions   Print all available definitions.
  --print-frames        Print all available frames.
```

To create an image on light mode:

```bash
tricks-code-highlighter --input test.rs --output test.png --frame "Light" --theme "Breeze Light" --width 1600 --line-numbers
```

![](https://aseman.io/projects/tch/tch-light.png)

Or on dark mode:

```bash
tricks-code-highlighter --input test.rs --output test.png --frame "Dark" --theme "Breeze Dark" --width 1600 --line-numbers
```

