package datastruct;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

public class Archive {

    private File name;
    private RandomAccessFile file; //los metodos de randomAccesFile estan en ingles, no se puede cambiar

    public Archive(String nameFile) {
        try {
            name = new File(nameFile);
            file = new RandomAccessFile(name, "rw");
        } catch (IOException e) {
            System.out.println("ERROR");
        }
    }

    public String readChar() {
        try {
            Character a = new Character((char) file.readByte());
            return a.toString();
        } catch (IOException e) {
            System.out.println("ERROR");
            return null;
        }
    }

    public void writeChar(String x) {		// puede ser saveChar tambien
        try {
            file.writeBytes(x);
        } catch (IOException e) {
            System.out.println("ERROR");
        }

    }

    public String readLine() {
        try {
            return file.readLine();
        } catch (IOException e) {
            System.out.println("ERROR");
            return null;
        }

    }

    public void saveString(String x) {		// puede ser saveString
        try {
            file.writeBytes(x);
        } catch (IOException e) {
            System.out.println("ERROR");
        }
    }

    public void position(long x) {
        try {
            file.seek(x);
        } catch (IOException e) {
            System.out.println("ERROR");
        }
    }

    public long getPosition() {
        try {
            return file.getFilePointer();
        } catch (IOException e) {
            System.out.println("ERROR");
            return 0;
        }
    }

    public boolean eof() {
        try {
            return (file.length() == file.getFilePointer());
        } catch (IOException e) {
            System.out.println("ERROR");
            return true;
        }
    }

    public void goEnd() {
        try {
            file.seek(file.length());
        } catch (IOException e) {
            System.out.println("ERROR");
        }
    }

    public long length() {
        try {
            return file.length();
        } catch (IOException e) {
            System.out.println("ERROR");
            return 0;

        }
    }

    public void close(){
        try {
            file.close();
        } catch (IOException e) {
            System.out.println("ERROR");
        }
    }

}
