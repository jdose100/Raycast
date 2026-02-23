{ pkgs ? import (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/a82ccc39.tar.gz";
  }) {} }:

with pkgs;

mkShell rec { 
  buildInputs = [
    gccNGPackages_15.gccWithLibc
    radare2
    sdl3 
  ];
  
  shellHook = ''
    export CMAKE_PREFIX_PATH="${pkgs.sdl3.dev}/lib/cmake:$CMAKE_PREFIX_PATH"
  '';

  LD_LIBRARY_PATH = lib.makeLibraryPath buildInputs;
}
