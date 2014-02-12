package cestruct;

import java.util.StringTokenizer;

public class Attribute {
    private String type; // type of variable
    private String name; // name of the variable
    private int arrayCapax; // capacity of a array containing variables of this type

    // this list contain the primitive types of data that are considered and accepted.
    private final static String[] TYPES;

    static {
        TYPES = new String[]{"int", "char", "double", "float", "long",
                "byte", "boolean", "short", "String"};
    }

    public Attribute(String type, String name, int arrayCapax) {
        this.type = type;
        this.name = name;
        this.arrayCapax = arrayCapax;
    }

    public Attribute(String line) {
        if (isVariable(line)) {
            StringTokenizer stringTokenizer = new StringTokenizer(line);

            type = stringTokenizer.nextToken();
            name = stringTokenizer.nextToken();

            String arraySizeString;
            arrayCapax = 0;

            if (name.contains("[")) {
                arraySizeString = name.substring(name.indexOf("[") + 1, name.indexOf("]"));
                name = name.substring(0, name.indexOf("["));
                arrayCapax = Integer.parseInt(arraySizeString);
            } else {
                if (name.contains(";")) {
                    name = name.substring(0, name.length() - 1);
                } else {
                    name = name.substring(0, name.length());
                }
            }
        }
    }



    public static boolean isVariable(String line) {
        if (line.equals("")) {
            return false;
        }
        StringTokenizer stringTokenizer = new StringTokenizer(line);
        String token = stringTokenizer.nextToken();
        if (!stringTokenizer.hasMoreTokens()) {
            return false;
        }
        String token2 = stringTokenizer.nextToken();

        return (forEquals(token) && (!token2.contains("(") && !token2.contains("{")));
    }

    private static boolean forEquals(String token){
        boolean aux = false;
        for (String TYPE : TYPES) {
            aux = aux || token.equals(TYPE);
        }
        return aux;
    }

    public String getType() {
        return type;
    }

    public String getName() {
        return name;
    }

    public int getArrayCapax() {
        return arrayCapax;
    }

    public String toString() {
        if (arrayCapax == 0) {
            return "\t" + (type + " " + name);
        } else {
            return "\t" + (type + " " + name + "[" + arrayCapax + "]") + "\n";
        }
    }
}