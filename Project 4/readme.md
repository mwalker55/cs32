#Project 4
This is project 4 from Winter 2015 CS32 at UCLA.

##About
The purpose of this project was to create a digital steganography system that would allow one to hide certain messages in webpages.

To do this, I had to create a templated hashtable from scratch as well as implement the LZR compression algorithm in C++.

Note that Provided.h and Http.h were provided by Professor Carey Nachenberg.  These files include the portion of the project that acquired HTML code for us and did other handy calculations.

##Running
To run, simply build all files together.  A sample test is provided: it creates a long, random message then hides it in a given WebPage and attempts to then extract it.