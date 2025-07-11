#include "Application.h"
#include "imgui.h"
#include "imgui_stdlib.h"

//Código base
std::string inputText = "";
std::string outputText = "";
std::string lexico = "";
std::string sintactico = "Bienvenidos a Cascabel";
std::string semantico = BIENVENIDA;
int pos;
bool correcto = false;
bool semantic = true;
bool leyendoDec = false;
bool leyendoFun = false;
int argumentos = 0;
std::vector<Token> tokens;
std::map<std::string, std::string> simbolos;
std::map<std::string, int> funciones;
std::set<std::string> arguments;


namespace MyApp
{
    bool compilar()
    {

        std::map<std::string, std::string> x;
        std::map<std::string, int> fun;
        fun["print"] = 99;
        fun["input"] = 0;
        simbolos = x;
        funciones = fun;
        correcto = true;
        semantic = true;
        tokens = lexer(inputText);
        for (const auto& token : tokens) {
            lexico += token.lexeme;
            lexico += "\n";
            lexico += token.type;
            lexico += "\n";
        }
        pos = 0;
        if (correcto) {
            S();
            if (correcto) {
                sintactico += "Sintaxis correcta\n";
                sintactico += BRUJA;
                if (semantic) {
                    semantico += "Sema correcta\n";
                    semantico += FELIZ;
                }
                else {
                    semantico += ENOJADA;
                }
            }
            else {
                correcto = false;
                sintactico += "Sintaxis incorrecta\n";
                sintactico += DETECTIVE;
                semantico = TRISTE;
            }
        }
        else {
            sintactico = ENOJADA;
            semantico = SORPRENDIDA;
        }
        return correcto;
    }

    void correr()
    {
        outputText = inputText;
        outputText += R"(final = """Presione Enter para continuar...
              ____
  _          / . .\\
 (_)         \  u /
(__)          \  /
/ /___________/ /
\______________/"""
print(final)
end = int(input());)";
        std::ofstream MyFile("C:\\Users\\Public\\m.py");
        MyFile << outputText;
        MyFile.close();
        system("python C:\\Users\\Public\\m.py");
    }

    void RenderUI()
    {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


        ImGuiTabBarFlags tab_flags = ImGuiTabBarFlags_None;
        ImGuiWindowFlags codigo_flags
            = ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoNav
            | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse;

        ImGuiTableFlags table_flags = 0
            | ImGuiTableFlags_BordersInnerH
            | ImGuiTableFlags_BordersInnerV
            | ImGuiTableFlags_RowBg;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::Button("Compilar"))
                {
                    lexico = "";
                    semantico = "";
                    sintactico = "";
                    if (inputText == "") {
                        sintactico += "Codigo vacio\n";
                        sintactico += SORPRENDIDA;
                        semantico += TRISTE;
                        correcto = false;
                    }
                    else {
                        correcto = compilar();
                    }
                };

                if (ImGui::Button("Correr"))
                {
                    if (correcto) {
                        correr();
                    }
                    else {
                        sintactico = "";
                        sintactico += "Se debe compilar exitosamente\n";
                    }
                };

                if (ImGui::Button("Cargar")) {
                    inputText = "";
                    std::ifstream codigo("codigo_ejemplo/ejemplo.txt");
                    std::string linea = "";
                    while (codigo.good()) {
                        getline(codigo, linea);
                        inputText += linea;
                        inputText += "\n";
                    }
                    codigo.close();
                };
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::End();



        //Cuadro de texto, código
        ImGui::Begin("Codigo", nullptr, codigo_flags);
        if (ImGui::InputTextMultiline("Codigo", &inputText, ImVec2(-1.0f, -1.0f), ImGuiInputTextFlags_AllowTabInput)) correcto = false;
        ImGui::End();


        //Cuadro de Mensajes
        ImGui::Begin("Lexico");
        //Tabla
        ImGui::BeginTable("Analisis Lexico", 2, table_flags, ImVec2(-1.0f, -1.0f));
        ImGui::TableSetupColumn("Lexema");
        ImGui::TableSetupColumn("Tipo");
        ImGui::TableHeadersRow();
        std::stringstream ss(lexico);
        std::string lex;
        bool row = 0;
        while (getline(ss, lex, '\n'))
        {
            if (row % 2 == 0) ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(row % 2);
            ImGui::TextWrapped(lex.c_str());
            row = !row;
        }
        ImGui::EndTable();
        ImGui::End();


        //Cuadro de Mensajes
        ImGui::Begin("Sintactico");
        ImGui::TextWrapped(sintactico.c_str());
        ImGui::End();

        //Cuadro de Mensajes
        ImGui::Begin("Semantico");
        ImGui::TextWrapped(semantico.c_str());
        ImGui::End();
    }
}

void error(std::string token) {
    sintactico += "Se espera ";
    sintactico += token;
    sintactico += " y se tiene ";
    sintactico += tokens[pos].lexeme;
    sintactico += "\n";
}

std::vector<Token> lexer(const std::string& expression) {
    std::vector<Token> tokens;
    std::regex tokenPattern(R"(([+-]?\d+(\.\d+)?)|([a-zA-Z_][a-zA-Z0-9_]*)|('[^'\\]*(\\.[^'\\]*)*')|("[^"\\]*(\\.[^"\\]*)*")|(!=|==|!|\|\||&&)|(-=|\+=|\*=|/=|%=|=)|(<=|>=|<|>|==|!=)|(def)|(\/\/[^\n]*)|(true|false)|\S)");
    std::regex numeroPatron(R"([+-]?\d+(\.\d+)?)");
    std::regex variablePatron(R"([a-zA-Z_][a-zA-Z0-9_]*)");
    std::regex signoPatron(R"([+-])");
    std::regex asteriscoPatron(R"(\*)");
    std::regex dospuntosPatron(R"(:)");
    std::regex divisorPatron(R"(/)");
    std::regex asignarPatron(R"(-=|\+=|\*=|/=|%=|=)");
    std::regex notPatron(R"(!)");
    std::regex compararLogicoPatron(R"(!=|==|\|\||&&)");
    std::regex menorIgualPatron(R"(<=)");
    std::regex mayorIgualPatron(R"(>=)");
    std::regex menorPatron(R"(<)");
    std::regex mayorPatron(R"(>)");
    std::regex igualPatron(R"(==)");
    std::regex distintoPatron(R"(!=)");
    std::regex puntocomaPatron(R"(;)");
    std::regex tipodatoPatron(R"(int|char|float|string|double|bool|void)");
    std::regex abreparentesisPatron(R"(\()");
    std::regex cierraparentesisPatron(R"(\))");
    std::regex abrecorchetePatron(R"(\[)");
    std::regex cierracorchetePatron(R"(\])");
    std::regex abrellavePatron(R"(\{)");
    std::regex cierrallavePatron(R"(\})");
    std::regex charPatron(R"('([^'\\]*(\\.[^'\\]*)*)')");
    std::regex stringPatron(R"("([^"\\]*(\\.[^"\\]*)*)\")");
    std::regex definePatron(R"(def)");
    std::regex unknownPatron(R"(\S)");
    std::regex returnPatron(R"(return\b)");
    std::regex commentPatron(R"(\/\/[^\n]*)");
    std::regex comaPatron(R"(,)");
    std::regex andPatron(R"(&)");
    std::regex whilePatron(R"(while)");
    std::regex boolPatron(R"(true|false)");

    auto token_iterator = std::sregex_iterator(expression.begin(), expression.end(), tokenPattern);

    for (auto it = token_iterator; it != std::sregex_iterator(); ++it) {
        std::string lexeme = it->str();
        std::string type;

        if (regex_match(lexeme, commentPatron)) {
            continue;
        }
        if (regex_match(lexeme, numeroPatron)) {
            type = NUMERO;
        }
        else if (regex_match(lexeme, whilePatron)) {
            type = WHILE;
        }
        else if (regex_match(lexeme, dospuntosPatron)) {
            type = DOS_PUNTOS;
        }
        else if (regex_match(lexeme, definePatron)) {
            type = DEF;
        }
        else if (regex_match(lexeme, boolPatron)) {
            type = BOOL;
        }
        else if (regex_match(lexeme, returnPatron)) {
            type = RET;
        }
        else if (regex_match(lexeme, tipodatoPatron)) {
            type = TIPO;
        }
        else if (regex_match(lexeme, notPatron)) {
            type = NEGACION;
        }
        else if (regex_match(lexeme, variablePatron)) {
            type = Id;
        }
        else if (regex_match(lexeme, signoPatron)) {
            type = SIGNO;
        }
        else if (regex_match(lexeme, asteriscoPatron)) {
            type = ASTERISCO;
        }
        else if (regex_match(lexeme, divisorPatron)) {
            type = DIVISION;
        }
        else if (regex_match(lexeme, asignarPatron)) {
            type = ASIGNACION;
        }
        else if (regex_match(lexeme, compararLogicoPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, menorIgualPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, mayorIgualPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, menorPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, mayorPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, igualPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, distintoPatron)) {
            type = COMPARACION;
        }
        else if (regex_match(lexeme, puntocomaPatron)) {
            type = PUNTO_COMA;
        }
        else if (regex_match(lexeme, abreparentesisPatron)) {
            type = PARENTESIS_ABRE;
        }
        else if (regex_match(lexeme, cierraparentesisPatron)) {
            type = PARENTESIS_CIERRA;
        }
        else if (regex_match(lexeme, abrecorchetePatron)) {
            type = CORCHETE_ABRE;
        }
        else if (regex_match(lexeme, cierracorchetePatron)) {
            type = CORCHETE_CIERRA;
        }
        else if (regex_match(lexeme, abrellavePatron)) {
            type = LLAVE_ABRE;
        }
        else if (regex_match(lexeme, cierrallavePatron)) {
            type = LLAVE_CIERRA;
        }
        else if (regex_match(lexeme, charPatron)) {
            type = CHAR;
        }
        else if (regex_match(lexeme, stringPatron)) {
            type = STRING;
        }
        else if (regex_match(lexeme, comaPatron)) {
            type = COMA;
        }
        else if (regex_match(lexeme, andPatron)) {
            type = AND;
        }
        else {
            type = "Desconocido";
            correcto = false;
        }

        tokens.push_back({ lexeme, type });
    }

    return tokens;
}

bool S() {
    if (LINEA()) {
        if (tokens[pos].type == PUNTO_COMA) {
            pos++;
            if (LINEAS()) {
                return pos == tokens.size();
            }
        }
        else {
            sintactico += "Se espera ;\n";
            correcto = false;
            return false;
        }
    }
    return false;
}

bool LINEAS() {
    if (LINEA()) {
        if (tokens[pos].type == PUNTO_COMA) {
            pos++;
            if (LINEAS()) {
                return true;
            }
        }
        else {
            sintactico += "Se espera ;\n";
            correcto = false;
            return false;
        }
    }
    return true;
}

bool LINEA() {
    if (ASSIGN()) {
        return true;
    }
    if (FUNCION()) {
        return true;
    }
    if (DEF_FUNCION()) {
        return true;
    }
    if (E()) {
        return true;
    }
    return false;
}

bool DEF_FUNCION() {
    std::string id;
    leyendoDec = true;
    int funargs;
    if (tokens[pos].type == DEF) {
        pos++;
        if (tokens[pos].type == Id) {
            id = tokens[pos].lexeme;
            pos++;
            if (tokens[pos].type == PARENTESIS_ABRE) {
                pos++;
                if (ARGUMENTOS()) {
                    funargs = argumentos;
                    leyendoDec = false;
                    leyendoFun = true;
                    if (tokens[pos].type == PARENTESIS_CIERRA) {
                        pos++;
                        if (tokens[pos].type == DOS_PUNTOS) {
                            pos++;
                            if (LINEA_FUN()) {
                                if (tokens[pos].type == PUNTO_COMA) {
                                    pos++;
                                    if (LINEAS_FUN()) {
                                        if (RETURN()) {
                                            leyendoFun = false;
                                            if (tokens[pos].type == PUNTO_COMA) {
                                                if (simbolos.find(id) != simbolos.end()) {
                                                    semantico += "Variable ";
                                                    semantico += id;
                                                    semantico += " no puede ser asignada como funcion\n";
                                                    semantic = false;
                                                }
                                                else if (funciones.find(id) != funciones.end()) {
                                                    semantico += "Funcion ";
                                                    semantico += id;
                                                    semantico += " fue definida previamente\n";
                                                    semantic = false;
                                                }
                                                else {
                                                    funciones[id] = funargs;
                                                }
                                                argumentos = 0;
                                                return true;
                                            }
                                            else {
                                                sintactico += "Se espera ; despues de return\n";
                                                correcto = false;
                                            }
                                        }
                                        else {
                                            sintactico += "Se espera return al final de la funcion\n";
                                            correcto = false;
                                        }
                                    }
                                }
                                else {
                                    sintactico += "Se espera ; dentro de la funcion\n";
                                    correcto = false;
                                }
                            }
                            else {
                                sintactico += "Funcion no puede estar vacia\n";
                                correcto = false;
                            }
                        }
                        else {
                            sintactico += "Debe tener dos puntos despues de los parentesis\n";
                            correcto = false;
                        }
                    }
                    else {
                        sintactico += "Debe tener parentesis ) despues del id\n";
                        correcto = false;
                    }
                }
            }
            else {
                sintactico += "Debe tener parentesis ( despues del id\n";
                correcto = false;
            }
        }
        else {
            sintactico += "Debe tener id la funcion\n";
            correcto = false;
        }
    }
    return false;
}

bool ARGUMENTOS() {
    if (ARGUMENTO()) {
        return true;
    }
    return true;
}

bool ARGUMENTO() {
    if (VALOR()) {
        argumentos++;
        if (tokens[pos].type == COMA) {
            pos++;
            if (ARGUMENTO()) {
                return true;
            }
        }
        return true;
    }
    return false;
}

bool VALOR() {
    if (E()) {
        return true;
    }
    if (tokens[pos].type == STRING) {
        pos++;
        return true;
    }
    return false;
}

bool ASSIGN() {
    std::string id;
    std::string tipo;
    if (tokens[pos].type == Id) {
        id = tokens[pos].lexeme;
        pos++;
        if (tokens[pos].type == ASIGNACION) {
            pos++;
            if (tokens[pos].type == TIPO) {
                tipo = tokens[pos].lexeme;
                pos++;
                if (tokens[pos].type == PARENTESIS_ABRE) {
                    pos++;
                    if (E()) {
                        if (tokens[pos].type == PARENTESIS_CIERRA) {
                            pos++;
                            if (leyendoFun) {
                                arguments.insert(id);
                            }
                            else if (funciones.find(id) != funciones.end()) {
                                semantico += "Funcion ";
                                semantico += id;
                                semantico += " no puede ser asignada como variable\n";
                                semantic = false;
                            }
                            else {
                                simbolos[id] = tipo;
                            }
                            return true;
                        }
                        else {
                            sintactico += "Se espera un parentesis )\n";
                            correcto = false;
                        }
                    }
                    else {
                        sintactico += "Se espera una expresion\n";
                        correcto = false;
                    }
                }
                else {
                    sintactico += "Se espera un parentesis (\n";
                    correcto = false;
                }
            }
            else {
                sintactico += "Se espera tipo de dato\n";
                correcto = false;
            }
        }
        pos--;
        return false;
    }
    return false;
}

bool E() {
    if (T()) {
        if (tokens[pos].type == SIGNO) {
            pos++;
            if (E()) {
                return true;
            }
        }
        return true;
    }
    return false;
}

bool T() {
    if (F()) {
        if (tokens[pos].type == ASTERISCO && tokens[pos + 1].type == ASTERISCO) {
            pos += 2;
            if (T()) {
                return true;
            }
        }
        if (tokens[pos].type == ASTERISCO || tokens[pos].type == DIVISION) {
            pos++;
            if (T()) {
                return true;
            }
        }
        return true;
    }
    return false;
}

bool F() {
    if (FUNCION()) {
        return true;
    }
    if (tokens[pos].type == NUMERO) {
        pos++;
        return true;
    }
    if (tokens[pos].type == Id) {
        if (leyendoDec) {
            arguments.insert(tokens[pos].lexeme);
        }
        else if (leyendoFun) {
            if (arguments.find(tokens[pos].lexeme) == arguments.end()) {
                semantico += tokens[pos].lexeme;
                semantico += " no ha sido declarada dentro de funcion\n";
                semantic = false;
            }
        }
        else if (simbolos.find(tokens[pos].lexeme) == simbolos.end()) {
            semantico += tokens[pos].lexeme;
            semantico += " no ha sido declarada\n";
            semantic = false;
        }
        pos++;
        return true;
    }
    if (tokens[pos].type == PARENTESIS_ABRE) {
        pos++;
        if (E()) {
            if (tokens[pos].type == PARENTESIS_CIERRA) {
                pos++;
                return true;
            }
            else {
                sintactico += "Debe tener dos parentesis )\n";
                correcto = false;
                return false;
            }
        }
    }
    return false;
}

bool FUNCION() {
    std::string id;
    if (tokens[pos].type == Id) {
        id = tokens[pos].lexeme;
        pos++;
        if (tokens[pos].type == PARENTESIS_ABRE) {
            pos++;
            if (ARGUMENTOS()) {
                if (tokens[pos].type == PARENTESIS_CIERRA) {
                    pos++;
                    if (funciones.find(id) == funciones.end()) {
                        semantico += "Funcion ";
                        semantico += id;
                        semantico += " no ha sido declarada\n";
                        semantic = false;
                    }
                    else if (funciones[id] != 99 && funciones[id] != argumentos) {
                        semantico += "Numero de argumentos en ";
                        semantico += id;
                        semantico += " [";
                        semantico += std::to_string(argumentos);
                        semantico += "] es distinto al esperado [";
                        semantico += std::to_string(funciones[id]);
                        semantico += "]\n";
                        semantic = false;
                    }
                    argumentos = 0;
                    return true;
                }
                sintactico += "Debe tener dos parentesis )\n";
                correcto = false;
                return false;
            }
        }
        pos--;
        return false;
    }
    return false;
}

bool LINEAS_FUN() {
    if (LINEA_FUN()) {
        if (tokens[pos].type == PUNTO_COMA) {
            pos++;
            if (LINEAS_FUN()) {
                return true;
            }
        }
        else {
            sintactico += "Se espera ;\n";
            correcto = false;
            return false;
        }
    }
    return true;
}

bool LINEA_FUN() {
    if (ASSIGN()) {
        return true;
    }
    if (FUNCION()) {
        return true;
    }
    if (E()) {
        return true;
    }
    return false;
}

bool RETURN() {
    if (tokens[pos].type == RET) {
        pos++;
        if (E()) {
            return true;
        }
        return true;
    }
    return false;
}
