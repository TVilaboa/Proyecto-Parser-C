package compiler.cstruct;

import compiler.InvalidExpressionException;
import compiler.fileanalyzer.Token;
import compiler.fileanalyzer.TokenListFactory;
import compiler.fileanalyzer.TokenType;

import java.io.IOException;
import java.io.StringReader;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

/**
 * User: Javier Isoldi
 * Date: 4/29/13
 * Time: 1:12 PM
 */
public class Adt {
    private String name; // name of the Adt
    private List<Attribute> attributes; // attributes of the Adt

    public Adt(String name, List<Attribute> attributes) {
        this.name = name;
        this.attributes = attributes;
    }

    public String getName() {
        return name;
    }

    public List<Attribute> getAttributes() {
        return attributes;
    }

    public String toString() {
        String result = ("\tThe name is : " + name);
        if (attributes != null) {
            result += ("\n\tThe variables are: ");
            for (Attribute attribute : attributes) {
                result += "\n\t" + (attribute.toString());
            }
        }
        return result;
    }

    public static List<Attribute> readAttributesFromBlock(Token token, List<Adt> adts) throws IOException, InvalidExpressionException {
        String adtBody = token.getValue();
        String adtBodyWithoutBrackets = adtBody.substring(1, adtBody.length() - 1);
        TokenListFactory tokenListFactory = new TokenListFactory();
        Iterator<Token> tokenIterator;
        tokenIterator = tokenListFactory.getTokenFileFromCFile(new StringReader(adtBodyWithoutBrackets)).iterator();

        List<Attribute> attributeList = new LinkedList<Attribute>();

        while (tokenIterator.hasNext()) {
            token = tokenIterator.next();
            attributeList.add(readAttribute(token, tokenIterator, adts));
        }

        return attributeList;
    }

    private static Attribute readAttribute(Token token, Iterator<Token> tokenIterator, List<Adt> adts) throws InvalidExpressionException {
        if (token.getType() == TokenType.BASIC_TYPE) {
            String type = token.getValue();
            token = tokenIterator.next();
            return readAttributeWithType(type, token, tokenIterator);
        } else if (token.getType() == TokenType.IDENTIFIER) {
            for (Adt adt : adts) {
                if (token.getValue().equals(adt.getName())) {
                    String type = token.getValue();
                    token = tokenIterator.next();
                    return readAttributeWithType(type, token, tokenIterator);
                }
            }
        }
        throw new InvalidExpressionException(token.toString());

    }

    private static Attribute readAttributeWithType(String type, Token token, Iterator<Token> tokenIterator) throws InvalidExpressionException {
        if (token.getType() == TokenType.IDENTIFIER) {
            String name = token.getValue();
            token = tokenIterator.next();
            if (token.getType() == TokenType.SENTENCE_END) {
                return new Attribute(type, name, false, 0);
            } else if (token.getType() == TokenType.SQUARE_BRACKET_BLOCK) {
                token = tokenIterator.next();
                if (token.getType() == TokenType.SENTENCE_END) {
                    return new Attribute(type, name, true, 1);
                } else {
                    throw new InvalidExpressionException(token.toString());
                }
            } else {
                throw new InvalidExpressionException(token.toString());
            }
        } else {
            throw new InvalidExpressionException(token.toString());
        }
    }
}
