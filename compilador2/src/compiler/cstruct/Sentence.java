package compiler.cstruct;

import compiler.fileanalyzer.Token;

import java.util.List;

/**
 * Created by IntelliJ IDEA.
 * User: Ignacio
 * Date: 8/04/13
 * Time: 14:16
 * To change this template use File | Settings | File Templates.
 */
public class Sentence {
    private SentenceType type;
    private List<Token> sentenceComponents;

    public Sentence(SentenceType type, List<Token> sentenceComponents) {
        this.type = type;
        this.sentenceComponents = sentenceComponents;
    }

    public SentenceType getType() {
        return type;
    }

    public List<Token> getSentenceComponents() {
        return sentenceComponents;
    }

    @Override
    public String toString() {
        String result = "";
        for (Token sentenceComponent : sentenceComponents) {
            result += sentenceComponent.getValue() + " ";
        }

        return result;
    }
}
