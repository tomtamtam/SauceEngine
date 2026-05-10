{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    # OpenGL
    libGL
    libGLU
    mesa

    # X11
    xorg.libX11
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXi
    xorg.libXext

    # Wayland (required by GLFW 3.4)
    wayland
    wayland-protocols
    libxkbcommon

    # Build tools
    pkg-config
    cmake
    ninja
    gcc
  ];

  shellHook = ''
    export PKG_CONFIG_PATH=$(pkg-config --variable pc_path pkg-config)
    export PKG_CONFIG_PATH=${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libGL}/lib/pkgconfig:${pkgs.xorg.libX11}/lib/pkgconfig:$PKG_CONFIG_PATH
    export LD_LIBRARY_PATH=${pkgs.libGL}/lib:${pkgs.wayland}/lib:$LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=${pkgs.libxkbcommon}/lib:${pkgs.libGL}/lib:${pkgs.wayland}/lib:$LD_LIBRARY_PATH
  '';
}
