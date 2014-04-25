package compiler.cstruct;

import java.io.Serializable;

/**
 * User: Javier Isoldi
 * Date: 4/29/13
 * Time: 12:55 PM
 */
public class Attribute implements Comparable<Attribute>, Serializable {
    private String type; // type of variable
    private String name; // name of the variable
    private boolean isArray;
    private int arrayCapax; // capacity of a array containing variables of this type

    public Attribute(String type, String name, boolean array, int arrayCapax) {
        this.type = type;
        this.name = name;
        isArray = array;
        this.arrayCapax = arrayCapax;
    }

    public String getType() {
        return type;
    }

    public String getName() {
        return name;
    }

    public boolean isArray(){
        return isArray;
    }

    public int getArrayCapax() {
        return arrayCapax;
    }

    public String toString() {
        if (!isArray) {
            return "\t" + (type + " " + name);
        } else {
            return "\t" + (type + " " + name + "[" + arrayCapax + "]") + "\n";
        }
    }

    @Override
    public int compareTo(Attribute o) {
        final int nameResult = name.compareTo(o.getName());
        if (nameResult != 0) {
            return nameResult;
        }

        final int returnResult = type.compareTo(o.type);
        if (returnResult != 0) {
            return returnResult;
        }

        return 0;
    }
}
