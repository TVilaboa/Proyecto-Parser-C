package javacandidatestruct;


import compiler.cstruct.Attribute;
import compiler.cstruct.Function;
import compiler.cstruct.Module;

import java.io.Serializable;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

/**
 * Created by IntelliJ IDEA.
 * User: Ignacio
 * Date: 27/11/12
 * Time: 17:02
 * To change this template use File | Settings | File Templates.
 */
public class CandidateClass implements Serializable {
    private List<JavaAttribute> attributes;
    private List<JavaMethod> methods;
    private String name;

    public CandidateClass(String name) {
        this.name = name;
        attributes = new LinkedList<>();
        methods = new LinkedList<>();
    }

    public CandidateClass(Module module) {
        name = module.getFile().getName();
        attributes = new LinkedList<>();
        methods = new LinkedList<>();
        parseModule(module);
//        for (Module module1 : module.getModulesIncluded()) {
//            parseModule(module1);
//        }
    }

    private void parseModule(Module module) {
        for (Function function : module.getFunctions()) {
            addMethod(new JavaMethod(function.getReturns(), function.getName(),
                    function.getArguments(), function.getBody()));
        }
        for (Attribute attribute : module.getAtributes()) {
            addAttribute(new JavaAttribute(attribute.getType(), attribute.getName(),
                    attribute.isArray(), attribute.getArrayCapax()));
        }
        for (Map.Entry<String, Number> entry : module.getDefines().entrySet()) {
            addAttribute(new JavaAttribute("Constant : ->" + entry.getValue(), entry.getKey(), false, 0));
        }  //they are constants
    }

    public void addAttribute(JavaAttribute attribute) {
        attributes.add(attribute);
    }

    public void addMethod(JavaMethod method) {
        methods.add(method);
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder("CandidateClass: " + name +
                "\n\tAttributes are: ");
        for (JavaAttribute attribute : attributes) {
            result.append("\n\t\t");
            result.append(attribute);
        }
        result.append("\n\tMethods are: ");
        for (JavaMethod method : methods) {
            result.append("\n\t\t");
            result.append(method);
        }
        return result.toString();

    }

    public List<JavaMethod> getMethods() {
        return methods;
    }

    public void setMethods(List<JavaMethod> methods) {
        this.methods = methods;
    }

    public List<JavaAttribute> getAttributes() {
        return attributes;
    }

    public void setAttributes(List<JavaAttribute> attributes) {
        this.attributes = attributes;
    }

    public String generateName(Collection<Attribute> attributes){
        StringBuilder stringBuilder = new StringBuilder();
        for (Attribute attribute : attributes) {
            stringBuilder.append(attribute.getName()).append(attribute.getName());
        }
        return stringBuilder.toString();
    }

    public String getName() {
        return name;
    }
}
