import compiler.InvalidExpressionException;
import compiler.NoSupportedInstructionException;
import compiler.cstruct.Compiler;

import java.io.File;
import java.io.IOException;

/**
 * Created by IntelliJ IDEA.
 * User: Ignacio
 * Date: 25/10/12
 * Time: 12:05
 * To change this template use File | Settings | File Templates.
 */
public class Implement {

    public static void main(String[] Args) {
        Compiler myCompiler = new Compiler();
        try {
//            myCompiler.run();

            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 1\\SimpleAirport.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 2\\elevs.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 3\\WAV\\makeWav.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 4\\light.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 5\\ballotBox.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 6\\University.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 7\\cars.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 8\\airport.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 9\\bank.c"));
            myCompiler = new Compiler();
            myCompiler.run(new File("C:\\Users\\Toto\\Projects\\Proyecto Nacho Cassol\\Proyecto-Parser-C\\compilador2\\Lote de pruebas\\Case 10\\calculator.c"));

        } catch (IOException | NoSupportedInstructionException | InvalidExpressionException e) {
            e.printStackTrace();
        }
    }
}
