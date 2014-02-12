package cestruct;

import datastruct.Archive;
import datastruct.ListD;

import java.util.StringTokenizer;

public class Compiler {

    private Archive myFile;
    private ListD preProcessor;             // BUSCAR
    private ListD adts;
    private ListD functions;
    private ListD modules;
    private ListD variables;
    private ListD candidates;

    public Compiler(String namefile) {
        myFile = new Archive(namefile);
        preProcessor = new ListD();
        adts = new ListD();
        functions = new ListD();
        modules = new ListD();
        variables = new ListD();
        candidates = new ListD();
    }

    public void run() {
        String lineFile;

        while (!myFile.eof()) {
            lineFile = myFile.readLine();
            if (!lineFile.equals("")) {
                // what if the line is a line comment?... or a block comment??

                if (Function.isMain(lineFile)) {
                    break;
                }

                if (Adt.isADT(lineFile)) {
                    doIfItsADT(lineFile, myFile, adts);

                } else if (Function.isFunction(lineFile)) {
                    Function trying = new Function(lineFile);
                    functions.addAfter(trying);

                } else if (Attribute.isVariable(lineFile)) {
                    Attribute attribute = new Attribute(lineFile);
                    variables.addAfter(attribute);

                } else if (Module.isModule(lineFile)) {
                    Module module = new Module(lineFile);
                    modules.addAfter(module);
                }
            }
        }

        myFile.position(0);
        myFile.goEnd();

        myFile.close();

        uploadModules(); //take all the information of each modules (listed in the header the main file). The names of all modules are stored in ListD modules

        createCandidates_step1();  //start merging the 4 independent lists (functions, modules, vars and ADT)
        printLists();

    }

    // this method start merging the information uploaded from the header. Only creat 1 candidate for each function
    private void createCandidates_step1() {

        functions.goFirst();
        for (int m = 0; m == functions.getLength(); m++) {
        //    candidates.addAfter(new CandidateClass((Function) functions.getActual()));
            functions.goAhead();
        }
    }

    // this method read each name of modules (stored in listD modules) and adds all the information implemented inside each file to each object of the list.
    private void uploadModules() {
        modules.goFirst();
        System.out.println("");
        int count = 1;

        while (count <= modules.getLength()) {
            ((Module) modules.getActual()).uploadDataofModule();
            modules.goAhead();
            count++;
        }

    }

    private void doIfItsADT(String lineFile, Archive myFile, ListD adts) {

        Adt tadaux = new Adt(lineFile);
        lineFile = myFile.readLine();

        while (!(tadaux.hasEnd(lineFile))) {
            if (Attribute.isVariable(lineFile)) {
                tadaux.addVariable(lineFile);
                lineFile = myFile.readLine();
            } else {
                lineFile = myFile.readLine();
            }
        }

        StringTokenizer tokenizer = new StringTokenizer(lineFile, "}");
        if (tokenizer.hasMoreTokens()) {

            String aux = tokenizer.nextToken();
            if (Attribute.isVariable(aux)) {
                tadaux.addVariable(aux);
            }
            if (tokenizer.hasMoreTokens()) {
                tadaux.setSize(tokenizer.nextToken());
            }
        }
        adts.addAfter(tadaux);
    }

    private void printLists() {

        printSimple(modules, "Modules are: ", "It has no modules");
        printSimple(functions, "Functions are: ", "It has no functions");
        printSimple(adts, "ADTs are: ", "It has no ADTs");
        printSimple(variables, "Variables are: ", "It has no variables");
    }

    private void printSimple(ListD list, String nonEmptyListMessage, String emptyListMessage) {
        if (!list.isEmpty()) {
            list.goFirst();
            System.out.println(nonEmptyListMessage);
            System.out.println("");
            int count = 1;

            while (count <= list.getLength()) {
                System.out.println(list.getActual());
                list.goAhead();
                count++;
            }
            System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        } else {
            System.out.println(emptyListMessage);
            System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        }
    }
}
