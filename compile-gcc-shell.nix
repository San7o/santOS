let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {

  hardeningDisable = [ "all" ]; # Disable all hardening flags

  packages = with pkgs; [
      libgcc
      bison
      gnumake
      flex
      gmp
      mpfr
      libmpc
      texinfo
      isl
  ];
}
