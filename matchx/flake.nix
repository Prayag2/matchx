# How to enter this shell: 
# $ nix develop
# 
# To fix file path errors in neovim with LSP or VSCode
# $ cmake -B build && cd build
# $ bear -- make
# $ mv ./compile_commands.json ../compile_commands.json

{
    description = "x86_64 dev environment for Matchx, an REG-NMS inspired crypto matching engine.";
    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    };
    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
    in
    {
        devShells.${system}.default = pkgs.mkShell {
            buildInputs = with pkgs; [
                bear
                cmake
                gnumake
                doxygen
                plog
                redis-plus-plus
            ];

            shellHook = ''
              which zsh >/dev/null && exec zsh
            '';
        };
    };
}
