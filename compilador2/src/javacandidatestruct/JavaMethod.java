package javacandidatestruct;

import compiler.cstruct.Attribute;
import compiler.cstruct.Function;

import java.util.List;

/**
 * User: Javier Isoldi
 * Date: 3/7/13
 * Time: 6:43 PM
 */
public class JavaMethod {

    private String returnType;
    private String name;
    private List<Attribute> arguments;
    private String body;

    public JavaMethod(String returnType, String name, List<Attribute> arguments1, String body) {
        this.returnType = returnType;
        this.name = name;
        this.arguments = arguments1;
        this.body = body;
    }



    public static JavaMethod getJavaMethodFromCFunction(Function function){
        return new JavaMethod(function.getReturns(), function.getName(), function.getArguments(), function.getBody());
    }

    public String toString() {
        String result = ("\tMethod name: " + name + "\n\tReturns: " + returnType);
        if (arguments != null) {
            result += ("\n\tThe arguments are:");
            for (Attribute argument : arguments) {
                result += "\n\t" + argument;
            }
        }
        return result;
    }

}

