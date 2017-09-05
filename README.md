# MAC0215
Repositório para códigos do projeto de pesquisa

## Códigos implementados

- API pdf extractor, em phyton com uso do pdfminer para obter o texto de
um arquivo pdf, além das posições de suas respectivas bounding boxes em relação às páginas
do arquivo. Essa API será utilizada na implementação da prova de conceito.

- Wrapper para utilizar o pdf extractor em código escrito em c++.

- Classe glyph, em c++, que armazena dados relativos aos glifos de um determinado atlas;

- Classe text, em python, utilizando a biblioteca freetype:
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
