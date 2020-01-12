CC=g++-9
ARGS=-std=c++2a
ARGSV=-std=c++2a -lsfml-graphics -lsfml-window -lsfml-system

game: main.cpp lib/piece.o lib/game.o lib/specialized_pieces.o lib/move_path.o \
	  lib/color.o lib/type.o lib/board.o lib/boardController.o lib/location.o
	$(CC) $^ -o $@ $(ARGSV)

lib/piece.o : src/model/piece.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/game.o : src/model/game.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/specialized_pieces.o : src/model/specialized_pieces.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/move_path.o : src/model/move_path.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/color.o : src/model/color.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/type.o : src/model/type.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/location.o : src/model/location.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/board.o : src/view/board.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/boardController.o : src/controller/boardController.cpp
	$(CC) $< -c -o $@ $(ARGS)

.PHONY : build test
test: test.cpp lib/piece.o lib/game.o lib/specialized_pieces.o lib/move_path.o \
	  lib/color.o lib/type.o
	$(CC) $^ -o $@ $(ARGS)

.PHONY : clean
clean :
	rm lib/*.o
	rm test
