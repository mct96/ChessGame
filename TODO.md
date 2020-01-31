# TODO List
* Refatorar o tabuleiro para obter os estados: Cada posição deve ser definida
por "Tipo da Peça" e "Cor da Peça".
* Mover os métodos de "Pruning" e "Exceptional Moves" para a classe CPiece.
* Não é necessário diversas instancias das peças (para cada cor e 8 × Peões,
 2 × das demais), apenas um e o estado (cor + posição) é passado quando a peça
 for selecinada.
* Salvar o estado do jogo.