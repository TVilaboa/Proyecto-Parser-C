package cestruct;//FALTA REVISAR

import java.util.StringTokenizer;

public class Function {
    private String returns;
    private String name;
    private String[] arguments;
    protected String body;


    public Function(String returns, String name, String[] arguments) {
        this.returns = returns;
        this.name = name;
        this.arguments = arguments;
    }

    public Function(String line) {

        StringTokenizer comprob = new StringTokenizer(line, "(");

        String aux = comprob.nextToken();
        StringTokenizer tokenizerAux = new StringTokenizer(aux);

        returns = tokenizerAux.nextToken();
        name = tokenizerAux.nextToken();
        aux = comprob.nextToken();
        tokenizerAux = new StringTokenizer(aux, ",");
        arguments = new String[tokenizerAux.countTokens()];
        aux = tokenizerAux.nextToken();
        int cont = 0;
        while (aux.charAt(aux.length() - 1) != ';') {
            arguments[cont] = aux;
            cont++;
            aux = tokenizerAux.nextToken();
        }
        String nameArg = "";
        cont = 0;
        while (aux.charAt(cont) != ')' && aux.length() > cont) {
            nameArg = nameArg + aux.charAt(cont);
            cont++;
        }

        arguments[arguments.length - 1] = nameArg;

    }


    public static boolean isFunction(String workingLine) {
        StringTokenizer st = new StringTokenizer(workingLine);
        if (st.hasMoreElements()) {
            while (st.countTokens() > 1) {
                st.nextToken();
            }
            String lastToken = st.nextToken();
            if (lastToken.length() >= 2 && (lastToken.subSequence(lastToken.length() - 2, lastToken.length())).equals(");")) {
                return true;
            }
        }
        return false;
    }

    public static boolean isMain(String line) {
        StringTokenizer comprob = new StringTokenizer(line);
        while (comprob.hasMoreTokens()) {
            String testing = comprob.nextToken();
            if (testing.length() >= 4 && testing.substring(0, 4).equals("main")) {
                return true;
            }
        }
        return false;
    }

    public String getReturns() {
        return returns;
    }

    public String getName() {
        return name;
    }

    public String[] getArguments() {
        return arguments;
    }

    public String toString() {
        String result = ("\tFunction name: " + name + "\n\tReturns: " + returns);
        if (arguments != null) {
            result += ("\n\tThe arguments are:");
            for (String argument : arguments) {
                result += "\n\t" + argument;
            }
        }
        return result;
    }
}
