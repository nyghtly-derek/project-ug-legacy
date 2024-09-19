{
  description = "Dev environment for project ugly gob.";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system: let
        pkgs = import nixpkgs { inherit system; };
        in {
            packages = {
                default = pkgs.stdenv.mkDerivation {
                    name = "play-gdg";
                    src = ./.;
                    nativeBuildInputs = with pkgs; [
                        gnumake 
                        (if pkgs.stdenv.isDarwin then clang else gcc)
                    ];
                    buildInputs = with pkgs; [
                        SDL2
                        SDL2_ttf
                    ];
                    buildPhase = ''
                        ${if pkgs.stdenv.isDarwin then "export CC=clang++" else "export CC=g++"}
                        make all
                    '';
                    installPhase = ''
                        mkdir -p $out/bin
                        mv play-gdg $out/bin
                        cp -r data $out/data
                    '';
                };
            };
        }
    );
}
