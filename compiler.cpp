#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

enum TokenType {
    TOK_INT_LIT, TOK_IDENT, TOK_STR_LIT,
    TOK_INT, TOK_IF, TOK_ELSE, TOK_WHILE, TOK_FOR, TOK_PRINT, TOK_RETURN,
    TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_ASSIGN,
    TOK_EQ, TOK_NEQ, TOK_LT, TOK_GT, TOK_LTE, TOK_GTE,
    TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_LBRACKET, TOK_RBRACKET,
    TOK_SEMI, TOK_COMMA,
    TOK_EOF
};

struct Token {
    TokenType type;
    string value;
};

class Lexer {
    string src;
    int pos;
public:
    Lexer(string source) : src(source), pos(0) {}

    char peek() { return pos < (int)src.size() ? src[pos] : '\0'; }
    char advance() { return src[pos++]; }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < (int)src.size()) {
            char c = peek();

            if (isspace(c)) { advance(); continue; }

            if (c == '/' && pos + 1 < (int)src.size() && src[pos+1] == '/') {
                while (peek() != '\n' && peek() != '\0') advance();
                continue;
            }

            if (c == '"') {
                advance();
                string str;
                while (peek() != '"' && peek() != '\0') str += advance();
                advance();
                tokens.push_back({TOK_STR_LIT, str});
                continue;
            }

            if (isdigit(c)) {
                string num;
                while (isdigit(peek())) num += advance();
                tokens.push_back({TOK_INT_LIT, num});
                continue;
            }

            if (isalpha(c) || c == '_') {
                string word;
                while (isalnum(peek()) || peek() == '_') word += advance();
                if      (word == "int")    tokens.push_back({TOK_INT,    word});
                else if (word == "if")     tokens.push_back({TOK_IF,     word});
                else if (word == "else")   tokens.push_back({TOK_ELSE,   word});
                else if (word == "while")  tokens.push_back({TOK_WHILE,  word});
                else if (word == "for")    tokens.push_back({TOK_FOR,    word});
                else if (word == "print")  tokens.push_back({TOK_PRINT,  word});
                else if (word == "return") tokens.push_back({TOK_RETURN, word});
                else                       tokens.push_back({TOK_IDENT,  word});
                continue;
            }

            advance();
            switch (c) {
                case '+': tokens.push_back({TOK_PLUS,     "+"}); break;
                case '-': tokens.push_back({TOK_MINUS,    "-"}); break;
                case '*': tokens.push_back({TOK_MUL,      "*"}); break;
                case '/': tokens.push_back({TOK_DIV,      "/"}); break;
                case '(': tokens.push_back({TOK_LPAREN,   "("}); break;
                case ')': tokens.push_back({TOK_RPAREN,   ")"}); break;
                case '{': tokens.push_back({TOK_LBRACE,   "{"}); break;
                case '}': tokens.push_back({TOK_RBRACE,   "}"}); break;
                case '[': tokens.push_back({TOK_LBRACKET, "["}); break;
                case ']': tokens.push_back({TOK_RBRACKET, "]"}); break;
                case ';': tokens.push_back({TOK_SEMI,     ";"}); break;
                case ',': tokens.push_back({TOK_COMMA,    ","}); break;
                case '=':
                    if (peek() == '=') { advance(); tokens.push_back({TOK_EQ,     "=="}); }
                    else                             tokens.push_back({TOK_ASSIGN, "="});
                    break;
                case '!':
                    if (peek() == '=') { advance(); tokens.push_back({TOK_NEQ, "!="}); }
                    break;
                case '<':
                    if (peek() == '=') { advance(); tokens.push_back({TOK_LTE, "<="}); }
                    else                             tokens.push_back({TOK_LT,  "<"});
                    break;
                case '>':
                    if (peek() == '=') { advance(); tokens.push_back({TOK_GTE, ">="}); }
                    else                             tokens.push_back({TOK_GT,  ">"});
                    break;
                default:
                    cerr << "Unknown character: " << c << endl;
            }
        }
        tokens.push_back({TOK_EOF, ""});
        return tokens;
    }
};

struct ASTNode {
    string type;
    string value;
    ASTNode* left;
    ASTNode* right;
    ASTNode* condition;
    ASTNode* elseBody;
    ASTNode* init;
    ASTNode* update;
    ASTNode* index;     
    vector<ASTNode*> body;
    vector<ASTNode*> args;
    vector<string> params;
    int arraySize;     

    ASTNode(string t, string v = "") : type(t), value(v), left(0), right(0),
        condition(0), elseBody(0), init(0), update(0), index(0), arraySize(0) {}
};

class Parser {
    vector<Token> tokens;
    int pos;

    Token peek()  { return tokens[pos]; }
    Token advance() { return tokens[pos++]; }
    Token expect(TokenType t) {
        if (peek().type != t) {
            cerr << "Parse error: unexpected token '" << peek().value << "'" << endl;
            exit(1);
        }
        return advance();
    }

    bool isFuncDef() {
        return tokens[pos].type   == TOK_INT   &&
               tokens[pos+1].type == TOK_IDENT &&
               tokens[pos+2].type == TOK_LPAREN;
    }

    bool isArrayDecl() {
        return tokens[pos].type   == TOK_INT   &&
               tokens[pos+1].type == TOK_IDENT &&
               tokens[pos+2].type == TOK_LBRACKET;
    }

public:
    Parser(vector<Token> toks) : tokens(toks), pos(0) {}

    vector<ASTNode*> parseProgram() {
        vector<ASTNode*> stmts;
        while (peek().type != TOK_EOF)
            stmts.push_back(isFuncDef() ? parseFuncDef() : parseStatement());
        return stmts;
    }

    ASTNode* parseFuncDef() {
        expect(TOK_INT);
        string name = expect(TOK_IDENT).value;
        expect(TOK_LPAREN);
        ASTNode* node = new ASTNode("funcdef", name);
        while (peek().type != TOK_RPAREN) {
            expect(TOK_INT);
            string paramName = expect(TOK_IDENT).value;
            node->params.push_back(paramName);
            if (peek().type == TOK_COMMA) advance();
        }
        expect(TOK_RPAREN);
        expect(TOK_LBRACE);
        while (peek().type != TOK_RBRACE)
            node->body.push_back(parseStatement());
        expect(TOK_RBRACE);
        return node;
    }

    ASTNode* parseStatement() {
        Token t = peek();

        if (isArrayDecl()) {
            expect(TOK_INT);
            string name = expect(TOK_IDENT).value;
            expect(TOK_LBRACKET);
            int size = stoi(expect(TOK_INT_LIT).value);
            expect(TOK_RBRACKET);
            expect(TOK_SEMI);
            ASTNode* node = new ASTNode("arraydecl", name);
            node->arraySize = size;
            return node;
        }

        if (t.type == TOK_INT) {
            advance();
            string name = expect(TOK_IDENT).value;
            expect(TOK_ASSIGN);
            ASTNode* expr = parseExpr();
            expect(TOK_SEMI);
            ASTNode* node = new ASTNode("declare", name);
            node->left = expr;
            return node;
        }

        if (t.type == TOK_IDENT) {
            if (tokens[pos+1].type == TOK_LPAREN) {
                ASTNode* call = parseFuncCall();
                expect(TOK_SEMI);
                return call;
            }
            if (tokens[pos+1].type == TOK_LBRACKET) {
                string name = advance().value;
                expect(TOK_LBRACKET);
                ASTNode* idx = parseExpr();
                expect(TOK_RBRACKET);
                expect(TOK_ASSIGN);
                ASTNode* expr = parseExpr();
                expect(TOK_SEMI);
                ASTNode* node = new ASTNode("arrayassign", name);
                node->index = idx;
                node->left  = expr;
                return node;
            }
            string name = advance().value;
            expect(TOK_ASSIGN);
            ASTNode* expr = parseExpr();
            expect(TOK_SEMI);
            ASTNode* node = new ASTNode("assign", name);
            node->left = expr;
            return node;
        }

        if (t.type == TOK_IF) {
            advance();
            expect(TOK_LPAREN);
            ASTNode* cond = parseExpr();
            expect(TOK_RPAREN);
            expect(TOK_LBRACE);
            ASTNode* node = new ASTNode("if");
            node->condition = cond;
            while (peek().type != TOK_RBRACE) node->body.push_back(parseStatement());
            expect(TOK_RBRACE);
            if (peek().type == TOK_ELSE) {
                advance();
                expect(TOK_LBRACE);
                ASTNode* elseNode = new ASTNode("else");
                while (peek().type != TOK_RBRACE) elseNode->body.push_back(parseStatement());
                expect(TOK_RBRACE);
                node->elseBody = elseNode;
            }
            return node;
        }

        if (t.type == TOK_WHILE) {
            advance();
            expect(TOK_LPAREN);
            ASTNode* cond = parseExpr();
            expect(TOK_RPAREN);
            expect(TOK_LBRACE);
            ASTNode* node = new ASTNode("while");
            node->condition = cond;
            while (peek().type != TOK_RBRACE) node->body.push_back(parseStatement());
            expect(TOK_RBRACE);
            return node;
        }

        if (t.type == TOK_FOR) {
            advance();
            expect(TOK_LPAREN);
            ASTNode* node = new ASTNode("for");
            expect(TOK_INT);
            string initName = expect(TOK_IDENT).value;
            expect(TOK_ASSIGN);
            ASTNode* initExpr = parseExpr();
            expect(TOK_SEMI);
            ASTNode* initNode = new ASTNode("declare", initName);
            initNode->left = initExpr;
            node->init = initNode;
            node->condition = parseExpr();
            expect(TOK_SEMI);
            string updateName = expect(TOK_IDENT).value;
            expect(TOK_ASSIGN);
            ASTNode* updateExpr = parseExpr();
            ASTNode* updateNode = new ASTNode("assign", updateName);
            updateNode->left = updateExpr;
            node->update = updateNode;
            expect(TOK_RPAREN);
            expect(TOK_LBRACE);
            while (peek().type != TOK_RBRACE) node->body.push_back(parseStatement());
            expect(TOK_RBRACE);
            return node;
        }

        if (t.type == TOK_PRINT) {
            advance();
            expect(TOK_LPAREN);
            ASTNode* node = new ASTNode("print");
            if (peek().type == TOK_STR_LIT) {
                node->value = advance().value;
                node->left  = 0;
            } else {
                node->left = parseExpr();
            }
            expect(TOK_RPAREN);
            expect(TOK_SEMI);
            return node;
        }

        if (t.type == TOK_RETURN) {
            advance();
            ASTNode* expr = parseExpr();
            expect(TOK_SEMI);
            ASTNode* node = new ASTNode("return");
            node->left = expr;
            return node;
        }

        cerr << "Parse error: unexpected token '" << t.value << "'" << endl;
        exit(1);
    }

    ASTNode* parseFuncCall() {
        string name = advance().value;
        expect(TOK_LPAREN);
        ASTNode* node = new ASTNode("funccall", name);
        while (peek().type != TOK_RPAREN) {
            node->args.push_back(parseExpr());
            if (peek().type == TOK_COMMA) advance();
        }
        expect(TOK_RPAREN);
        return node;
    }

    ASTNode* parseExpr() { return parseComparison(); }

    ASTNode* parseComparison() {
        ASTNode* left = parseAddSub();
        Token t = peek();
        if (t.type == TOK_EQ  || t.type == TOK_NEQ ||
            t.type == TOK_LT  || t.type == TOK_GT  ||
            t.type == TOK_LTE || t.type == TOK_GTE) {
            advance();
            ASTNode* node = new ASTNode("binop", t.value);
            node->left  = left;
            node->right = parseAddSub();
            return node;
        }
        return left;
    }

    ASTNode* parseAddSub() {
        ASTNode* left = parseMulDiv();
        while (peek().type == TOK_PLUS || peek().type == TOK_MINUS) {
            string op = advance().value;
            ASTNode* node = new ASTNode("binop", op);
            node->left  = left;
            node->right = parseMulDiv();
            left = node;
        }
        return left;
    }

    ASTNode* parseMulDiv() {
        ASTNode* left = parsePrimary();
        while (peek().type == TOK_MUL || peek().type == TOK_DIV) {
            string op = advance().value;
            ASTNode* node = new ASTNode("binop", op);
            node->left  = left;
            node->right = parsePrimary();
            left = node;
        }
        return left;
    }

    ASTNode* parsePrimary() {
        Token t = peek();
        if (t.type == TOK_INT_LIT) { advance(); return new ASTNode("num", t.value); }
        if (t.type == TOK_IDENT) {
            if (tokens[pos+1].type == TOK_LPAREN) return parseFuncCall();
            if (tokens[pos+1].type == TOK_LBRACKET) {
                string name = advance().value;
                expect(TOK_LBRACKET);
                ASTNode* idx = parseExpr();
                expect(TOK_RBRACKET);
                ASTNode* node = new ASTNode("arrayaccess", name);
                node->index = idx;
                return node;
            }
            advance();
            return new ASTNode("var", t.value);
        }
        if (t.type == TOK_LPAREN) {
            advance();
            ASTNode* expr = parseExpr();
            expect(TOK_RPAREN);
            return expr;
        }
        cerr << "Parse error in primary: unexpected '" << t.value << "'" << endl;
        exit(1);
    }
};

struct FuncInfo { vector<string> params; };

class CodeGen {
    map<string, int>      varOffset;
    map<string, int>      arrayBase;   
    map<string, FuncInfo> functions;
    map<string, string>   stringLabels;
    int stackSize;
    int labelCount;
    int strCount;
    ostringstream data;
    ostringstream text;

    int newLabel() { return labelCount++; }

    string getStringLabel(const string& str) {
        if (stringLabels.find(str) == stringLabels.end()) {
            string lbl = "str" + to_string(strCount++);
            stringLabels[str] = lbl;
            data << "    " << lbl << " db \"" << str << "\", 0\n";
        }
        return stringLabels[str];
    }

    void genArrayAddr(const string& name, ASTNode* indexNode) {
        if (arrayBase.find(name) == arrayBase.end()) {
            cerr << "Undefined array: " << name << endl; exit(1);
        }
        int base = arrayBase[name];
        genExpr(indexNode);                          
        text << "    imul eax, 4\n";                 
        text << "    mov ecx, ebp\n";
        text << "    sub ecx, " << base << "\n";     
        text << "    add ecx, eax\n";                
        text << "    mov eax, ecx\n";                
    }

    void genExpr(ASTNode* node) {
        if (node->type == "num") {
            text << "    mov eax, " << node->value << "\n";
        }
        else if (node->type == "var") {
            if (varOffset.find(node->value) == varOffset.end()) {
                cerr << "Undefined variable: " << node->value << endl; exit(1);
            }
            int offset = varOffset[node->value];
            if (offset < 0)
                text << "    mov eax, [ebp + " << -offset << "]\n";
            else
                text << "    mov eax, [ebp - " << offset << "]\n";
        }
        else if (node->type == "arrayaccess") {
            genArrayAddr(node->value, node->index);
            text << "    mov eax, [eax]\n";  
        }
        else if (node->type == "funccall") {
            genFuncCall(node);
        }
        else if (node->type == "binop") {
            string op = node->value;
            bool isCmp = (op=="==" || op=="!=" || op=="<" || op==">" || op=="<=" || op==">=");

            genExpr(node->left);
            text << "    push eax\n";
            genExpr(node->right);
            text << "    mov ecx, eax\n";
            text << "    pop eax\n";

            if (!isCmp) {
                if      (op == "+") text << "    add eax, ecx\n";
                else if (op == "-") text << "    sub eax, ecx\n";
                else if (op == "*") text << "    imul eax, ecx\n";
                else if (op == "/") { text << "    cdq\n"; text << "    idiv ecx\n"; }
            } else {
                text << "    cmp eax, ecx\n";
                text << "    mov eax, 0\n";
                if      (op == "==") text << "    sete al\n";
                else if (op == "!=") text << "    setne al\n";
                else if (op == "<")  text << "    setl al\n";
                else if (op == ">")  text << "    setg al\n";
                else if (op == "<=") text << "    setle al\n";
                else if (op == ">=") text << "    setge al\n";
            }
        }
    }

    void genFuncCall(ASTNode* node) {
        for (int i = (int)node->args.size() - 1; i >= 0; i--) {
            genExpr(node->args[i]);
            text << "    push eax\n";
        }
        text << "    call _" << node->value << "\n";
        if (!node->args.empty())
            text << "    add esp, " << node->args.size() * 4 << "\n";
    }

    void genStatement(ASTNode* node) {
        if (node->type == "declare") {
            stackSize += 4;
            varOffset[node->value] = stackSize;
            text << "    sub esp, 4\n";
            genExpr(node->left);
            text << "    mov [ebp - " << varOffset[node->value] << "], eax\n";
        }
        else if (node->type == "arraydecl") {
            int size = node->arraySize;
            int totalBytes = size * 4;
            stackSize += totalBytes;
            arrayBase[node->value] = stackSize - totalBytes + 4;
            text << "    sub esp, " << totalBytes << "\n";
        }
        else if (node->type == "assign") {
            if (varOffset.find(node->value) == varOffset.end()) {
                cerr << "Undefined variable: " << node->value << endl; exit(1);
            }
            genExpr(node->left);
            int offset = varOffset[node->value];
            if (offset < 0)
                text << "    mov [ebp + " << -offset << "], eax\n";
            else
                text << "    mov [ebp - " << offset << "], eax\n";
        }
        else if (node->type == "arrayassign") {
            genExpr(node->left);
            text << "    push eax\n";
            genArrayAddr(node->value, node->index);
            text << "    pop ecx\n";
            text << "    mov [eax], ecx\n";
        }
        else if (node->type == "if") {
            int lbl = newLabel();
            genExpr(node->condition);
            text << "    cmp eax, 0\n";
            if (node->elseBody) text << "    je .else" << lbl << "\n";
            else                text << "    je .endif" << lbl << "\n";
            for (ASTNode* s : node->body) genStatement(s);
            if (node->elseBody) {
                text << "    jmp .endif" << lbl << "\n";
                text << ".else" << lbl << ":\n";
                for (ASTNode* s : node->elseBody->body) genStatement(s);
            }
            text << ".endif" << lbl << ":\n";
        }
        else if (node->type == "while") {
            int lbl = newLabel();
            text << ".while" << lbl << ":\n";
            genExpr(node->condition);
            text << "    cmp eax, 0\n";
            text << "    je .endwhile" << lbl << "\n";
            for (ASTNode* s : node->body) genStatement(s);
            text << "    jmp .while" << lbl << "\n";
            text << ".endwhile" << lbl << ":\n";
        }
        else if (node->type == "for") {
            int lbl = newLabel();
            genStatement(node->init);
            text << ".for" << lbl << ":\n";
            genExpr(node->condition);
            text << "    cmp eax, 0\n";
            text << "    je .endfor" << lbl << "\n";
            for (ASTNode* s : node->body) genStatement(s);
            genStatement(node->update);
            text << "    jmp .for" << lbl << "\n";
            text << ".endfor" << lbl << ":\n";
        }
        else if (node->type == "print") {
            if (node->left == 0) {
                string lbl = getStringLabel(node->value);
                text << "    push " << lbl << "\n";
                text << "    push fmtstr\n";
                text << "    call _printf\n";
                text << "    add esp, 8\n";
            } else {
                genExpr(node->left);
                text << "    push eax\n";
                text << "    push fmt\n";
                text << "    call _printf\n";
                text << "    add esp, 8\n";
            }
        }
        else if (node->type == "funccall") {
            genFuncCall(node);
        }
        else if (node->type == "return") {
            genExpr(node->left);
            text << "    mov esp, ebp\n";
            text << "    pop ebp\n";
            text << "    ret\n";
        }
    }

    void genFuncDef(ASTNode* node) {
        FuncInfo info;
        info.params = node->params;
        functions[node->value] = info;

        varOffset.clear();
        arrayBase.clear();
        stackSize = 0;

        text << "_" << node->value << ":\n";
        text << "    push ebp\n";
        text << "    mov ebp, esp\n\n";

        for (int i = 0; i < (int)node->params.size(); i++)
            varOffset[node->params[i]] = -(8 + i * 4);

        for (ASTNode* s : node->body) genStatement(s);

        text << "    mov eax, 0\n";
        text << "    mov esp, ebp\n";
        text << "    pop ebp\n";
        text << "    ret\n\n";
    }

public:
    CodeGen() : stackSize(0), labelCount(0), strCount(0) {}

    string generate(vector<ASTNode*> stmts) {
        data << "section .data\n";
        data << "    fmt    db \"%d\", 10, 0\n";
        data << "    fmtstr db \"%s\", 10, 0\n";

        text << "\nsection .text\n";
        text << "    global _main\n";
        text << "    extern _printf\n\n";

        for (ASTNode* s : stmts)
            if (s->type == "funcdef") genFuncDef(s);

        varOffset.clear();
        arrayBase.clear();
        stackSize = 0;

        text << "_main:\n";
        text << "    push ebp\n";
        text << "    mov ebp, esp\n\n";

        for (ASTNode* s : stmts)
            if (s->type != "funcdef") genStatement(s);

        text << "\n    mov eax, 0\n";
        text << "    mov esp, ebp\n";
        text << "    pop ebp\n";
        text << "    ret\n";

        return data.str() + text.str();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: minic <source.mc>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) { cerr << "Cannot open file: " << argv[1] << endl; return 1; }
    stringstream buf;
    buf << file.rdbuf();
    string src = buf.str();

    Lexer lexer(src);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    vector<ASTNode*> ast = parser.parseProgram();

    CodeGen codegen;
    string asmCode = codegen.generate(ast);

    ofstream out("output.asm");
    out << asmCode;
    cout << "Compiled successfully -> output.asm" << endl;

    return 0;
}