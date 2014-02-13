package compiler.fileanalyzer;

import compiler.InvalidExpressionException;

import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

/**
 * User: Javier Isoldi
 * Date: 3/15/13
 * Time: 11:27 AM
 */
public class TokenListFactory {
    private List<Token> tokensList;
    private int readCharacter;

    private static final String[] KEY_WORDS = {"auto", "break", "case", "const", "continue", "default", "do",
            "else", "enum", "extern", "for", "goto", "if", "register", "return", "signed", "static", "struct",
            "switch", "typedef", "union", "unsigned", "volatile", "while"};
    private static final String[] BASIC_TYPES = {"char", "double", "float", "for", "int", "long", "short", "void"};
    private static final String SIZE_OF_OPERATOR = "sizeof";

    private static final String[] ARITHMETIC_OPERATORS = {"+", "-", "*", "/", "%"};
    private static final String[] ASSIGNATION_OPERATORS = {"=", "+=", "-=", "*=", "/="};
    private static final String[] INCREMENTAL_OPERATORS = {"++", "--"};
    private static final String[] ASSOCIATIVE_OPERATORS = {"==", "<", ">", "<=", ">=", "!="};
    private static final String[] LOGIC_OPERATORS = {"||", "&&"};
    private static final String LOGIC_NEG_OPERATOR = "!";
    private static final String COMMA_OPERATOR = ",";

    private static final String[] PRE_PROCESSOR_INSTRUCTION = {"#define", "#undef", "#if", "#ifdef", "#ifndef", "#endif",
            "#else", "#elif", "#include", "#pragma", "#error"};

    public TokenListFactory() {
    }

    public List<Token> getTokenFileFromCFile(Reader cFile) throws IOException, InvalidExpressionException {
        tokensList = new ArrayList<Token>();

        readCharacter = cFile.read();
        StringBuilder readToken = new StringBuilder();
        while (readCharacter != -1) {
            if (('a' <= readCharacter && readCharacter <= 'z') ||
                    ('A' <= readCharacter && readCharacter <= 'Z')) {
                readToken.append((char) readCharacter);
                tokensList.add(readIdentifierOrKeyWord(readToken, cFile));
                readToken = new StringBuilder();
            } else if (readCharacter == '!' || readCharacter == '%' || readCharacter == '+' || readCharacter == '-' ||
                    readCharacter == '|' || readCharacter == '=' || readCharacter == '>') {
                readToken.append((char) readCharacter);
                tokensList.add(readOperator(readToken, cFile));
                readToken = new StringBuilder();
            } else if ('0' <= readCharacter && readCharacter <= '9') {
                readToken.append((char) readCharacter);
                tokensList.add(readConstant(readToken, cFile));
                readToken = new StringBuilder();
            } else {
                switch (readCharacter) {
                    case ' ':
                        readCharacter = cFile.read();
                        break;
                    case '\n':
                        readCharacter = cFile.read();
                        break;
                    case '\t':
                        readCharacter = cFile.read();
                        break;
                    case '/':
                        readToken.append((char) readCharacter);
                        tokensList.add(readDivisionOrComment(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case ',':
                        tokensList.add(new Token(",", TokenType.COMMA_OPERATOR));
                        readCharacter = cFile.read();
                        break;
                    case '.':
                        tokensList.add(new Token(".", TokenType.POINT_OPERATOR));
                        readCharacter = cFile.read();
                        break;
                    case ';':
                        tokensList.add(new Token(";", TokenType.SENTENCE_END));
                        readCharacter = cFile.read();
                        break;
                    case '#':
                        readToken.append((char) readCharacter);
                        tokensList.add(readPreProcessorInstruction(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case '(':
                        tokensList.add(new Token("(", TokenType.OPENING_BRACKET));
                        readCharacter = cFile.read();
                        break;
                    case ')':
                        tokensList.add(new Token(")", TokenType.CLOSING_BRACKET));
                        readCharacter = cFile.read();
                        break;
                    case '{':
                        readToken.append((char) readCharacter);
                        tokensList.add(readBlock(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case '[':
                        readToken.append((char) readCharacter);
                        tokensList.add(readSquareBracketBlock(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case '*':
                        readToken.append((char) readCharacter);
                        Token token = readAsterisk(readToken, cFile);
                        if (token.getType() != TokenType.ASTERISK) {
                            tokensList.add(token);
                        }
                        readToken = new StringBuilder();
                        break;
                    case '&':
                        readToken.append((char) readCharacter);
//                        tokensList.add(readAddressChar(readToken, cFile));
                        readCharacter = cFile.read();
                        readToken = new StringBuilder();
                        break;
                    case '"':
                        readToken.append((char) readCharacter);
                        tokensList.add(readCharChain(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case '<':
                        readToken.append((char) readCharacter);
                        tokensList.add(readLowerChar(readToken, cFile));
                        readToken = new StringBuilder();
                        break;

                    case 13:
                        readCharacter = cFile.read();
                        break;
                    default:
                        throw new InvalidExpressionException(readToken.toString());
                }
            }
        }

        return tokensList;
    }

    private Token readLowerChar(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {

        if (tokensList.get(tokensList.size() - 1).getValue().equals("#include")) {
            readCharacter = cFile.read();
            while (readCharacter != -1 && readCharacter != '\n') {
                readToken.append((char) readCharacter);
                if (readCharacter == '>') {

                    readCharacter = cFile.read();
                    return new Token(readToken.toString(), TokenType.FILE_TO_INCLUDE);

                }
                readCharacter = cFile.read();
            }
            throw new InvalidExpressionException(readToken.toString());
        } else {
            return readOperator(readToken, cFile);
        }
    }

    private Token readCharChain(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.read();

        if (tokensList.get(tokensList.size() - 1).getValue().equals("#include")) {
            while (readCharacter != -1 && readCharacter != '\n') {
                readToken.append((char) readCharacter);
                if (readCharacter == '>') {

                    readCharacter = cFile.read();
                    return new Token(readToken.toString(), TokenType.FILE_TO_INCLUDE);

                }
                readCharacter = cFile.read();
            }
            throw new InvalidExpressionException(readToken.toString());
        } else {
            while (readCharacter != -1 && readCharacter != '\n') {
                readToken.append((char) readCharacter);
                if (readCharacter == '"') {
                    if (readToken.charAt(readToken.length() - 2) != '\\') {
                        readCharacter = cFile.read();
                        return new Token(readToken.toString(), TokenType.CHAR_CHAIN);
                    }
                }
                readCharacter = cFile.read();
            }
            throw new InvalidExpressionException(readToken.toString());
        }

    }

    private Token readAddressChar(StringBuilder readToken, Reader cFile) throws IOException {
        readCharacter = cFile.read();
        if (readCharacter == '&') {
            readToken.append((char) readCharacter);
            readCharacter = cFile.read();
            return new Token(readToken.toString(), TokenType.LOGIC_OPERATOR);
        } else {
            return new Token(readToken.toString(), TokenType.ADDRESS_OPERATOR);
        }
    }

    private Token readConstant(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        boolean hasDecimalPart = false;
        readCharacter = cFile.read();
        while (('0' <= readCharacter && readCharacter <= '9') || readCharacter == '.') {
            if (readCharacter == '.') {
                if (hasDecimalPart) {
                    throw new InvalidExpressionException(readToken.toString());
                } else {
                    hasDecimalPart = true;
                    readToken.append((char) readCharacter);
                }
                readToken.append((char) readCharacter);
            }

        }
        return new Token(readToken.toString(), TokenType.NUMERICAL_CONSTANT);
    }

    private Token readSquareBracketBlock(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        Stack<Character> bracketStack = new Stack<Character>();
        readCharacter = cFile.read();
        while (readCharacter != -1) {
            readToken.append((char) readCharacter);
            if (readCharacter == '[') {
                bracketStack.push((char) readCharacter);
            } else if (readCharacter == ']') {
                if (bracketStack.isEmpty()) {
                    readCharacter = cFile.read();
                    return new Token(readToken.toString(), TokenType.SQUARE_BRACKET_BLOCK);
                } else {
                    bracketStack.pop();
                    readToken.append((char) readCharacter);
                }
            }
            readCharacter = cFile.read();
        }

        throw new InvalidExpressionException(readToken.toString());
    }

    private Token readAsterisk(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.read();
        if (readCharacter == -1) {
            throw new InvalidExpressionException(readToken.toString());
        } else if (readCharacter == '=') {
            readToken.append((char) readCharacter);
            readCharacter = cFile.read();
            return new Token(readToken.toString(), TokenType.ARITHMETIC_OPERATOR);
        }
        return new Token(readToken.toString(), TokenType.ASTERISK);

    }

    private Token readBlock(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        Stack<Character> bracketStack = new Stack<Character>();
        readCharacter = cFile.read();
        while (readCharacter != -1) {
            readToken.append((char) readCharacter);
            if (readCharacter == '{') {
                bracketStack.push((char) readCharacter);
            } else if (readCharacter == '}') {
                if (bracketStack.isEmpty()) {
                    readCharacter = cFile.read();
                    return new Token(readToken.toString(), TokenType.BLOCK);
                } else {
                    bracketStack.pop();
                }
            }
            readCharacter = cFile.read();
        }

        throw new InvalidExpressionException(readToken.toString());
    }

    private Token readPreProcessorInstruction(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.read();
        while (readCharacter != -1 && (('a' <= readCharacter && readCharacter <= 'z') ||
                ('A' <= readCharacter && readCharacter <= 'Z') ||
                ('0' <= readCharacter && readCharacter <= '9'))) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.read();
        }
        String readIdentifier = readToken.toString();
        for (String keyWord : PRE_PROCESSOR_INSTRUCTION) {
            if (readIdentifier.equals(keyWord)) {
                return new Token(readIdentifier, TokenType.PRE_PROCESSOR_INSTRUCTION);
            }
        }

        throw new InvalidExpressionException(readToken.toString());
    }

    private Token readDivisionOrComment(StringBuilder readToken, Reader cFile) throws IOException {
        readCharacter = cFile.read();
        if (readCharacter != '*' && readCharacter != '/') {
            return new Token(readToken.toString(), TokenType.ARITHMETIC_OPERATOR);
        }
        if (readCharacter == '*') {
            readToken.append(readCharacter);
            readCharacter = cFile.read();
            while (readCharacter != -1) {
                readToken.append((char) readCharacter);
                if (readCharacter == '*') {
                    readCharacter = cFile.read();
                    readToken.append((char) readCharacter);
                    if (readCharacter == '/') {
                        break;
                    }
                }
                readCharacter = cFile.read();
            }
        } else {
            readToken.append(readCharacter);
            readCharacter = cFile.read();
            while (readCharacter != -1) {
                readToken.append((char) readCharacter);
                if (readCharacter == '\n') {
                    break;
                }
                readCharacter = cFile.read();
            }
        }
        return new Token(readToken.toString(), TokenType.COMMENT);

    }

    private Token readOperator(StringBuilder readToken, Reader cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.read();
        while (readCharacter != -1 && (readCharacter == '!' || readCharacter == '%' || readCharacter == '+' ||
                readCharacter == '|' || readCharacter == '-' || readCharacter == '<' || readCharacter == '=' ||
                readCharacter == '>')) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.read();
        }
        String readOperator = readToken.toString();
        for (String arithmeticOperator : ARITHMETIC_OPERATORS) {
            if (readOperator.equals(arithmeticOperator)) {
                return new Token(readOperator, TokenType.ARITHMETIC_OPERATOR);
            }
        }
        for (String assignationOperator : ASSIGNATION_OPERATORS) {
            if (readOperator.equals(assignationOperator)) {
                return new Token(readOperator, TokenType.ASSIGNATION_OPERATOR);
            }
        }
        for (String incrementalOperator : INCREMENTAL_OPERATORS) {
            if (readOperator.equals(incrementalOperator)) {
                return new Token(readOperator, TokenType.INCREMENTAL_OPERATOR);
            }
        }
        for (String associativeOperator : ASSOCIATIVE_OPERATORS) {
            if (readOperator.equals(associativeOperator)) {
                return new Token(readOperator, TokenType.ASSOCIATIVE_OPERATOR);
            }
        }
        for (String logicOperator : LOGIC_OPERATORS) {
            if (readOperator.equals(logicOperator)) {
                return new Token(readOperator, TokenType.LOGIC_OPERATOR);
            }
        }
        if (readOperator.equals(LOGIC_NEG_OPERATOR)) {
            return new Token(readOperator, TokenType.LOGIC_NEG_OPERATOR);
        }
        if (readOperator.equals(COMMA_OPERATOR)) {
            return new Token(readOperator, TokenType.COMMA_OPERATOR);
        }

        throw new InvalidExpressionException(readOperator);
    }

    private Token readIdentifierOrKeyWord(StringBuilder readToken, Reader cFile) throws IOException {
        readCharacter = cFile.read();
        while (readCharacter != -1 && (('a' <= readCharacter && readCharacter <= 'z') ||
                ('A' <= readCharacter && readCharacter <= 'Z') ||
                ('0' <= readCharacter && readCharacter <= '9'))) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.read();
        }
        String readIdentifier = readToken.toString();
        for (String keyWord : KEY_WORDS) {
            if (readIdentifier.equals(keyWord)) {
                return new Token(readIdentifier, TokenType.KEY_WORD);
            }
        }

        for (String basicType : BASIC_TYPES) {
            if (readIdentifier.equals(basicType)) {
                return new Token(readIdentifier, TokenType.BASIC_TYPE);
            }
        }

        if (readIdentifier.equals(SIZE_OF_OPERATOR)) {
            return new Token(readIdentifier, TokenType.SIZE_OF_OPERATOR);
        }

        return new Token(readIdentifier, TokenType.IDENTIFIER);
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        for (Token token : tokensList) {
            result.append("\n").append(token.toString());
        }

        return result.toString();
    }


}
