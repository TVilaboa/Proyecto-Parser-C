package cestruct;

import datastruct.Archive;
import datastruct.ListD;

import java.io.File;
import java.util.StringTokenizer;

public class Module {

    private String name; // name of the module
    private ListD modulesIncluded; //list of includes defined in the header
    private ListD functions; //list of functions defined and implemented

    // list of modules that must not be considered as part of the project (or the particular system).
    // This list contain modules that are part of the language (e.g. stdio.h)
    private static final String[] MODULELIST;

    static {
        MODULELIST = new String[]{"assert.h", "complex.h", "ctype.h",
                "errno.h", "fenv.h", "float.h",
                "inttypes.h", "iso646.h", "limits.h",
                "locale.h", "math.h", "setjmp.h",
                "signal.h", "stdarg.h", "stdbool.h",
                "stdint.h", "stddef.h", "stdio.h",
                "stdlib.h", "string.h", "tgmath.h",
                "time.h", "wchar.h", "wctype.h"};
    }


    public Module(File moduleFile){

    }

    public Module(String line) {
        if (isModule(line)) {
            StringTokenizer stringTokenizer = new StringTokenizer(line);
            stringTokenizer.nextToken();
            name = stringTokenizer.nextToken();
            name = name.substring(1, name.length() - 1);
        }
        modulesIncluded = new ListD();
        functions = new ListD();
    }

    public static boolean isModule(String line) {
        if (line.equals("")) {
            return false;
        }
        StringTokenizer stringTokenizer = new StringTokenizer(line);
        String token = stringTokenizer.nextToken();
        if (!token.equals("#include")) {
            return false;
        }
        if (!stringTokenizer.hasMoreTokens()) {
            return false;
        }
        String token2 = stringTokenizer.nextToken();

        return forEquals(token2);
    }

    private static boolean forEquals(String token2) {
        boolean aux = true;
        token2 = token2.substring(1, token2.length() - 1);
        for (String moduleListItem : MODULELIST) {
            if (token2.equals(moduleListItem)) {
                aux = false;
            }
        }
        return aux;
    }

    //get the name of the module, open the file and store the information (functions) in the object
    public void uploadDataofModule() {
//        System.out.println("I am in uploadDataofModule");
        Archive FileofModule = new Archive(name);
//        System.out.println(name);
        String lineFile;

        while (!FileofModule.eof()) {
            lineFile = FileofModule.readLine();
            if (!lineFile.equals("")) {
                // what if the line is a line comment?... or a block comment??

                if (endOfDefinition(lineFile)) {
                    break;
                }
                // upload the definition of the functions
                if (Function.isFunction(lineFile)) {
                    Function trying = new Function(lineFile);
                    functions.addAfter(trying);
                } else if (Module.isModule(lineFile)) {
                    Module module = new Module(lineFile);
//                  I have to make it recursive because I have to upload the information of each module of this module.
//                  It's necessary to check if next line works properly...
                    module.uploadDataofModule();

                    modulesIncluded.addAfter(module);
                }
            }

        }
    }

    public ListD getModulesIncluded() {
        return modulesIncluded;
    }

    public ListD getFunctions() {
        return functions;
    }

    private boolean endOfDefinition(String lineFile) {
        if (lineFile.contains("{")) {
//            System.out.println("encontre el comienzo de la implementacion");
            return true;
        } else
            return false;
    }

    public String toString() {
        String modulePrint = "\t\"" + name + "\"\n\n";
        if (functions != null) {
            if (!functions.isEmpty()) {
                String intro = "\tThe functions in this module are: \n";
                modulePrint += intro + printFunction(functions, intro);
            } else {
                String intro = "\tThis module has no functions \n";
            }
        }
        if (modulesIncluded != null) {
            if (!functions.isEmpty()) {
                String intro = "\tThe modules of this module are: \n";
                modulePrint += intro + printModule(modulesIncluded, intro);
            } else {
                String intro = "\tThis module has no modules: \n";
            }

        }

        return modulePrint;
    }

    private String printFunction(ListD list, String string) {
        list.goFirst();
        String aux = "";
        int count = 1;

        while (count <= list.getLength()) {
            aux += "\n" +  list.getActual() + "\n\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
            list.goAhead();
            count++;
        }
        aux += "\n";
        return aux;
    }

    private String printModule(ListD list, String string) {
        list.goFirst();
        String aux = "";
        int count = 1;

        while (count <= list.getLength()) {
            aux += "\n" + list.getActual() + "\n\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
            list.goAhead();
            count++;
        }
        aux += "\n";
        return aux;
    }

}
