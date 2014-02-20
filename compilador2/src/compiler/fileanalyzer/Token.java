package compiler.fileanalyzer;

/**
 * User: Javier Isoldi
 * Date: 3/15/13
 * Time: 11:43 AM
 */
public class Token {
    private String value;
    private TokenType type;

    public Token(String value, TokenType type) {
        this.value = value;
        this.type = type;
    }

    public TokenType getType() {
        return type;
    }

    public String getValue() {
        return value;
    }

    @Override
    public String toString() {
        return "Token{" + " value = '" + value + '\'' + ", type = " + type + '}';
    }

    public void setValue(String value) {
        this.value = value;
    }
}
