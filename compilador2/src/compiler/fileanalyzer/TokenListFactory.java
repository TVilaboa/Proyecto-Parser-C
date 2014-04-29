package compiler.fileanalyzer;

import compiler.InvalidExpressionException;

import java.io.IOException;
import java.io.Reader;
import java.util.*;

/**
 * User: Javier Isoldi
 * Date: 3/15/13
 * Time: 11:27 AM
 */
public class TokenListFactory {
    private static final String[] KEY_WORDS = {"auto", "break", "case", "const", "continue", "default", "do",
            "else", "enum", "extern", "for", "goto", "if", "register", "return", "signed", "static", "struct",
            "switch", "typedef", "union", "unsigned", "volatile", "while"};
    private static final String[] BASIC_TYPES = {"char", "double", "float", "for", "int", "long", "short", "void", "FILE", "time_t"};
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
    private List<Token> tokensList;
    private char readCharacter;
    private Map<String, Integer> globalAttributes;

    public TokenListFactory(Map<String, Integer> globalAttributes) {
        this.globalAttributes = globalAttributes;
    }

    public List<Token> getTokenFileFromCFile(Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        tokensList = new ArrayList<Token>();
        readCharacter = cFile.next();
        StringBuilder readToken = new StringBuilder();
        while (cFile.hasNext()) {
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
                        readCharacter = cFile.next();
                        break;
                    case '\n':
                        readCharacter = cFile.next();
                        break;
                    case '\t':
                        readCharacter = cFile.next();
                        break;
                    case '/':
                        readToken.append((char) readCharacter);
                        tokensList.add(readDivisionOrComment(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case ',':
                        tokensList.add(new Token(",", TokenType.COMMA_OPERATOR));
                        readCharacter = cFile.next();
                        break;
                    case '.':
                        tokensList.add(new Token(".", TokenType.POINT_OPERATOR));
                        readCharacter = cFile.next();
                        break;
                    case ';':
                        tokensList.add(new Token(";", TokenType.SENTENCE_END));
                        readCharacter = cFile.next();
                        break;
                    case '#':
                        readToken.append((char) readCharacter);
                        tokensList.add(readPreProcessorInstruction(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    case '(':
                        tokensList.add(new Token("(", TokenType.OPENING_BRACKET));
                        readCharacter = cFile.next();
                        break;
                    case ')':
                        tokensList.add(new Token(")", TokenType.CLOSING_BRACKET));
                        readCharacter = cFile.next();
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
                        Token token;
                        if (tokensList.get(tokensList.size() - 1).getType() == TokenType.NUMERICAL_CONSTANT) {
                            token = readNumericalAsterisk(cFile, Integer.parseInt(tokensList.get(tokensList.size() - 1).getValue()), readToken);
                            tokensList.remove(tokensList.size() - 1);
                        } else {
                            token = readAsterisk(readToken, cFile);
                        }
                        if (token.getType() != TokenType.ASTERISK) {
                            tokensList.add(token);
                        }
                        readToken = new StringBuilder();
                        break;
                    case '&':
                        readToken.append((char) readCharacter);
//                        tokensList.add(readAddressChar(readToken, cFile));
                        readCharacter = cFile.next();
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


                    case '\'':
                        readCharacter = cFile.next();
                        break;

                    case 13:     // case carriage return
                        readCharacter = cFile.next();
                        break;
                    case '_':
                        readToken.append((char) readCharacter);
                        tokensList.add(readIdentifierOrKeyWord(readToken, cFile));
                        readToken = new StringBuilder();
                        break;
                    default:
                        throw new InvalidExpressionException(readToken.toString());
                }
            }
        }
        Token token = null;
        for (Iterator<Token> iterator = tokensList.iterator(); iterator.hasNext(); ) {
            token = iterator.next();
            if (token.getType() == TokenType.COMMENT) //delets comments
                iterator.remove();

        }

        return tokensList;
    }

    public List<Token> getTokenFileFromCFile(Reader cFile) throws IOException, InvalidExpressionException {
        int readCharacter = cFile.read();
        StringBuilder readToken = new StringBuilder();
        while (readCharacter != -1) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.read();
        }
        char[] chars = readToken.toString().toCharArray();
        List<Character> characters = new ArrayList<>();
        for (Character c : chars) {
            characters.add(c);
        }
        return getTokenFileFromCFile(characters.iterator());
    }

    private Token readLowerChar(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {

        if (tokensList.get(tokensList.size() - 1).getValue().equals("#include")) {
            readCharacter = cFile.next();
            while (readCharacter != -1 && readCharacter != '\n') {
                readToken.append((char) readCharacter);
                if (readCharacter == '>') {

                    readCharacter = cFile.next();
                    return new Token(readToken.toString(), TokenType.FILE_TO_INCLUDE);

                }
                readCharacter = cFile.next();
            }
            throw new InvalidExpressionException(readToken.toString());
        } else {
            return readOperator(readToken, cFile);
        }
    }

    private Token readCharChain(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.next();

        if (tokensList.get(tokensList.size() - 1).getValue().equals("#include")) {
            while (readCharacter != -1 && readCharacter != '\n') {
                readToken.append((char) readCharacter);
                if (readCharacter == '>' || readCharacter == '"') {

                    readCharacter = cFile.next();
                    return new Token(readToken.toString(), TokenType.FILE_TO_INCLUDE);

                }
                readCharacter = cFile.next();
            }
            throw new InvalidExpressionException(readToken.toString());
        } else {
            while (readCharacter != -1 && readCharacter != '\n') {
                readToken.append((char) readCharacter);
                if (readCharacter == '"') {
                    if (readToken.charAt(readToken.length() - 2) != '\\') {
                        readCharacter = cFile.next();
                        return new Token(readToken.toString(), TokenType.CHAR_CHAIN);
                    }
                }
                readCharacter = cFile.next();
            }
            throw new InvalidExpressionException(readToken.toString());
        }

    }

    private Token readAddressChar(StringBuilder readToken, Iterator<Character> cFile) throws IOException {
        readCharacter = cFile.next();
        if (readCharacter == '&') {
            readToken.append((char) readCharacter);
            readCharacter = cFile.next();
            return new Token(readToken.toString(), TokenType.LOGIC_OPERATOR);
        } else {
            return new Token(readToken.toString(), TokenType.ADDRESS_OPERATOR);
        }
    }

    private Token readConstant(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        boolean hasDecimalPart = false;
        readCharacter = cFile.next();
        while (('0' <= readCharacter && readCharacter <= '9') || readCharacter == '.') {
            if (readCharacter == '.') {
                if (hasDecimalPart) {
                    throw new InvalidExpressionException(readToken.toString());
                } else {
                    hasDecimalPart = true;
                    readToken.append((char) readCharacter);
                }
                readToken.append((char) readCharacter);
            } else {
                readToken.append((char) readCharacter);
                readCharacter = cFile.next();
            }

        }
        return new Token(readToken.toString(), TokenType.NUMERICAL_CONSTANT);
    }

    private Token readSquareBracketBlock(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        Stack<Character> bracketStack = new Stack<Character>();
        readCharacter = cFile.next();
        while (readCharacter != -1) {
            readToken.append((char) readCharacter);
            if (readCharacter == '[') {
                bracketStack.push((char) readCharacter);
            } else if (readCharacter == ']') {
                if (bracketStack.isEmpty()) {
                    readCharacter = cFile.next();
                    String tokenStr = readToken.toString();
                    if (!globalAttributes.isEmpty()) {
                        for (Map.Entry<String, Integer> entry : globalAttributes.entrySet()) {
                            tokenStr = tokenStr.replaceAll(entry.getKey(), entry.getValue() + ""); //replaces each globalAttribute ocurrence for its actual valor
                        }
                    }
                    return new Token(tokenStr, TokenType.SQUARE_BRACKET_BLOCK);
                } else {
                    bracketStack.pop();
                    readToken.append((char) readCharacter);
                }
            }
            readCharacter = cFile.next();
        }

        throw new InvalidExpressionException(readToken.toString());
    }

    private Token readAsterisk(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.next();
        if (readCharacter == -1) {
            throw new InvalidExpressionException(readToken.toString());
        } else if (readCharacter == '=') {
            readToken.append((char) readCharacter);
            readCharacter = cFile.next();
            return new Token(readToken.toString(), TokenType.ARITHMETIC_OPERATOR);
        }
        return new Token(readToken.toString(), TokenType.ASTERISK);

    }

    private Token readNumericalAsterisk(Iterator<Character> cFile, int operand, StringBuilder readToken) throws IOException, InvalidExpressionException {
        readCharacter = cFile.next();
        if ('0' <= readCharacter && readCharacter <= '9') {
            return new Token(operand * Character.getNumericValue(readCharacter) + "", TokenType.NUMERICAL_CONSTANT);
        } else if (('a' <= readCharacter && readCharacter <= 'z') ||
                ('A' <= readCharacter && readCharacter <= 'Z')) {
            readToken.append((char) readCharacter);
            return (readIdentifierOrKeyWord(readToken, cFile));
        } else {
            throw new InvalidExpressionException(readCharacter + "");
        }


    }

    private Token readBlock(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        Stack<Character> bracketStack = new Stack<Character>();
        readCharacter = cFile.next();
        while (readCharacter != -1) {
            readToken.append((char) readCharacter);
            if (readCharacter == '{') {
                bracketStack.push((char) readCharacter);
            } else if (readCharacter == '}') {
                if (bracketStack.isEmpty()) {
                    readCharacter = cFile.next();
                    return new Token(readToken.toString(), TokenType.BLOCK);
                } else {
                    bracketStack.pop();
                }
            }
            readCharacter = cFile.next();
        }

        throw new InvalidExpressionException(readToken.toString());
    }

    private Token readPreProcessorInstruction(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.next();
        while (readCharacter != -1 && (('a' <= readCharacter && readCharacter <= 'z') ||
                ('A' <= readCharacter && readCharacter <= 'Z') ||
                ('0' <= readCharacter && readCharacter <= '9'))) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.next();
        }
        String readIdentifier = readToken.toString();
        for (String keyWord : PRE_PROCESSOR_INSTRUCTION) {
            if (readIdentifier.equals(keyWord)) {
                return new Token(readIdentifier, TokenType.PRE_PROCESSOR_INSTRUCTION);
            }
        }

        throw new InvalidExpressionException(readToken.toString());
    }

    private Token readDivisionOrComment(StringBuilder readToken, Iterator<Character> cFile) throws IOException {
        readCharacter = cFile.next();
        if (readCharacter != '*' && readCharacter != '/') {
            return new Token(readToken.toString(), TokenType.ARITHMETIC_OPERATOR);
        }
        if (readCharacter == '*') {
            readToken.append(readCharacter);
            readCharacter = cFile.next();
            while (readCharacter != -1) {
                readToken.append((char) readCharacter);
                if (readCharacter == '*') {
                    readCharacter = cFile.next();
                    readToken.append((char) readCharacter);
                    if (readCharacter == '/') {
                        break;
                    }
                }
                readCharacter = cFile.next();
            }
        } else {
            readToken.append(readCharacter);
            readCharacter = cFile.next();
            while (readCharacter != -1) {
                readToken.append((char) readCharacter);
                if (readCharacter == '\n') {
                    break;
                }
                readCharacter = cFile.next();
            }
        }
        return new Token(readToken.toString(), TokenType.COMMENT);

    }

    private Token readOperator(StringBuilder readToken, Iterator<Character> cFile) throws IOException, InvalidExpressionException {
        readCharacter = cFile.next();
        while (readCharacter != -1 && (readCharacter == '!' || readCharacter == '%' || readCharacter == '+' ||
                readCharacter == '|' || readCharacter == '-' || readCharacter == '<' || readCharacter == '=' ||
                readCharacter == '>')) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.next();
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
        if (readOperator.equalsIgnoreCase("->")) {
            return new Token(readOperator, TokenType.POINT_OPERATOR);
        }

        throw new InvalidExpressionException(readOperator);
    }

    private Token readIdentifierOrKeyWord(StringBuilder readToken, Iterator<Character> cFile) throws IOException {
        readCharacter = cFile.next();
        while (readCharacter != -1 && (('a' <= readCharacter && readCharacter <= 'z') ||
                ('A' <= readCharacter && readCharacter <= 'Z') ||
                ('0' <= readCharacter && readCharacter <= '9') ||
                readCharacter == '_')) {
            readToken.append((char) readCharacter);
            readCharacter = cFile.next();
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
