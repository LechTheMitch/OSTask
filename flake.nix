{
  description = "GTK development flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: 
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        # Set your GTK version here (e.g., gtk4 or gtk3)
        gtkVersion = pkgs.gtk4;

      in {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            gtkVersion
            meson
            ninja
            pkg-config
            gobject-introspection
            glib
          ];

          shellHook = ''
            echo "GTK development environment ready."
            export PKG_CONFIG_PATH="${gtkVersion.dev}/lib/pkgconfig"
          '';
        };
      });
}
