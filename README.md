# Buzz Bombers Plus

Buzz Bombers Plus is a C++ game built using SFML. This repository contains the source code and instructions on how to set up, compile, and run the game on Ubuntu.

## Cloning the Repository

To clone the repository, run the following command in your terminal:

```sh
 git clone https://github.com/hamza-sultann/Buzz-Bombers-Plus.git
```

Then navigate into the project directory:

```sh
 cd Buzz-Bombers-Plus
```

## Installing SFML

Before compiling the project, you need to install the SFML library. Use the following command to install it:

```sh
 sudo apt update
 sudo apt install libsfml-dev
```

## Compiling the Code

To compile the code, use the following command:

```sh
 g++ buzz.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

## Running the Application

After compiling, run the game using:

```sh
 ./sfml-app
```

## Game Display
Here is what the game looks like

![Buzz Bombers Plus](https://github.com/hamza-sultann/Buzz-Bombers-Plus/blob/main/Images/image.png)

## Contributions

Contributions are welcome! Feel free to submit pull requests to enhance the game.

## License

This project is open-source. Feel free to modify and use it under the specified license in the repository.

---

Enjoy playing Buzz Bombers Plus!

