# Adversarial Search (Games)
## My personal solution to the 3<sup>rd</sup> practice of *Artificial Intelligence*, 2022/2023

![Screenshot from 2023-07-19 10-44-31](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/76cb3270-fa78-485f-b0db-6965ba59984c)


In this practice, we are asked to design and implement a deliberative agent capable of carrying out intelligent behavior within the modified rules of the game of **parchís**.

The game has been adapted by replacing the random behavior of rolling a die with the choice of the die among the available ones. Additionally, it is played one-on-one, and each player plays with 2 colors, which can attack each other. The player who manages to place all the pieces of any of their colors on the board first wins the game, regardless of the positions of the rest of their pieces. Futhermore, we had to take into account the objects we could find through the game, which could be used to hinder the other player's advance or boost your own pieces.

For the design of a solution, I have chosen to use the Alpha-Beta pruning since it allows us to reach a greater depth in the search tree than the MinMax algorithm. This is because with pruning, we leave unexplored paths that we know will not lead us to any favorable outcomes (according to the evaluation of our heuristic).

---

Once all the students submitted their solutions, the teachers organised a tournament in which all AIs would play against each other and would gather points. In the end, my AI got first place among all the students and I got some cool badges :)

![LEADERBOARD_P3_-_TOP_1_GENERAL](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/4a684645-8101-4810-8b14-cdc8fcfd3853)
![LEADERBOARD_P3_-_TOP_1_GRUPO](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/2b120f67-df8c-4c87-b44c-9dd45259972b)
![LEADERBOARD_P3_-_TOP_1_SUBGRUPO](https://github.com/Mayhem929/AdversarialSearch/assets/76180710/275e8e58-e081-45f5-8e4e-a57c9a3f8151)

[made by panetone](https://github.com/PanetoneTryhard)

---

## Credits
The base sofware was made by the AI department in UGR. This is a fork from https://github.com/rbnuria/ParchisIA-23.
I, Adrián Jaén Fuentes, implemented the files AIPlayer.cpp and AIPlayer.hpp.
