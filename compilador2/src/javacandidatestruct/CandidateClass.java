package javacandidatestruct;

import compiler.cstruct.Attribute;

import java.io.Serializable;
import java.util.Collection;
import java.util.LinkedList;
import java.util.List;

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
        attributes = new LinkedList<JavaAttribute>();
        methods = new LinkedList<JavaMethod>();
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
