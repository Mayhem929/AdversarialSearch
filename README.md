# Adversarial Search (Games)
## My personal solution to the 3<sup>rd</sup> practice of *Artificial Intelligence*, 2022/2023

![Screenshot from 2023-07-19 10-44-31](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/90fc2ca7-8b1d-4341-a4bb-41453a2d0e60)


In this practice, we are asked to design and implement a deliberative agent capable of carrying out intelligent behavior within the modified rules of the game of **parchís**.

The game has been adapted by replacing the random behavior of rolling a die with the choice of the die among the available ones. Additionally, it is played one-on-one, and each player plays with 2 colors, which can attack each other. The player who manages to place all the pieces of any of their colors on the board first wins the game, regardless of the positions of the rest of their pieces. Futhermore, we had to take into account the objects we could find through the game, which could be used to hinder the other player's advance or boost your own pieces.

For the design of a solution, I have chosen to use the Alpha-Beta pruning since it allows us to reach a greater depth in the search tree than the MinMax algorithm. This is because with pruning, we leave unexplored paths that we know will not lead us to any favorable outcomes (according to the evaluation of our heuristic).

---

Once all the students submitted their solutions, the teachers organised a tournament in which all AIs would play against each other and would gather points. In the end, my AI got first place amongst all student and I got awarded some cool badges :)

![image](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/c928459d-e139-44b1-8866-3d590f1b7b8d=350x350)    ![image](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/be21bade-976f-440e-a455-9d23768c2716)    ![image](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/d1af9c36-1419-4b24-9d8e-6cd59d2879c7)

---

## Credits
The base sofware was made by the AI department in UGR. This is a fork from https://github.com/rbnuria/ParchisIA-23.
I, Adrián Jaén Fuentes, implemented the files AIPlayer.cpp and AIPlayer.hpp.
