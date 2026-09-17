{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
  };

  outputs =
    inputs@{ ... }:
    let
      supportedSystems = [
        "x86_64-linux"
      ];
      forAllSystems = inputs.nixpkgs.lib.genAttrs supportedSystems;
      buildNixpkgs =
        system:
        import inputs.nixpkgs {
          inherit system;
          overlays = [ ];
        };
    in
    {
      devShells = forAllSystems (
        system:
        let
          pkgs = buildNixpkgs system;

          bazelisk-override = pkgs.bazelisk.overrideAttrs (old: {
            postInstall = (old.postInstall or "") + ''
              ln -s $out/bin/bazelisk $out/bin/bazel
            '';
          });
        in
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              bazelisk-override
              libtinfo
              clang-tools

              treefmt

              # Formatters:
              bazel-buildtools
              nixpkgs-fmt
              prettier
            ];
          };
        }
      );
    };
}
