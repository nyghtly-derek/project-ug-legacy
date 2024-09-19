# Project Ugly Goblin

## How to play!

1. Install Nix package manager
    - https://nixos.org/download/ 
2. Start the game
    - `nix --extra-experimental-features flakes --extra-experimental-features nix-command run github:nyghtly-derek/project-ug-legacy`

### Goal

Try to defeat both knights without losing any workers!

### Controls

- Click and drag to select
- Right-click to move & attack
- A + left-click for attack-move
- Move cursor to edge for map scrolling

## About this project
 
"In progress."

So far, the following features have been implemented:
- Map loading from text files
- Dynamic pathfinding for multiple units at a time 
- Various methods of control for units, including boxed selection
- Basic AI for enemies
- Camera scrolling across the map

## Development

- Install nix at https://nixos.org/download/
- Run with `nix run`
- Build with `nix build`
- To debug, just uncomment this line from the files that you care about `// #define PRINT_INFO`

## Contributions

- This is a personal project and no outside contributions are welcomed at this time.
