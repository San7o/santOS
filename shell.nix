let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-24.05";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {

  hardeningDisable = [ "all" ]; # Disable all hardening flags

  packages = with pkgs; [
      gnumake
      grub2
      libisoburn        # xorriso
  ];
}
