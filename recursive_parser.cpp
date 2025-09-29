#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

// This example implements a tiny recursive descent parser for arithmetic expressions.
// Parsing relies on a grammar where expression -> term ((+|-) term)* and term -> factor ((*|/) factor)*.
// Each non-terminal is mapped to a mutually recursive function. Because recursive descent follows the
// grammar structure literally, it is a natural way to parse nested constructs such as parentheses.

enum class TokenType {
    Number,
    Plus,
    Minus,
    Multiply,
    Divide,
    LParen,
    RParen,
    End
};

struct Token {
    TokenType type;
    double value; // Only meaningful when type == Number
};

class Lexer {
public:
    explicit Lexer(const std::string& input) : text(input), pos(0) {}

    Token nextToken() {
        skipWhitespace();
        if (pos >= text.size()) {
            return {TokenType::End, 0.0};
        }
        char current = text[pos];
        if (std::isdigit(static_cast<unsigned char>(current)) || current == '.') {
            return numberToken();
        }
        ++pos;
        switch (current) {
            case '+': return {TokenType::Plus, 0.0};
            case '-': return {TokenType::Minus, 0.0};
            case '*': return {TokenType::Multiply, 0.0};
            case '/': return {TokenType::Divide, 0.0};
            case '(': return {TokenType::LParen, 0.0};
            case ')': return {TokenType::RParen, 0.0};
            default:
                throw std::runtime_error(std::string("Unexpected character: ") + current);
        }
    }

private:
    std::string text;
    size_t pos;

    void skipWhitespace() {
        while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) {
            ++pos;
        }
    }

    Token numberToken() {
        size_t start = pos;
        while (pos < text.size() && (std::isdigit(static_cast<unsigned char>(text[pos])) || text[pos] == '.')) {
            ++pos;
        }
        double value = std::stod(text.substr(start, pos - start));
        return {TokenType::Number, value};
    }
};

class Parser {
public:
    explicit Parser(Lexer lexer) : lexer(std::move(lexer)), currentToken(read()) {}

    double parseExpression() {
        return expression();
    }

private:
    Lexer lexer;
    Token currentToken;

    Token read() {
        return lexer.nextToken();
    }

    void consume(TokenType expected) {
        if (currentToken.type != expected) {
            throw std::runtime_error("Unexpected token");
        }
        currentToken = read();
    }

    double expression() {
        double value = term();
        while (currentToken.type == TokenType::Plus || currentToken.type == TokenType::Minus) {
            TokenType op = currentToken.type;
            consume(op);
            if (op == TokenType::Plus) {
                value += term();
            } else {
                value -= term();
            }
        }
        return value;
    }

    double term() {
        double value = factor();
        while (currentToken.type == TokenType::Multiply || currentToken.type == TokenType::Divide) {
            TokenType op = currentToken.type;
            consume(op);
            if (op == TokenType::Multiply) {
                value *= factor();
            } else {
                double divisor = factor();
                if (divisor == 0.0) {
                    throw std::runtime_error("Division by zero");
                }
                value /= divisor;
            }
        }
        return value;
    }

    double factor() {
        if (currentToken.type == TokenType::Number) {
            double value = currentToken.value;
            consume(TokenType::Number);
            return value;
        }
        if (currentToken.type == TokenType::Minus) {
            consume(TokenType::Minus);
            return -factor();
        }
        if (currentToken.type == TokenType::LParen) {
            consume(TokenType::LParen);
            double value = expression();
            consume(TokenType::RParen);
            return value;
        }
        throw std::runtime_error("Expected number or parenthesis");
    }
};

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression (supports +, -, *, /, parentheses):\n";
    std::getline(std::cin, input);

    try {
        Lexer lexer(input);
        Parser parser(lexer);
        double result = parser.parseExpression();
        std::cout << "Result: " << result << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "Parsing error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
