package compiler.cstruct;

import compiler.InvalidExpressionException;
import compiler.NoSupportedInstructionException;
import compiler.fileanalyzer.Token;
import compiler.fileanalyzer.TokenType;
import javacandidatestruct.CandidateClass;
import javacandidatestruct.JavaAttribute;

import java.io.IOException;
import java.util.*;

/**
 * Created by IntelliJ IDEA.
 * User: Ignacio
 * Date: 8/04/13
 * Time: 13:18
 * To change this template use File | Settings | File Templates.
 */
public class MainFunction extends Function {

    private List<Sentence> insideMainAttributes;
    private List<Sentence> sentenceList = new ArrayList<>();
    private Map<String, CandidateClass> candidates;

    public MainFunction(String returns, List<Attribute> arguments, String body, Map<String, Integer> globalAttributes,
                        List<Attribute> variables, Set<Function> functions, Map<String, CandidateClass> candidates) throws IOException, InvalidExpressionException {
        super(returns, "main", arguments, body, globalAttributes, variables, functions);
        insideMainAttributes = new ArrayList<Sentence>();
        this.candidates = candidates;
    }

    public Map<String, CandidateClass> getCandidates() {
        return candidates;
    }

    public void setCandidates(Map<String, CandidateClass> candidates) {
        this.candidates = candidates;
    }

    public List<Sentence> generateSentenceList(List<Adt> fileAdt, List<Attribute> fileAttributes,
                                               Set<Function> fileDeclaredFunctions) throws InvalidExpressionException, NoSupportedInstructionException {
        List<Attribute> internalAttributes = new ArrayList<Attribute>();
        Iterator<Token> tokenIterator = bodyTokenList.iterator();
        while (tokenIterator.hasNext()) {
            Token token = tokenIterator.next();

            switch (token.getType()) {
                case IDENTIFIER:
                    processIdentifier(token, tokenIterator, fileAdt, fileAttributes, internalAttributes,
                            fileDeclaredFunctions);
                    break;
                case COMMENT:
                    break;
                case KEY_WORD:
                    sentenceList.add(processKeyWord(token, tokenIterator, fileAttributes, internalAttributes,
                            fileDeclaredFunctions));
                    break;
                case BASIC_TYPE:
                    processBasicType(token, tokenIterator, fileAttributes, internalAttributes, fileDeclaredFunctions
                    );
                    break;
                case SENTENCE_END:
                    break;
                default:
                    throw new InvalidExpressionException(token.toString());
            }
        }

        for (Sentence sentence : sentenceList) {
            if (sentence.getType() == SentenceType.ATTRIBUTE_DECLARATION_FROM_FUNCTION || sentence.getType() == SentenceType.ATTRIBUTE_DECLARATION ||
                    sentence.getType() == SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE) {
                insideMainAttributes.add(sentence);
            }
        }
        return sentenceList;
    }

    public List<Sentence> getInsideMainAttributes() {
        return insideMainAttributes;
    }

    public void setInsideMainAttributes(List<Sentence> insideMainAttributes) {
        this.insideMainAttributes = insideMainAttributes;
    }

    public List<Sentence> getSentenceList() {
        return sentenceList;
    }

    public void setSentenceList(List<Sentence> sentenceList) {
        this.sentenceList = sentenceList;
    }

    private Sentence processKeyWord(Token token, Iterator<Token> tokenIterator, List<Attribute> fileAttributes,
                                    List<Attribute> internalAttributes, Set<Function> fileDeclaredFunctions) throws NoSupportedInstructionException, InvalidExpressionException {
        List<Token> sentenceTokens = new LinkedList<Token>();
        Sentence sentence;
        sentenceTokens.add(token);
        if (token.getValue().equals("if")) {
            readConditionSentence(tokenIterator, sentenceTokens);
            sentence = new Sentence(SentenceType.IF, sentenceTokens);
        } else if (token.getValue().equals("do")) {
            token = tokenIterator.next();
            if (token.getType() == TokenType.BLOCK) {
                sentenceTokens.add(token);
                token = tokenIterator.next();
                if (token.getType() == TokenType.KEY_WORD && token.getValue().equals("while")) {
                    token = tokenIterator.next();
                    if (token.getType() == TokenType.OPENING_BRACKET) {
                        while (token.getType() != TokenType.CLOSING_BRACKET) {
                            sentenceTokens.add(token);
                            token = tokenIterator.next();
                        }
                        sentenceTokens.add(token);
                    } else {
                        throw new InvalidExpressionException(token.toString());
                    }
                } else {
                    throw new InvalidExpressionException(token.toString());
                }
            }
            sentence = new Sentence(SentenceType.DO_WHILE, sentenceTokens);
        } else if (token.getValue().equals("while")) {
            readConditionSentence(tokenIterator, sentenceTokens);
            sentence = new Sentence(SentenceType.WHILE, sentenceTokens);
        } else if (token.getValue().equals("for")) {
            readConditionSentence(tokenIterator, sentenceTokens);
            sentence = new Sentence(SentenceType.FOR, sentenceTokens);
        } else if (token.getValue().equals("switch")) {
            readConditionSentence(tokenIterator, sentenceTokens);
            sentence = new Sentence(SentenceType.SWITCH, sentenceTokens);
        } else if (token.getValue().equals("return")) {
            readReturn(tokenIterator, sentenceTokens);
            sentence = new Sentence(SentenceType.RETURN, sentenceTokens);
        } else {
            throw new NoSupportedInstructionException(token.toString());
        }
        return sentence;
    }

    private void readReturn(Iterator<Token> tokenIterator, List<Token> sentenceTokens) {
        Token token = tokenIterator.next();
        while (token.getType() != TokenType.SENTENCE_END) {
            sentenceTokens.add(token);
            token = tokenIterator.next();
        }
        sentenceTokens.add(token);
    }

    private void readConditionSentence(Iterator<Token> tokenIterator, List<Token> sentenceTokens) throws NoSupportedInstructionException, InvalidExpressionException {
        Token token;
        token = tokenIterator.next();
        if (token.getType() == TokenType.OPENING_BRACKET) {
            while (token.getType() != TokenType.CLOSING_BRACKET) {
                sentenceTokens.add(token);
                token = tokenIterator.next();
            }
            sentenceTokens.add(token);
            token = tokenIterator.next();
            if (token.getType() == TokenType.BLOCK) {
                sentenceTokens.add(token);
            } else {
                throw new NoSupportedInstructionException(token.toString());
            }
        } else {
            throw new InvalidExpressionException(token.toString());
        }
    }

    private void processBasicType(Token token, Iterator<Token> tokenIterator, List<Attribute> fileAttributes,
                                  List<Attribute> internalAttributes, Set<Function> fileDeclaredFunctions)
            throws NoSupportedInstructionException, InvalidExpressionException {
        Sentence sentence = null;
        List<Token> sentenceTokens = new LinkedList<Token>();
        sentenceTokens.add(token);
        Token firstToken = token;
        token = tokenIterator.next();
        if (token.getType() == TokenType.IDENTIFIER) {
            sentenceTokens.add(token);
            token = tokenIterator.next();
            if (token.getType() == TokenType.SQUARE_BRACKET_BLOCK) {
                try {
                    internalAttributes.add(new Attribute(sentenceTokens.get(0).getValue(), sentenceTokens.get(1).getValue(), true,
                            Integer.parseInt(token.getValue().substring(1, token.getValue().length() - 1))));
                } catch (NumberFormatException e) {
                    internalAttributes.add(new Attribute(sentenceTokens.get(0).getValue(), sentenceTokens.get(1).getValue(), true,
                            -1));   //This is the way to deal with global variables that are changed through the execution, since
                    //i cant replace every ocurrence with a value, and i dont want to keep the value trought the program, i use -1
                }
                sentenceTokens.add(token);
                token = tokenIterator.next();
            } else
                internalAttributes.add(new Attribute(sentenceTokens.get(0).getValue(), token.getValue(), false, 0));
            if (token.getType() == TokenType.SENTENCE_END) {
                sentenceTokens.add(token);
                sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION, sentenceTokens);
            } else if (token.getType() == TokenType.COMMA_OPERATOR) {
                sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION, sentenceTokens);
            } else if (token.getType() == TokenType.ASSIGNATION_OPERATOR) {
                sentenceTokens.add(token);
                token = tokenIterator.next();
                if (token.getValue().equalsIgnoreCase("length")) {
                    do {
                        sentenceTokens.add(token);
                        token = tokenIterator.next();
                    }
                    while (token.getType() != TokenType.SENTENCE_END);
                    sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_FUNCTION, sentenceTokens);
                    sentenceList.add(sentence);
                    return;
                }

                if (token.getType() == TokenType.IDENTIFIER) {
                    for (Attribute fileAttribute : fileAttributes) {
                        if (token.getValue().equals(fileAttribute.getName())) {
                            sentenceTokens.add(token);
                            token = tokenIterator.next();
                            if (token.getType() == TokenType.SENTENCE_END) {
                                sentenceTokens.add(token);
                                sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE, sentenceTokens);
                            } else if (token.getType() == TokenType.COMMA_OPERATOR) {
                                sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE, sentenceTokens);
                            } else {
                                throw new NoSupportedInstructionException(token.toString());
                            }
                        }
                    }
                    for (Attribute internalAttribute : internalAttributes) {
                        if (token.getValue().equals(internalAttribute.getName())) {
                            sentenceTokens.add(token);
                            token = tokenIterator.next();
                            if (token.getType() == TokenType.SENTENCE_END) {
                                sentenceTokens.add(token);
                                sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE, sentenceTokens);
                            } else if (token.getType() == TokenType.COMMA_OPERATOR) {
                                sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE, sentenceTokens);
                            } else {
                                throw new NoSupportedInstructionException(token.toString());
                            }
                        }
                    }
                    for (Function fileDeclaredFunction : fileDeclaredFunctions) {
                        if (token.getValue().equals(fileDeclaredFunction.getName())) {
                            sentenceTokens.add(token);
                            token = tokenIterator.next();
                            if (token.getType() == TokenType.OPENING_BRACKET) {
                                sentenceTokens.add(token);
                                token = tokenIterator.next();
                                while (token.getType() != TokenType.CLOSING_BRACKET) {
                                    sentenceTokens.add(token);
                                    token = tokenIterator.next();
                                }
                                sentenceTokens.add(token);
                                token = tokenIterator.next();
                                if (token.getType() == TokenType.SENTENCE_END) {
                                    sentenceTokens.add(token);
                                    sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_FUNCTION, sentenceTokens);
                                } else if (token.getType() == TokenType.COMMA_OPERATOR) {
                                    sentenceTokens.add(new Token(";", TokenType.SENTENCE_END));
                                    sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_FUNCTION, sentenceTokens);
                                } else {
                                    throw new NoSupportedInstructionException(token.toString());
                                }
                            } else {
                                throw new InvalidExpressionException(token.toString());
                            }
                        }
                    }
                    if (sentence == null) {
                        //throw new NoSupportedInstructionException(token.toString());
                        while (token.getType() != TokenType.SENTENCE_END) {
                            sentenceTokens.add(token);
                            token = tokenIterator.next();
                        }
                        sentenceTokens.add(token);
                        sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION, sentenceTokens);
                    }
                } else if (token.getType() == TokenType.NUMERICAL_CONSTANT) {
                    token = readValueFromConstant(token, tokenIterator, fileAttributes, internalAttributes,
                            fileDeclaredFunctions, sentenceTokens);
                    sentence = new Sentence(SentenceType.ATTRIBUTE_VALUE_FROM_CONSTANT, sentenceTokens);

                } else {
                    throw new NoSupportedInstructionException(token.toString()); //TODO prepare to support other sentence types
                }
            } else {
                throw new InvalidExpressionException(token.toString());
            }
        }
        if (sentence != null)                    //happens when an attribute from an adt is modified from the main
            sentenceList.add(sentence);
        if (token.getType() == TokenType.COMMA_OPERATOR) {
            processBasicType(firstToken, tokenIterator, fileAttributes, internalAttributes, fileDeclaredFunctions);
        }
    }

    private Token readValueFromConstant(Token token, Iterator<Token> tokenIterator, List<Attribute> fileAttributes,
                                        List<Attribute> internalAttributes, Set<Function> fileDeclaredFunctions, List<Token> sentenceTokens) {
        while (token.getType() != TokenType.SENTENCE_END && token.getType() != TokenType.COMMA_OPERATOR) {
            sentenceTokens.add(token);
            token = tokenIterator.next();
        }
        sentenceTokens.add(new Token(";", TokenType.SENTENCE_END));
        return token;
    }

    private void processIdentifier(Token token, Iterator<Token> tokenIterator, List<Adt> fileAdt, List<Attribute> fileAttributes,
                                   List<Attribute> internalAttributes, Set<Function> fileDeclaredFunctions) throws NoSupportedInstructionException, InvalidExpressionException {
        if (token.getValue().equals("fprintf")) {
            processFprintF(tokenIterator, sentenceList);
            return;
        }
        if (token.getValue().equals("fwrite")) {
            processFwrite(tokenIterator, sentenceList);
            return;
        }

        if (!token.getValue().equals("printf") && !token.getValue().equals("scanf")) {
            Sentence sentence = null;
            for (Adt adt : fileAdt) {
            if (adt.getName().equals(token.getValue())) {
                processBasicType(token, tokenIterator, fileAttributes, internalAttributes, fileDeclaredFunctions);
                return;
            }
        }


//        for (Attribute fileAttribute : fileAttributes) {
//            //TODO seguir comparando nombres de indentificadores con los que se tiene previos
//        }

        List<Token> sentenceTokens = new ArrayList<Token>();
        sentenceTokens.add(token);
        token = tokenIterator.next();
            if (token.getType() == TokenType.POINT_OPERATOR || token.getType() == TokenType.ASSIGNATION_OPERATOR) {
                while (token.getType() != TokenType.SENTENCE_END) {
                sentenceTokens.add(token);
                token = tokenIterator.next();
            }
            sentenceTokens.add(token);
            return;
            } else {
                while (token.getType() != TokenType.SENTENCE_END) {
                    token = tokenIterator.next();
                }
            }
        } else {
            while (token.getType() != TokenType.SENTENCE_END) {
                token = tokenIterator.next();
            }
        }
    }

    /**
     * @param token
     * @param tokenIterator
     * @param sentenceTokens        : sentence first tokens, type and identifier;
     * @param fileAttributes
     * @param internalAttributes
     * @param fileDeclaredFunctions
     * @return
     * @throws NoSupportedInstructionException
     *
     * @throws InvalidExpressionException
     */
    private Sentence readAttributeValueSentence(Token token, Iterator<Token> tokenIterator, List<Token> sentenceTokens, List<Attribute> fileAttributes,
                                                List<Attribute> internalAttributes, List<Function> fileDeclaredFunctions)
            throws NoSupportedInstructionException, InvalidExpressionException {
        Sentence sentence = null;
        token = tokenIterator.next();
        if (token.getType() == TokenType.ASSIGNATION_OPERATOR) {
            sentenceTokens.add(token);
            token = tokenIterator.next();
            if (token.getType() == TokenType.IDENTIFIER) {
                for (Attribute fileAttribute : fileAttributes) {
                    if (token.getValue().equals(fileAttribute.getName())) {
                        sentenceTokens.add(token);
                        token = tokenIterator.next();
                        if (token.getType() == TokenType.SENTENCE_END) {
                            sentenceTokens.add(token);
                            sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE, sentenceTokens);
                        } else {
                            throw new NoSupportedInstructionException(token.toString());
                        }
                    }
                }
                for (Attribute internalAttribute : internalAttributes) {
                    if (token.getValue().equals(internalAttribute.getName())) {
                        sentenceTokens.add(token);
                        token = tokenIterator.next();
                        if (token.getType() == TokenType.SENTENCE_END) {
                            sentenceTokens.add(token);
                            sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_ATTRIBUTE, sentenceTokens);
                        } else {
                            throw new NoSupportedInstructionException(token.toString());
                        }
                    }
                }
                for (Function fileDeclaredFunction : fileDeclaredFunctions) {
                    if (token.getValue().equals(fileDeclaredFunction.getName())) {
                        sentenceTokens.add(token);
                        token = tokenIterator.next();
                        if (token.getType() == TokenType.SENTENCE_END) {
                            sentenceTokens.add(token);
                            sentence = new Sentence(SentenceType.ATTRIBUTE_DECLARATION_FROM_FUNCTION, sentenceTokens);
                        } else {
                            throw new NoSupportedInstructionException(token.toString());
                        }
                    }
                }
                if (sentence == null) {
                    throw new NoSupportedInstructionException(token.toString());
                }
            }
            throw new NoSupportedInstructionException(token.toString()); //TODO prepare to support other sentence types
        }
        return sentence;
    }

    @Override
    public String toString() {
        String result = "MainFunction" +
                "\n\tGlobal Sentences are :";
        for (Sentence globalAttribute : insideMainAttributes) {
            result += globalAttribute.toString();
        }

        return result;

    }


    //create CCD from fwrite using file name as name and first parameter as attribute

    private void processFwrite(Iterator<Token> tokenIterator, List<Sentence> sentenceList) throws InvalidExpressionException {
        Token token = tokenIterator.next();

        if (token.getType() == TokenType.OPENING_BRACKET) {
            token = tokenIterator.next();
            String attributeName = token.getValue();
            if (token.getType() == TokenType.IDENTIFIER) {
                for (int i = 0; i < 6; i++) {
                    token = tokenIterator.next();
                }
                if (token.getType() == TokenType.IDENTIFIER) {
                    CandidateClass clazz = new CandidateClass(token.getValue());
                    if (candidates.containsKey(clazz.getName())) {
                        clazz = candidates.get(clazz.getName());
                    }
                    clazz.addAttribute(new JavaAttribute("Unknown", attributeName, false, 0));
                    candidates.put(clazz.getName(), clazz);
                    while (token.getType() != TokenType.SENTENCE_END) {
                        token = tokenIterator.next();
                    }
                    return;
                } else throw new InvalidExpressionException(token.toString());
            } else throw new InvalidExpressionException(token.toString());
        } else throw new InvalidExpressionException(token.toString());
    }

    //creates CCD from fprintf, using file name as name, and variables as attributes. Only if used outside control flow ,same with attributes
    private void processFprintF(Iterator<Token> tokenIterator, List<Sentence> sentenceList) throws InvalidExpressionException {
        Token token = tokenIterator.next();
        CandidateClass clazz = null;
        if (token.getType() == TokenType.OPENING_BRACKET) {
            token = tokenIterator.next();
            if (token.getType() == TokenType.IDENTIFIER) {
                clazz = new CandidateClass(token.getValue());
                if (candidates.containsKey(clazz.getName())) {
                    clazz = candidates.get(clazz.getName());
                } else {
                    token = tokenIterator.next();
                    if (token.getType() == TokenType.COMMA_OPERATOR) {
                        token = tokenIterator.next();
                        if (token.getType() == TokenType.CHAR_CHAIN) {
                            token = tokenIterator.next();
                            if (token.getType() == TokenType.COMMA_OPERATOR) {
                                do {
                                    token = tokenIterator.next();
                                    if (token.getType() == TokenType.IDENTIFIER) {
                                        boolean insideControlFlowAttribute = true;
                                        for (Sentence sentence : sentenceList) {
                                            if (sentence.getSentenceComponents().get(1).getValue().equals(token.getValue())) {
                                                boolean exist = false;
                                                insideControlFlowAttribute = false;
                                                for (Iterator<JavaAttribute> iterator = clazz.getAttributes().iterator(); iterator.hasNext(); ) {
                                                    JavaAttribute attribute = iterator.next();
                                                    if (attribute.getName().equals(sentence.getSentenceComponents().get(0))) {
                                                        exist = true;
                                                        break;
                                                    }
                                                }
                                                //adds attributes that are defined outside any flow control
                                                if (!exist) {
                                                    if (sentence.getSentenceComponents().get(2).getType() == TokenType.SQUARE_BRACKET_BLOCK) {
                                                        String array = sentence.getSentenceComponents().get(2).getValue();
                                                        clazz.addAttribute(new JavaAttribute(sentence.getSentenceComponents().get(0).getValue(),
                                                                sentence.getSentenceComponents().get(1).getValue(), true,
                                                                Integer.parseInt(array.substring(1, array.length() - 1))));
                                                    } else
                                                        clazz.addAttribute(new JavaAttribute(sentence.getSentenceComponents().get(0).getValue(),
                                                                sentence.getSentenceComponents().get(1).getValue(), false, 0));
                                                }
                                            }


                                        }
                                        if (insideControlFlowAttribute) {
                                            clazz.addAttribute(new JavaAttribute("Unknown", token.getValue(), false, 0));
                                        }

                                    }
                                }
                                while (token.getType() != TokenType.CLOSING_BRACKET);
                                token = tokenIterator.next();
                                candidates.put(clazz.getName(), clazz);
                                return;

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
            } else {
                throw new InvalidExpressionException(token.toString());
            }
        } else {
            throw new InvalidExpressionException(token.toString());
        }
    }
}
