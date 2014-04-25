package compiler.cstruct;

import compiler.InvalidExpressionException;
import compiler.fileanalyzer.Token;
import compiler.fileanalyzer.TokenListFactory;
import compiler.fileanalyzer.TokenType;

import java.io.IOException;
import java.io.StringReader;
import java.util.*;

/**
 * User: Javier Isoldi
 * Date: 4/29/13
 * Time: 12:55 PM
 */
public class Function implements Comparable<Function> {

    protected String body;
    protected List<Token> bodyTokenList;
    private String returns;
    private String name;
    private List<Attribute> arguments;
    private Map<String, Integer> globalAttributes;
    private List<Attribute> globalVariablesUsed = new ArrayList<>();
    private List<Function> usedFunctions = new ArrayList<>();

    public Function(String returns, String name, List<Attribute> arguments, Map<String, Integer> globalAttributes, List<Attribute> variables
    ) {
        this.returns = returns;
        this.name = name;
        this.arguments = arguments;
        this.globalAttributes = globalAttributes;

    }

    public Function(String returns, String name, List<Attribute> arguments, String body,
                    Map<String, Integer> globalAttributes, List<Attribute> variables, Set<Function> functions) throws IOException, InvalidExpressionException {
        this.returns = returns;
        this.name = name;
        this.arguments = arguments;
        this.body = body;
        this.globalAttributes = globalAttributes;
        TokenListFactory tokenListFactory = new TokenListFactory(globalAttributes);
        String bodyWithOutBrackets = body.substring(1, body.length() - 1);
        bodyTokenList = tokenListFactory.getTokenFileFromCFile(new StringReader(bodyWithOutBrackets));
        for (Token token : bodyTokenList) {
            for (Attribute attribute : variables) {
                if (attribute.getName().equals(token.getValue())) {
                    globalVariablesUsed.add(attribute);
                }
            }
            for (Function function : functions) {
                if (function.getName().equals(token.getValue()) && !usedFunctions.contains(function)) {
                    usedFunctions.add(function);
                }
            }
        }

    }

    // args : token = initial bracket, tokenIterator = iterator of the following tokens
    public static List<Attribute> getArguments(Token token, Iterator<Token> tokenIterator, List<Adt> adts) throws InvalidExpressionException {
        List<Attribute> arguments = new ArrayList<Attribute>();
        token = tokenIterator.next();

        while (token.getType() != TokenType.CLOSING_BRACKET && token.getType() != TokenType.SENTENCE_END) {
            if (token.getType() == TokenType.BASIC_TYPE) {
                String type = token.getValue();
                token = tokenIterator.next();
                token = readAttributeWithType(type, token, tokenIterator, arguments);
            } else if (token.getType() == TokenType.IDENTIFIER) {
                String type = null;
                for (Adt adt : adts) {
                    if (token.getValue().equals(adt.getName())) {
                        type = adt.getName();
                        break;
                    }
                }
                if (type != null) {
                    token = tokenIterator.next();
                    token = readAttributeWithType(type, token, tokenIterator, arguments);
                    if (token.getType() == TokenType.COMMA_OPERATOR) {
                        token = tokenIterator.next();
                    }
                } else {
                    throw new InvalidExpressionException(token.toString());
                }
            } else {
                throw new InvalidExpressionException(token.toString());
            }
        }
        return arguments;
    }

    private static Token readAttributeWithType(String type, Token token, Iterator<Token> tokenIterator, List<Attribute> attributes) throws InvalidExpressionException {
        Attribute attribute = null;
        if (token.getType() == TokenType.IDENTIFIER) {
            String name = token.getValue();
            token = tokenIterator.next();
            if (token.getType() == TokenType.SQUARE_BRACKET_BLOCK) {
                attribute = new Attribute(type, name, true, 0);
                token = tokenIterator.next();
            } else {
                attribute = new Attribute(type, name, false, 0);
            }


        } else if (token.getType() == TokenType.SQUARE_BRACKET_BLOCK) {
            attribute = new Attribute(type, "", true, 0);
            token = tokenIterator.next();
        } else if (token.getType() == TokenType.COMMA_OPERATOR || token.getType() == TokenType.CLOSING_BRACKET) {

            attribute = new Attribute(type, "", false, 0);
        } else if (type.equals("void")) {
            while (token.getType() == TokenType.CLOSING_BRACKET || token.getType() == TokenType.OPENING_BRACKET
                    || token.getValue().equals("void")) {
                token = tokenIterator.next();
            }
            attribute = new Attribute(type, "", false, 0);
        } else {
            throw new InvalidExpressionException(token.toString());
        }
        attributes.add(attribute);
        if (token.getType() == TokenType.COMMA_OPERATOR) {
            token = tokenIterator.next();
        }

        return token;
    }

    public List<Function> getUsedFunctions() {
        return usedFunctions;
    }

    public boolean hasBody() {
        return body != null;
    }

    public String getReturns() {
        return returns;
    }

    public String getName() {
        return name;
    }

    public List<Attribute> getArguments() {
        return arguments;
    }

    public String toString() {
        String result = ("\tFunction name: " + name + "\n\tReturns: " + returns);
        if (arguments != null) {
            result += ("\n\tThe arguments are:");
            for (Attribute argument : arguments) {
                result += "\n\t" + argument;
            }
        }
        return result + "\n";
    }

    public String getBody() {
        return body;
    }

    @Override
    public int compareTo(Function o) {
        final int nameResult = this.name.compareTo(o.getName());
        if (nameResult != 0) {
            return nameResult;
        }

        final int returnResult = returns.compareTo(o.getReturns());
        if (returnResult != 0) {
            return returnResult;
        }

        final int argumentSizeResult = arguments.size() - o.arguments.size();
        if (argumentSizeResult != 0) {
            return argumentSizeResult;
        }

        int argumentResult;
        for (int i = 0; i < arguments.size(); i++) {
            argumentResult = arguments.get(i).getType().compareTo(o.arguments.get(i).getType());
            if (argumentResult != 0) {
                return argumentResult;
            }
        }

        return 0;
    }

    public boolean hasArgument(Attribute argument) {
        for (Attribute attribute : arguments) {
            if (attribute.compareTo(argument) == 0) {
                return true;
            }
        }
        return false;
    }

    public List<Attribute> getGlobalVariablesUsed() {
        return globalVariablesUsed;
    }
}
