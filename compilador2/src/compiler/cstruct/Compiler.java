package compiler.cstruct;

import compiler.InvalidExpressionException;
import compiler.NoSupportedInstructionException;
import compiler.fileanalyzer.Token;
import compiler.fileanalyzer.TokenListFactory;
import compiler.fileanalyzer.TokenType;
import javacandidatestruct.CandidateClass;
import javacandidatestruct.JavaAttribute;
import javacandidatestruct.JavaMethod;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class Compiler {

    private List<Adt> adts;
    private Set<Function> functions;
    private MainFunction mainFunction;
    private List<Module> modules;
    private List<Attribute> attributes;
    private Map<String, CandidateClass> candidates;
    private Map<String, Integer> globalAttributes;
    private List<File> alreadyProcessed;

    //private List<Token> tokenList;

    public Compiler() {
        // myFile = new File(fileName);
        adts = new ArrayList<Adt>();
        functions = new TreeSet<Function>();
        modules = new ArrayList<Module>();
        attributes = new ArrayList<Attribute>();
        candidates = new TreeMap<String, CandidateClass>();
        globalAttributes = new TreeMap<String, Integer>();
        alreadyProcessed = new ArrayList<>();
    }

    public void run(File myFile) throws IOException, InvalidExpressionException, NoSupportedInstructionException {
        System.out.println("Analisys for : " + myFile + "\n");
        subrun(myFile);
        if (mainFunction != null) {
            mainFunction.generateSentenceList(adts, attributes, functions);
        }
        printLists();
    }


    private void subrun(File myFile) throws IOException, InvalidExpressionException, NoSupportedInstructionException {
        alreadyProcessed.add(myFile);
        TokenListFactory tokenListFactory = new TokenListFactory(globalAttributes);
        List<Token> tokenList = tokenListFactory.getTokenFileFromCFile(new BufferedReader(new FileReader(myFile)));
        for (int i = 0; i < tokenList.size(); i++) {
            Token token = tokenList.get(i);
            if (token.getValue().equals("short") || token.getValue().equals("unsigned")) {
                tokenList.remove(token);
            }
        }
        preProcess(tokenList, myFile);
        uploadModules(); //take all the information of each modules (listed in the header the main file). The names of all modules are stored in ListD modules
        //first upload modules so that module´s functions and structs can be recognised later
        Iterator<Token> tokenIterator = tokenList.iterator();
        while (tokenIterator.hasNext()) {
            Token token = tokenIterator.next();
            switch (token.getType()) {
                case IDENTIFIER:
                    processIdentifier(token, tokenIterator);                  //There are 2 cases, a global attribute or a function that returns a struct .1er case treated in prerpocess
                    break;
                case COMMENT:
                    break;
                case NUMERICAL_CONSTANT:
                    break;
                case CHAR_CHAIN:
                    break;
                case ARITHMETIC_OPERATOR:
                    break;
                case ASSIGNATION_OPERATOR:
                    break;
                case INCREMENTAL_OPERATOR:
                    break;
                case ASSOCIATIVE_OPERATOR:
                    break;
                case LOGIC_OPERATOR:
                    break;
                case NEG_OPERATOR:
                    break;
                case POSITIVE_OPERATOR:
                    break;
                case SIZE_OF_OPERATOR:
                    break;
                case LOGIC_NEG_OPERATOR:
                    break;
                case COMMA_OPERATOR:
                    break;
                case BLOCK:
                    break;
                case SQUARE_BRACKET_BLOCK:
                    break;
                case KEY_WORD:
                    processKeyWord(token, tokenIterator);
                    break;
                case BASIC_TYPE:
                    processBasicType(token, tokenIterator);
                    break;
                case SENTENCE_END:
                    break;
                case OPENING_BRACKET:
                    break;
                case CLOSING_BRACKET:
                    break;
                case FILE_TO_INCLUDE:
                    break;
                case ASTERISK:
                    break;
                case ADDRESS_OPERATOR:
                    break;
                default:
                    break;
            }
        }


        createCandidatesFromADTs();  //start merging the 4 independent lists (functions, modules, vars and ADT)
        for (int i = 0; i < modules.size(); i++) {
            Module module = modules.get(i);
            if (myFile.equals(module.getFile())) {
                for (Iterator<Function> iterator = functions.iterator(); iterator.hasNext(); ) {
                    Function function = iterator.next();
                    module.addFunction(function);
                }

            }
        }


    }

    private void processIdentifier(Token token, Iterator<Token> tokenIterator) throws InvalidExpressionException, IOException {
        for (Adt adt : adts) {
            if (token.getValue().equals(adt.getName())) {
                processBasicType(token, tokenIterator);
                return;
            }
        }
        throw new InvalidExpressionException(token.toString());

    }

    private void processKeyWord(Token token, Iterator<Token> tokenIterator) throws NoSupportedInstructionException, IOException, InvalidExpressionException {
        if (token.getValue().equals("struct")) {
            processStruct(tokenIterator);
        } else if (token.getValue().equals("typedef")) {
            token = tokenIterator.next();
            if (token.getType() == TokenType.KEY_WORD && token.getValue().equals("struct")) {
                processStruct(tokenIterator);
            }
        } else {
            throw new NoSupportedInstructionException();
        }
    }

    private void processStruct(Iterator<Token> tokenIterator) throws IOException, InvalidExpressionException, NoSupportedInstructionException {
        Token token;
        token = tokenIterator.next();
        if (token.getType() == TokenType.IDENTIFIER) {
            String name = token.getValue();
            token = tokenIterator.next();
            if (token.getType() == TokenType.BLOCK) {
                List<Attribute> adtAttributes = Adt.readAttributesFromBlock(token, adts);
                adts.add(new Adt(name, adtAttributes, globalAttributes));
            }
        } else if (token.getType() == TokenType.BLOCK) {
            Token block = token;
            token = tokenIterator.next();
            if (token.getType() == TokenType.IDENTIFIER) {
                List<Attribute> adtAttributes = Adt.readAttributesFromBlock(block, adts);
                adts.add(new Adt(token.getValue(), adtAttributes, globalAttributes));
            }
        } else {
            throw new NoSupportedInstructionException();
        }
    }

    private void processBasicType(Token token, Iterator<Token> tokenIterator) throws InvalidExpressionException,
            IOException {
        String type = token.getValue();
        token = tokenIterator.next();
        if (token.getType() == TokenType.IDENTIFIER) {
            String name = token.getValue();
            token = tokenIterator.next();
            if (token.getType() == TokenType.OPENING_BRACKET) {
                List<Attribute> arguments = Function.getArguments(token, tokenIterator, adts);
                token = tokenIterator.next();
                if (token.getType() == TokenType.SENTENCE_END) {
                    addFunction(new Function(type, name, arguments, globalAttributes));
                } else if (token.getType() == TokenType.BLOCK) {
                    if (!name.equals("main")) {
                        addFunction(new Function(type, name, arguments, token.getValue()));
                    } else {
                        mainFunction = new MainFunction(type, arguments, token.getValue());
                    }
                } else {
                    throw new InvalidExpressionException(token.toString());
                }
            } else if (token.getType() == TokenType.SQUARE_BRACKET_BLOCK) {
                token = tokenIterator.next();
                if (token.getType() == TokenType.SENTENCE_END) {
                    attributes.add(new Attribute(type, name, true, 0));
                } else {
                    throw new InvalidExpressionException(token.toString());
                }
            } else if (token.getType() == TokenType.SENTENCE_END) {
                attributes.add(new Attribute(type, name, false, 0));
            } else {
                throw new InvalidExpressionException(token.toString());
            }
        } else {
            throw new InvalidExpressionException(token.toString());
        }

    }

    private void addFunction(Function function) {
        if (!functions.contains(function)) {
            functions.add(function);
        }
    }

    private void preProcess(List<Token> tokenList, File myFile) throws InvalidExpressionException {
        for (int i = 0; i < tokenList.size(); i++) {
            if (tokenList.get(i).getType() == TokenType.PRE_PROCESSOR_INSTRUCTION) {
                if (tokenList.get(i).getValue().equals("#include")) {
                    i = processInclude(tokenList, i, myFile);
                }
                if (tokenList.get(i).getValue().equals("#define")) {
                    i = processDefine(tokenList, i);
                }
                if (tokenList.get(i).getValue().equals("#ifndef")) {
                    i = processIfndef(tokenList, i); //tiene q omitir los proximos 3 tokens
                }
            }
        }
    }

    private int processIfndef(List<Token> tokenList, int i) throws InvalidExpressionException {
        tokenList.remove(i);  //#ifndef
        tokenList.remove(i);  // identifier
        tokenList.remove(i);  //#define
        tokenList.remove(i);  //identifier
        return i;

    }


    private int processDefine(List<Token> tokenList, int i) throws InvalidExpressionException {
        tokenList.remove(i);
        Token token = tokenList.get(i);
        if (token.getType() == TokenType.IDENTIFIER) {
            globalAttributes.put(token.getValue(), Integer.parseInt(tokenList.get(i + 1).getValue()));
            tokenList.remove(i);
            tokenList.remove(i);
            return i - 1;
        } else {
            throw new InvalidExpressionException(token.toString());
        }
    }

    private int processInclude(List<Token> tokenList, int i, File myFile) {
        tokenList.remove(i);
        Token token = tokenList.get(i);
        if (token.getType() == TokenType.FILE_TO_INCLUDE) {
            Module module = new Module(new File(token.getValue().substring(1, token.getValue().length() - 1)));
            for (Module addedModule : modules) {
                if (addedModule.getFile().getName().equals(module.getFile().getName())) {
                    tokenList.remove(i);
                    return i;

                }
            }
            modules.add(module);
            for (Module module1 : modules) {
                if (myFile.equals(module1.getFile())) {  //submodules loading. Module is the new module and module1 is the module in which the parser is working
                    module1.addModule(module);
                    break;

                }
            }

            tokenList.remove(i);
        }
        return i;
    }

    // this method start merging the information uploaded from the header. Only creat 1 candidate for each function
    private void createCandidatesFromADTs() {
        for (Adt adt : adts) {
            CandidateClass candidateClass = new CandidateClass(adt.getName());
            for (Attribute attribute : adt.getAttributes()) {
                candidateClass.addAttribute(JavaAttribute.getJavaAttributeFromCVariable(attribute));
            }
            for (Function function : functions) {
                if (function.getReturns().equals(adt.getName())) {
                    candidateClass.addMethod(JavaMethod.getJavaMethodFromCFunction(function));
                } else {
                    for (Attribute attribute : function.getArguments()) {
                        if (attribute.getType().equals(adt.getName())) {
                            candidateClass.addMethod(JavaMethod.getJavaMethodFromCFunction(function));
                            break;
                        }
                    }
                }
            }
            candidates.put(candidateClass.getName(),candidateClass);
        }
    }

    private void createCandidatesFromAttributes() {
        for (Function function : functions) {
            final Set<Attribute> attributeSet = new TreeSet<Attribute>();
            for (Attribute attribute : attributes) {
                if (function.hasArgument(attribute)){
                    attributeSet.add(attribute);
                }
            }
            if (!attributeSet.isEmpty()){
               // candidates.containsKey();
            }
        }
    }

    private void createCandidatesFromFunction() {
        for (Function function : functions) {
            CandidateClass candidateClass = new CandidateClass(function.getName());
            candidateClass.addMethod(JavaMethod.getJavaMethodFromCFunction(function));
            candidates.put(candidateClass.getName(),candidateClass);
        }
    }


    // this method read each name of modules (stored in listD modules) and adds all the information implemented inside
    // each file to each object of the list.
    private void uploadModules() throws InvalidExpressionException, NoSupportedInstructionException, IOException {

        for (int i = 0; i < modules.size(); i++) {
            Module module = modules.get(i);
            if (!module.isBasicModule() && !alreadyProcessed.contains(module.getFile()))
                this.subrun(module.getFile());
        }

    }


    private void printLists() {
        printSimple(modules, "Modules are: ", "It has no modules");
        printSimple(functions, "Functions are: ", "It has no functions");
        printSimple(adts, "ADTs are: ", "It has no ADTs");
        printSimple(attributes, "Variables are: ", "It has no attributes");
        printSimple(globalAttributes.entrySet(), "GlobalVariables are: ", "It has no GlobalVariables");
        printSimple(candidates.values(), "Candidate classes are:", "No candidates are suggested");

        /*if (mainFunction != null) {
            System.out.println("+------------------------------------+");
            System.out.println(mainFunction);
        }*/

    }

    private void printSimple(Collection list, String nonEmptyListMessage, String emptyListMessage) {
        if (!list.isEmpty()) {
            System.out.println(nonEmptyListMessage);
            System.out.println("");
            for (Object o : list) {
                System.out.println(o.toString());
            }
            System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        } else {
            System.out.println(emptyListMessage);
            System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        }
    }
}
