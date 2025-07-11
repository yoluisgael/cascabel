#pragma once

#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include "serpientes.h"

#define NUMERO "Numero"
#define WHILE "While"
#define BOOL "Bool"
#define RET "Return"
#define TIPO "Tipo"
#define NEGACION "Negacion"
#define Id "Id"
#define DOS_PUNTOS ":"
#define SIGNO "Signo"
#define ASTERISCO "Asterisco"
#define DIVISION "Division"
#define ASIGNACION "Asignacion"
#define COMPARACION "Comparacion"
#define PUNTO_COMA "PuntoComa"
#define PARENTESIS_ABRE "ParentesisAbre"
#define PARENTESIS_CIERRA "ParentesisCierra"
#define CORCHETE_ABRE "CorcheteAbre"
#define CORCHETE_CIERRA "CorcheteCierra"
#define LLAVE_ABRE "LlaveAbre"
#define LLAVE_CIERRA "LlaveCierra"
#define CHAR "char"
#define STRING "String"
#define DEF "def"
#define INCLUDE "Include"
#define COMA "Coma"
#define AND "And"
#define EXP "una expresion"
#define EOL "fin de linea"
#define TAB "tab"


struct Token {
    std::string lexeme;
    std::string type;
};

struct Nodo {
    std::string valor;
    std::vector<Nodo> ramas;
};

std::vector<Token> lexer(const std::string& expression);
bool S(),
LINEAS(),
LINEA(),
DEF_FUNCION(),
ARGUMENTOS(),
ARGUMENTO(),
VALOR(),
ASSIGN(),
E(),
T(),
F(),
FUNCION(),
LINEAS_FUN(),
LINEA_FUN(),
RETURN();


namespace MyApp
{
    void RenderUI();
};

