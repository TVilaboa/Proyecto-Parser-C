package javacandidatestruct;


import compiler.cstruct.Attribute;

/**
 * User: Javier Isoldi
 * Date: 3/7/13
 * Time: 6:43 PM
 */

public class JavaAttribute {

    private String type; // attribute type
    private String name; // attribute name
    private Boolean isArray;
    private int arrayCapacity; // if it's value is != from 0 this attribute is an array and it´s value indicates it size

    public JavaAttribute(String type, String name, Boolean isArray, int arrayCapacity) {
        this.type = type;
        this.name = name;
        this.isArray = isArray;
        this.arrayCapacity = arrayCapacity;
    }

    public static JavaAttribute getJavaAttributeFromCVariable(Attribute attribute){
        return new JavaAttribute(attribute.getType(), attribute.getName(), attribute.isArray(), attribute.getArrayCapax());
    }

    public String toString() {
        if (isArray) {
            return "\t" + (type + " " + name);
        } else {
            return "\t" + (type + " " + name + "[" + arrayCapacity + "]") + "\n";
        }
    }
}

