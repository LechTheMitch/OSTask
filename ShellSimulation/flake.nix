{
  description = "GTK development flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    oh-my-bash = {
      url = "github:ohmybash/oh-my-bash";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, flake-utils, ... }: 
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
            fish
            pango
            gtkVersion
            meson
            harfbuzz
            cairo
            ninja
            libadwaita
            pkg-config
            gobject-introspection
            glib
          ];

          shellHook = ''
            echo "GTK development environment ready."
            echo "✨ Welcome to your GTK dev shell"
            export PKG_CONFIG_PATH="${pkgs.gtk4.dev}/lib/pkgconfig:${pkgs.pango.dev}/lib/pkgconfig:${pkgs.glib.dev}/lib/pkgconfig:${pkgs.harfbuzz.dev}/lib/pkgconfig:${pkgs.cairo.dev}/lib/pkgconfig:${pkgs.gdk-pixbuf.dev}/lib/pkgconfig:${pkgs.vulkan-loader.dev}/lib/pkgconfig:${pkgs.graphene.dev}/lib/pkgconfig"
            if ! type fisher &>/dev/null; then
              echo "Installing fisher plugin manager..."
              curl -sL https://raw.githubusercontent.com/jorgebucaran/fisher/main/functions/fisher.fish | source && fisher install jorgebucaran/fisher
            fi
            fisher install IlanCosman/tide@v6
            fish
          '';
        };
      });
}
