package compiler.cstruct;

import java.io.File;
import java.util.*;

/**
 * User: Javier Isoldi
 * Date: 4/29/13
 * Time: 12:53 PM
 */
public class Module {

    // list of modules that must not be considered as part of the project (or the particular system).
    // This list contain modules that are part of the language (e.g. stdio.h)
    private static final String[] MODULE_LIST;
    private File file; // name of the module
    private List<Module> modulesIncluded; //list of includes defined in the header
    private List<Function> functions; //list of functions defined and implemented
    private List<Attribute> atributes;
    private Map<String, Integer> defines;

    public Module(File moduleFile) {
        file = moduleFile;
        functions = new LinkedList<Function>();
        modulesIncluded = new LinkedList<Module>();
        atributes = new ArrayList<>();
        defines = new TreeMap<String, Integer>();
    }

    public static String[] getModuleList() {
        return MODULE_LIST;
    }

    public void addFunction(Function function) {
        functions.add(function);
    }

    public void addModule(Module module) {
        modulesIncluded.add(0, module);
    }
    static {
        MODULE_LIST = new String[]{"assert.h", "complex.h", "ctype.h",
                "errno.h", "fenv.h", "float.h",
                "inttypes.h", "iso646.h", "limits.h",
                "locale.h", "math.h", "setjmp.h",
                "signal.h", "stdarg.h", "stdbool.h",
                "stdint.h", "stddef.h", "stdio.h",
                "stdlib.h", "string.h", "tgmath.h",
                "time.h", "wchar.h", "wctype.h",
                "windows.h", "conio.h", "fcntl.h", "unistd.h"};
    }

    public List<Attribute> getAtributes() {
        return atributes;
    }

    public void setAtributes(List<Attribute> atributes) {
        this.atributes = atributes;
    }

    public void addAttribute(Attribute attribute) {
        atributes.add(attribute);
    }

    public void addDefine(Map.Entry<String, Integer> entry) {
        defines.put(entry.getKey(), entry.getValue());
    }

    public Map<String, Integer> getDefines() {
        return defines;
    }

    public void setDefines(Map<String, Integer> defines) {
        this.defines = defines;
    }

    public boolean isBasicModule() {
        boolean aux = false;
        String module = file.getName();
        for (String moduleListItem : MODULE_LIST) {
            if (module.equals(moduleListItem)) {
                aux = true;
            }
        }
        return aux;
    }

    //get the name of the module, open the file and store the information (functions) in the object
    public void uploadDataofModule() {
    }

    public List<Module> getModulesIncluded() {
        return modulesIncluded;
    }

    public void setModulesIncluded(List<Module> modulesIncluded) {
        this.modulesIncluded = modulesIncluded;
    }

    public List<Function> getFunctions() {
        return functions;
    }

    public void setFunctions(List<Function> functions) {
        this.functions = functions;
    }

    private boolean endOfDefinition(String lineFile) {
        if (lineFile.contains("{")) {
//            System.out.println("encontre el comienzo de la implementacion");
            return true;
        } else
            return false;
    }

    public String toString() {
        String modulePrint = null;
        if (!isBasicModule()) {
            modulePrint = "\t\"" + file.getName() + "\"\n\n";
            if (functions != null) {
                if (!functions.isEmpty()) {
                    String intro = "\tThe functions in this module are: \n";
                    modulePrint += intro + printList(functions);
                } else {
                    String intro = "\tThis module has no functions \n";
                    modulePrint += intro;
                }
            }
            if (modulesIncluded != null) {
                if (!modulesIncluded.isEmpty()) {
                    String intro = "\tThe modules of this module are: \n";
                    modulePrint += intro + printList(modulesIncluded);
                } else {
                    String intro = "\tThis module has no modules \n";
                    modulePrint += intro;
                }

            }
            if (atributes != null) {
                if (!atributes.isEmpty()) {
                    String intro = "\tThe attributes of this module are: \n";
                    modulePrint += intro + printList(atributes);
                } else {
                    String intro = "\tThis module has no attributes \n";
                    modulePrint += intro;
                }

            }
        } else {
            modulePrint = "\t\"" + file.getName() + "\"\n\n" + "This is a basic module.\n";
        }

        return modulePrint;
    }

    private String printList(List list) {
        String aux = "";

        for (Object o : list) {
            aux += "\n" + o.toString() + "\n\t ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n";
        }

        aux += "\n";
        return aux;
    }

    public File getFile() {
        return file;
    }

    public void setFile(File file) {
        this.file = file;
    }
}

