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
                        makeWrapper
                        clang
                    ];
                    buildInputs = with pkgs; [
                        SDL2
                        SDL2_ttf
                    ]; 
                    buildPhase = ''
                        export CC=clang++
                        make all
                    '';
                    installPhase = ''
                        mkdir -p $out/bin
                        mv play-gdg $out/bin
                        cp -r data $out/data
                        wrapProgram $out/bin/play-gdg --chdir $out
                    '';
                };
            };
        }
    );
}
