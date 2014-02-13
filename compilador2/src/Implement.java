import compiler.InvalidExpressionException;
import compiler.NoSupportedInstructionException;
import compiler.cstruct.Compiler;

import java.io.IOException;

/**
 * Created by IntelliJ IDEA.
 * User: Ignacio
 * Date: 25/10/12
 * Time: 12:05
 * To change this template use File | Settings | File Templates.
 */
public class Implement {

    public static void main (String [] Args) {
        Compiler myCompiler = new Compiler("Aeropuerto.C");
        try {
            myCompiler.run();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InvalidExpressionException e) {
            e.printStackTrace();
        } catch (NoSupportedInstructionException e) {
            e.printStackTrace();
        }
    }
}
