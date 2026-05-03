# Pokemon Emerald Terminal

My professor is a Pokemon nerd. He is also quite insane. He has this yearly torture ritual for 2nd year IT students in his Data Structures & Algorithms class where he makes his inexperienced students code an entire game on the terminal. In ASCII, from scratch.

Naturally my first reaction was "this is just ridiculous, this is insane, I am never going to be able to do this" 17,000 lines later, it somehow worked out. It's messy, but somehow worked out. Constants are everywhere. Non-existent documentation, and if you accidentally duplicate a Pokemon in the database it crashes spectacularly. But it worked somehow! Got a 97/100 from my professor just from the sheer passion (but also burning rage) from this project.

Built it in 5 weeks. Learned more about state management and data organization in those weeks than the rest of the semester combined. You know the projects prior to this was a simple "make a mini library system!" or "haha make a mini student information system!", then there's this. "haha make the entire game in terminal in a month!".

The project has genuinely taught me a lot. It doesn't look flashy and future me will probably laugh at this, but this is one of those projects that I'll proudly look back to. Thank you Nescafe coffee sticks.

## Features include:

* Cursor-driven menus and text-based UI
* Full turn-based battle engine:

  * Type effectiveness and STAB
  * Critical hits and accuracy checks
  * Damage multipliers
* ASCII battle frames and animations
* Over 80 ASCII map layouts based on Pokémon Emerald locations
* NPCs and trainer battles inspired by the original game
* Story states and progression
* Mini ASCII cutscene animations
* Gen 3 Pokédex-style entries and Gen 3 moves

## Concepts Practiced (I think):

* Large-scale program organization
* State management (menus, exploration, battles, story)
* Game logic and rule systems
* Randomization and probability
* ASCII-based UI design
* Debugging and iteration over a long-term project

---

# How to Run

1. Compile using g++:
   g++ main.cpp -o pokemon
   
3. Run the output:
   ./pokemon
   (Linux/Wine)
   or
   pokemon.exe
   (Windows)
   
## Note: Built with Windows-based includes. System API such as <windows.h> & <conio.h> will break on standard bash. Ironic because I built this on an old Pentium linux device running via Wine. But amateurish me did not think about multiplatform compatibility and the project was presented via a windows-based device.

<img width="1135" height="636" alt="Pokemonss4" src="https://github.com/user-attachments/assets/03ba8a7b-b3be-4575-b600-01edab284284" />

<img width="467" height="458" alt="Pokemonss3" src="https://github.com/user-attachments/assets/c97cc60c-2be8-46fc-9e25-caa32a599a52" />

<img width="537" height="444" alt="POKEMONSS" src="https://github.com/user-attachments/assets/9277e70d-9642-4392-8e4c-32a1f1969015" />

<img width="389" height="386" alt="PokemonSS5" src="https://github.com/user-attachments/assets/0e5f8d1d-9a1e-4d04-8338-ec092ed77fc5" />

<img width="692" height="374" alt="pokemonss2" src="https://github.com/user-attachments/assets/314357c6-beba-4285-8a3e-fb9cdd9a64c3" />

<img width="461" height="457" alt="old_ale" src="https://github.com/user-attachments/assets/78cd7410-913b-40a0-81f8-9036c6d87698" />

<img width="336" height="141" alt="dialogue" src="https://github.com/user-attachments/assets/62ac3f13-f300-48db-890a-5792183bcfcb" />

<img width="326" height="55" alt="transition_msg" src="https://github.com/user-attachments/assets/b8656422-b67d-49e7-a2ef-93fc29b78716" />
