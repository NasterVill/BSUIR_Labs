package gui;

import javafx.scene.control.TextArea;
import java.io.IOException;
import java.io.OutputStream;

/**<h1>CustomConsole</h1>
 *
 * The CustomConsole class serves to contain
 * a TextArea and replace standard System.out
 * with custom one, represented by this class
 * @see TextArea
 */
public class CustomConsole extends OutputStream {

    private TextArea output;

    public CustomConsole(TextArea ta) {
        this.output = ta;
    }

    @Override
    public void write(int i) throws IOException {
        this.output.appendText(String.valueOf((char) i));
    }

    public void clear() {
        this.output.clear();
    }
}