CC=g++-9
ARGS=-std=c++2a

test: test.cpp lib/piece.o lib/game.o lib/piece_behavior.o lib/move_path.o
	$(CC) $^ -o $@ $(ARGS)

lib/piece.o : src/model/piece.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/game.o : src/model/game.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/piece_behavior.o : src/model/piece_behavior.cpp
	$(CC) $< -c -o $@ $(ARGS)

lib/move_path.o : src/model/move_path.cpp
	$(CC) $< -c -o $@ $(ARGS)

.PHONY : clean
clean :
	rm lib/*.o
	rm test
