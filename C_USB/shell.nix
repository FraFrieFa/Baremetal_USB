{ nixpkgs ? import <nixpkgs> {} }:

nixpkgs.mkShell {

  buildInputs = with nixpkgs; [
    gcc-arm-embedded-11
  ];
}
