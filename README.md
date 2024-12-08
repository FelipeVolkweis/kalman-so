# Introdução 
Este projeto é um leitor de Logs dos jogos da Robocup na modalidade SSL.
O objetivo é colocar em prática conceitos aprendidos na disciplina de Sistemas Operacionais I - SSC0140

# Pré requisitos
As dependências do projeto são as seguintes:
1.	Instalar 'protobuf' para desenvolvedores
2.	Instalar 'Qt5' para desenvolvedores
3.  Instalar 'cmake'
4.  Instalar 'gcc' ou outro compilador de C++
5.  Instalar 'eigen3'

# Build e Testes
Para compilar o projeto é necessário:

1.	Rodar 'sh protoc.sh' no diretório base, isso irá gerar os arquivos .pb provenientes dos protobufs definidos pela Robocup
2.  Crie um diretório chamado 'build/'
3.  Rode o cmake no diretório 'build/'
4.  Dentro do diretório build, rode o comando 'make'
5.  Rode o binário agregado de um nome de arquivo a ser lido.

# No Fedora 40
No Linux Fedora 40, o procedimento descrito acima ficaria assim (partindo o diretório base):

> sudo dnf install -y cmake gcc-c++ qt5-*-devel protobuf-devel freeglut-devel <br/>
> mkdir build && cd build <br/>
> cmake .. <br/>
> make <br/>
> ./SSLLogReader log-exemplo.log