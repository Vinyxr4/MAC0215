# MAC0215
Repositório para códigos do projeto de pesquisa

## Update 10/09

- Classe text
  * bake_mip_atlas constrói uma textura com uma quantidade de resoluções designada pelo usuário, gerando então um MIPmap;
  
  * A função bake_dist_transf constrói um atlas de glifos usando transformadas de distância, com as possíveis métricas:
    
    ** Distância euclidiana na transformada de distância utilizando um algoritmo trivial. Esse algoritmo se mostrou muito ineficiente, sendo extremamente lento para a criação de um atlas.
    
    ** Distância chessboard, utilizando um algoritmo linear no tamanho do glifo. Utiliza um algoritmo ensinado na disciplina MAC0417-2017, oferecida pelo professor Roberto Hirata. Foi implementado com 8-conectividade.
    
    ** Para implementar a distância city-block, basta alterar a conectividade no algoritmo anterior;
    
    ** Dada a ineficiência do algoritmo que usa distância euclidiana, resolvi procurar métodos melhores. Pela minha pesquisa, existe um modo de realizar de forma eficiente a transformada usando diagrama de Voronoi. Lerei os artigos que explicam a ideia, então farei a implementação otimizada.

- Renderização
  * A utilização do slider lateral como zoom ainda está mantida, mas alterei a função resizeGL para utilizar um parâmetro que pode ser variado como ângulo vertical na matriz de perspectiva. Assim, futuramente o slider estará conectado a esse parâmetro, fazendo com que a câmera não se mova, mas sim seja alterado o ângulo de visão para gerar efeito de zoom;

## Códigos implementados
- API pdf extractor, em phyton com uso do pdfminer para obter o texto de
um arquivo pdf, além das posições de suas respectivas bounding boxes em relação às páginas
do arquivo. Essa API será utilizada na implementação da prova de conceito.

- Wrapper para utilizar o pdf extractor em código escrito em c++.

- Classe glyph, em c++, que armazena dados relativos aos glifos de um determinado atlas;

- Classe text, em c++, utilizando a biblioteca freetype:
  * bake atlas cria um atlas de glifos, salvo em uma imagem a ser utilizada como textura
    para renderização de texto.
  
  * A função define text recebe uma string de texto e as posições em que o texto deve
    aparecer, configurando a renderização.
  
  * define text from pdf é um wrapper de define text que recebe o caminho de um arquivo
    pdf, extrai o texto e posição dos caracteres, com o uso da API pdf extractor, e os
    passa como argumentos de define text.
  
  * define font type recebe o caminho de uma fonte de texto e a define como a fonte a ser
    utilizada.
  
  * gen test e gen test pdf são funções que geram exemplos simples.

- Para a renderização, está sendo utilizado um código template disponibilizado no oferecimento
de MAC0420-2017, com algumas modificações. Os componentes criados por mim são:
  * A função LoadText usa a classe text para gerar o objeto a ser carregado na pipeline
    gráfica.
  
  * O shader texture usa a textura passada para definir o valor alpha de cada pixel. Assim,
    pixels mapeados a regiões da textura em que existe algum glifo ficam visíveis.

