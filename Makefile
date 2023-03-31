all: dirs bin/Parchis

obj/main.o: src/main.cpp
	g++  -g -c src/main.cpp -o obj/main.o -I include/ -std=c++14

obj/ParchisGUI.o: src/ParchisGUI.cpp include/ParchisGUI.h
	g++  -g -c src/ParchisGUI.cpp -o obj/ParchisGUI.o -I include/ -std=c++14

obj/Parchis.o: src/Parchis.cpp include/Parchis.h
	g++  -g -c src/Parchis.cpp -o obj/Parchis.o -I include/ -std=c++14

obj/Board.o: src/Board.cpp include/Board.h
	g++  -g -c src/Board.cpp -o obj/Board.o -I include/ -std=c++14

obj/Dice.o: src/Dice.cpp include/Dice.h
	g++  -g -c src/Dice.cpp -o obj/Dice.o -I include/ -std=c++14

obj/Piece.o: src/Piece.cpp include/Piece.h
	g++  -g -c src/Piece.cpp -o obj/Piece.o -I include/ -std=c++14

obj/BoardTrap.o: src/BoardTrap.cpp include/BoardTrap.h
	g++  -g -c src/BoardTrap.cpp -o obj/BoardTrap.o -I include/ -std=c++14

obj/ClickableSprite.o: src/ClickableSprite.cpp include/ClickableSprite.h
	g++  -g -c src/ClickableSprite.cpp -o obj/ClickableSprite.o -I include/ -std=c++14

obj/BoardSprite.o: src/BoardSprite.cpp include/BoardSprite.h
	g++  -g -c src/BoardSprite.cpp -o obj/BoardSprite.o -I include/ -std=c++14

obj/DiceSprite.o: src/DiceSprite.cpp include/DiceSprite.h
	g++  -g -c src/DiceSprite.cpp -o obj/DiceSprite.o -I include/ -std=c++14

obj/PieceSprite.o: src/PieceSprite.cpp include/PieceSprite.h
	g++  -g -c src/PieceSprite.cpp -o obj/PieceSprite.o -I include/ -std=c++14

obj/SpecialItemSprite.o: src/SpecialItemSprite.cpp include/SpecialItemSprite.h
	g++  -g -c src/SpecialItemSprite.cpp -o obj/SpecialItemSprite.o -I include/ -std=c++14

obj/BoardTrapSprite.o: src/BoardTrapSprite.cpp include/BoardTrapSprite.h
	g++  -g -c src/BoardTrapSprite.cpp -o obj/BoardTrapSprite.o -I include/ -std=c++14

obj/ExplosionSprite.o: src/ExplosionSprite.cpp include/ExplosionSprite.h
	g++  -g -c src/ExplosionSprite.cpp -o obj/ExplosionSprite.o -I include/ -std=c++14

obj/Player.o: src/Player.cpp include/Player.h
	g++  -g -c src/Player.cpp -o obj/Player.o -I include/ -std=c++14

obj/AIPlayer.o: src/AIPlayer.cpp include/AIPlayer.h
	g++  -g -c src/AIPlayer.cpp -o obj/AIPlayer.o -I include/ -std=c++14

obj/Ninja.o: src/Ninja.cpp include/Ninja.h
	g++  -g -c src/Ninja.cpp -o obj/Ninja.o -I include/ -std=c++14

obj/GUIPlayer.o: src/GUIPlayer.cpp include/GUIPlayer.h
	g++  -g -c src/GUIPlayer.cpp -o obj/GUIPlayer.o -I include/ -std=c++14

obj/RemotePlayer.o: src/RemotePlayer.cpp include/RemotePlayer.h
	g++  -g -c src/RemotePlayer.cpp -o obj/RemotePlayer.o -I include/ -std=c++14

obj/SpriteAnimator.o: src/SpriteAnimator.cpp include/SpriteAnimator.h
	g++  -g -c src/SpriteAnimator.cpp -o obj/SpriteAnimator.o -I include/ -std=c++14

obj/Button.o: src/Button.cpp include/Button.h
	g++  -g -c src/Button.cpp -o obj/Button.o -I include/ -std=c++14

obj/Connection.o: src/Connection.cpp include/Connection.h
	g++  -g -c src/Connection.cpp -o obj/Connection.o -I include/ -std=c++14

obj/GameSelector.o: src/GameSelector.cpp include/GameSelector.h
	g++  -g -c src/GameSelector.cpp -o obj/GameSelector.o -I include/ -std=c++14

bin/Parchis: obj/main.o obj/ParchisGUI.o obj/Parchis.o obj/Board.o obj/Dice.o obj/Piece.o obj/BoardTrap.o obj/SpriteAnimator.o obj/DiceSprite.o obj/PieceSprite.o obj/SpecialItemSprite.o obj/BoardTrapSprite.o obj/ExplosionSprite.o obj/BoardSprite.o obj/Button.o obj/ClickableSprite.o obj/RemotePlayer.o obj/GUIPlayer.o obj/Ninja.o obj/AIPlayer.o obj/Player.o obj/Connection.o obj/GameSelector.o
	g++  -g obj/main.o obj/ParchisGUI.o obj/Parchis.o obj/Board.o obj/Dice.o obj/Piece.o obj/BoardTrap.o obj/DiceSprite.o obj/SpriteAnimator.o obj/PieceSprite.o obj/SpecialItemSprite.o obj/BoardTrapSprite.o obj/ExplosionSprite.o obj/BoardSprite.o obj/Button.o obj/ClickableSprite.o obj/RemotePlayer.o obj/GUIPlayer.o obj/Ninja.o obj/AIPlayer.o obj/Player.o obj/Connection.o obj/GameSelector.o -o bin/Parchis -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lpthread

clean:
	rm -rf ./obj/*.o ./bin/*

dirs:
	mkdir -p obj
	mkdir -p bin

.PHONY: clean dirs