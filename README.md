# About

This repo contains a program to solve and plot the snake cube puzzle

<img src="doc/flat.jpg" width="300"> <img src="doc/partially_solved.jpg" width="300">

It does so by first setting up a legal flat cube:

and examining all possible rotations by brute force. As it turns out there is not one, but two possible solutions:

Have a nice day.

# Build

Plotting is done via [matplot++](https://github.com/alandefreitas/matplotplusplus). Note that `matplot++` has certain system dependencies (e.g. `gnuplot`) that are required at runtime.

# Disclaimer

This project was an excuse for me to implement and learn more about quaternions.