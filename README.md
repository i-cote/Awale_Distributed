# Awale Distributed
This repository hosts the source code for a distributed implementation
of the Awale game written in C.
<br>The game uses a client server architecture which enables players to play over the network.
<br> The following functionalities are available : 
- A player launches the client and connects to the server using a username and a password.
- Once connected they can see a list of all the other players present on the server.
- They can then choose to either watch an ongoing game or start a new one by defying a player in the lobby.
- The defied player can either deny or accept the invitation to play,
in the latter case a new game is started.
- At any point any player can choose to disconnect, when comming back they will have to provide the password they chose during their first connection.<br> The server also ensures that if a given player is connected, then no other connection using its username is possible.
- If a user disconnects while playing a game, then the game will be paused until the user reconnects.

## Building and running the binaries

⚠️ The project depends on ncurses, make sure it is installed on your system. 
<br>Normally this shouldn't be a problem, ncurses comes by default with every linux distribution.

Run make to build the client and server binaries <br>
```
make 
```
You will now find the binaries `client` and `server` in `build/bin`

Launch the server :
```
./build/bin/server
```
There's no need to specify anything, the server listens on port 8080

Launch several clients :
```
./build/bin/client <server_ip> 8080
```

## Logging into the server with a username and a password
After having launched the client you are prompted to provide a login and a password.
<br>Just start typing to enter the login and press tab to switch to the password field.
<br>After having typed both the login and the password, press enter to connect.

## "Players in game" and "Players in lobby" windows
Now that your are connected to the server you can see two windows :
- The "Players in lobby" window.
<br>in this window you will see the list of players that are waiting in the lobby.
- The "Players in game" window.
<br>in this window you will see the list of players that are currently playing.

<br>You can switch which window is in focus (green borders) using
the left and right arrow keys.

## Sending an invitation to another player
Place the focus on the "Players in lobby" window and select one of the listed players using the up/down arrow keys, then press enter to send the invitation. 

** the invited player presses 'y' to accept the invitation **

You are now presented with the board view, when its your turn to play, your name is highlited, you can then select the hole you want to play using the left and right keys and enter to make your move.

## Disconnecting then reconnecting in the middle of a game
While in the middle of a game, press ctrl+C.
This will shutdown the client and disconnect you from the server.
<br>Relaunch the client :
```
./build/bin/client <server_ip> 8080
```

You'll lend on the logging screen and you will have to input your username along with the password you had chosen when logging in for the first time.

After having successfully logged in, you can resume the game where you left off.

## Forfeiting
While playing the game you can choose to forfeit by pressing the 'f' key.
<br>You'll then be prompted for confirmation and if you do confirm, <br>you'll lend back in the lobby with the "Players in game" and "Players in lobby" windows view.

## Watching a game
While in the lobby you can also choose to watch an ongoing game.
<br>To do so you must switch to the "Players in game" window using the left/right arrow keys then you must select one of the players and press enter to watch the game of the selected player.  
