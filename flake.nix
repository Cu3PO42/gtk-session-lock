{
    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    };
    outputs = {nixpkgs, ...}: {
        packages.aarch64-linux.default = nixpkgs.legacyPackages.aarch64-linux.callPackage ./gtk-session-lock.nix {};
        packages.x86_64-linux.default = nixpkgs.legacyPackages.x86_64-linux.callPackage ./gtk-session-lock.nix {};
    };
}
