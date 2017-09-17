# MAC0215
Repositório para códigos do projeto de pesquisa

## Update 17/09

- O código foi refatorado para que as funções relacionadas com transformada de distância possam ficar organizadas em uma classe. Assim, a classe distance_transform foi criada.
  
  Funções públicas:
  
  * chess_board: Calcula a transformada de distância da imagem usando distância chessboard. Essa implementação e utiliza algoritmo 8sed;
  * city_block: Calcula a transformada de distância da imagem usando distância city block. Essa implementação e utiliza algoritmo 4sed;
  * euclidean: Calcula a transformada de distância da imagem usando distância euclidiana. Essa implementação é trivial, calculando para cada ponto do glifo, a distância do ponto complementar mais próximo a esse ponto percorrendo todo o glifo. Com isso, o algoritmo fica altamente ineficiente;
  * faster_euclidean: Calcula a transformada de distância exata da imagem usando distância euclidiana. Essa implementação utiliza o algoritmo apresentado em \cite{meijster2002general}, calculando em duas fases a transformada. A primeira fase segue o algoritmo apresentado por Meijester, porém a segunda fase, que envolve a minimização da função obtida na primeira fase é feita de modo trivial, obtendo um algoritmo com uma ordem de grandeza superior à do artigo, porém mais rápido que a implementação trivial;
  * get_transform_element: Getter para uma posição da transformada;
  * get_metric: Getter da métrica utilizada na transformada;
  * get_height: Getter da altura da transformada;
  * get_width: Getter da largura da transformada.
  
  Funções privadas:
  
  * closest_raster: Realiza a varredura em raster para o algorimos 4sed e 8sed;
  * closest_anti_raster: Realiza a varredura em anti-raster para o algorimos 4sed e 8sed;
  * closest: Procura o ponto complementar mais próximo de um dado ponto;
  * euclidean_distance: Calcula a transformada de distância de um ponto do glifo de forma trivial;
  * one_color_run:} Calcula a transformada 4sed ou 8sed para uma cor da imagem;
  * revert: Inverte a cor do glifo;
  * join_binary_transform: Recebe duas transformadas sem sinal e gera uma transformada com sinal;
  * meijester_phase_1: Realiza a primeira fase do calculo da transformada de distância exata com métrica euclidiana;
  * meijester_phase_2: Realiza a segunda fase do calculo da transformada de distância exata com métrica euclidiana;
  * transpose: Realiza transpõe de uma imagem.

- A função update, em glwidget, foi alterada. Agora, ao invés de mover a câmera para gerar efeito de zoom, o ângulo vertical da matriz de perspectiva é alterado a partir de um coeficiente de escala;

- As funções utilizadas para a geração de atlas foram refatoradas e a função privada \textbf{bake} foi criada para ser chamada por bake_atlas, bake_dist_transf e bake_mip_atlas pois essas tinham um corpo muito parecido;
  
- As funções construct_image e prepare_texture foram criadas para modularizar a função bake, pois a função estava muito comprida.


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

