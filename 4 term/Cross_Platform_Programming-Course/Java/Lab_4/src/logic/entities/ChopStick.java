package logic.entities;

import java.util.concurrent.*;
import java.util.*;

/**<h1>ChopStick</h1>
 *
 * The ChopStick class represents the limited resource,
 * which can be used by different Philosophers to "eat"
 * @see logic.man.Philosopher
 */
public class ChopStick {

    private boolean taken = false;

    /**
     * This method is used to synchronize chopstick's taking
     * procedure among all Philosophers
     * @throws InterruptedException If the thread was interrupted
     * @return void
     * @see logic.man.Philosopher
     */
    public synchronized
    void take() throws InterruptedException {
        while (taken) {
            wait();
        }
        taken = true;
    }

    /**
     * This method is used to synchronize chopstick's dropping
     * procedure among all Philosophers
     * @throws InterruptedException If the thread was interrupted
     * @return void
     * @see logic.man.Philosopher
     */
    public synchronized void drop() {
        taken = false;
        notifyAll();
    }
}