package cestruct;

import datastruct.ListD;

import java.util.StringTokenizer;

public class Adt {
    private String name; // name of the cestruct.Adt
    private ListD variables; // attributes of the cestruct.Adt

    public Adt(String line) {
        setName(line);
        variables = new ListD();
    }

    private void setName(String line) {
        StringTokenizer st = new StringTokenizer(line);
        st.nextToken();
        name = st.nextToken();
        if (name.contains("{")) {
            String s = "";
            for (int i = 0; i < (name.length() - 1); i++) {
                s += name.charAt(i);
            }
            name = s;
        }
    }

    public void addVariable(String line) {
        variables.addAfter(new Attribute(line));
    }

    public boolean hasEnd(String line) {
        if (line.equals("")) {
            return false;
        }
        StringTokenizer st = new StringTokenizer(line);
        String s = st.nextToken();
        return (s.contains("}"));
    }

    public void setSize(String line) {
        StringTokenizer st = new StringTokenizer(line);
        String s = st.nextToken();
        String size = "";
        int arrayLength;
        if (s.contains("[")) {
            int m = getPosition(s);
            while (s.charAt(m) != ']') {
                size += s.charAt(m);
                m++;
            }
            arrayLength = Integer.parseInt(size);
        } else
            arrayLength = 0;
    }

    private int getPosition(String s) {
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == '[')
                return (i + 1);
        }
        return -1;
    }

    public static boolean isADT(String line) {
        if (line.equals("")) {
            return false;
        }
        StringTokenizer st = new StringTokenizer(line);
        String result = st.nextToken();
        return (result.equals("struct") || result.equals("typedef"));
    }

    public String toString() {
        String result = ("\tThe name is : " + name);
        if (variables != null) {
            result += ("\n\tThe variables are: ");
            variables.goFirst();
            for (int i = 0; i < variables.getLength(); i++) {
                result += "\n\t" + (variables.getActual());
                variables.goAhead();
            }
        }
        return result;
    }
}
