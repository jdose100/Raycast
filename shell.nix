{ pkgs ? import (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/a82ccc39.tar.gz";
  }) {} }:

with pkgs;

mkShell rec { 
  buildInputs = [
    gccNGPackages_15.gccWithLibc
    cmake gnumake doxygen
 
    vulkan-loader
    alsa-lib 
    libGL

    # X11
    libX11
    libXrandr
    libXinerama
    libXext
    libXi
    libXcursor
  ];

  LD_LIBRARY_PATH = lib.makeLibraryPath buildInputs;
}
