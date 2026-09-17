# Midnight Sun Car #17 Firmware:

This repository contains all firmware for the [University of Waterloo](https://uwaterloo.ca/)'s [Midnight Sun Solar Rayce Car](http://www.uwmidsun.com/) team's car #24.

## Getting Started:
**Currently, we only support building & developing on `x86_64` Ubuntu Linux and Ubuntu Linux systems.**

1. Install [Nix](https://nixos.org/) using the [Determinate Systems Nix Installer](https://determinate.systems/install/).
2. Run `nix develop` in the repository root.
3. Run `bazel build //...`.

### Convenience:
Install [Direnv](https://direnv.net/), and run `direnv allow` in the repository root.