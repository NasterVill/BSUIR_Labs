package logic.entities;

/**<h1>Seat</h1>
 *
 * The Seat class gives a philosopher an ability
 * to "eat: with the given chopsticks. It is used to control
 * and synchronize the "eating" process between waiting and
 * operating philosophers
 * @see logic.man.Philosopher
 * @see logic.entities.ChopStick
 */
public class Seat {

    private boolean occupied;
    private ChopStick leftStick;
    private ChopStick rightStick;

    public ChopStick leftStick() {
        return this.leftStick;
    }

    public ChopStick rightStick() {
        return this.rightStick;
    }

    public void setLeftStick(ChopStick leftStick) {
        this.leftStick = leftStick;
    }

    public void setRightStick(ChopStick rightStick) {
        this.rightStick = rightStick;
    }

    public void occupy() {
        this.occupied = true;
    }

    public void release() {
        this.occupied = false;
    }

    public boolean isOccupied() {
        return this.occupied;
    }

    public Seat() {
        this.leftStick = null;
        this.rightStick = null;
        this.occupied = false;
    }

    public Seat(ChopStick leftStick, ChopStick rightStick) {
        this.leftStick = leftStick;
        this.rightStick = rightStick;
        this.occupied = false;
    }
}
