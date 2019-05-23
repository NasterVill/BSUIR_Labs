package logic.man;

import logic.entities.Seat;
import exceptions.InvalidPhilosopherParamException;
import java.util.concurrent.*;
import java.util.*;


/**<h1>Philosopher</h1>
 *
 * The Philosopher class is the representation of thread,
 * which is trying to get access to a limited resources:
 * ChopSticks and Seats. His main purpose is to "eat", by
 * keeping ownership of these resources for some period of time
 * @see Runnable
 * @see logic.entities.ChopStick
 * @see logic.entities.Seat
 */
public class Philosopher implements Runnable {
    private static final Integer RandomConstantOne = 42;
    private static final Integer RandomConstantTwo = 250;

    private Waiter waiter;
    private final int id;
    private final int delayTime;
    private Random rand = new Random(RandomConstantOne);

    private void actionPause() throws InterruptedException {
        if (delayTime == 0) {
            return;
        }
        TimeUnit.MILLISECONDS.sleep(
                rand.nextInt(delayTime * RandomConstantTwo));
    }

    public Philosopher(int id, int delayTime, Waiter waiter)
            throws InvalidPhilosopherParamException {
        if(id < 0)
            throw new InvalidPhilosopherParamException("The value of param Id can not be less than negative");
        if(delayTime <= 0)
            throw new InvalidPhilosopherParamException("The value of param DelayTime must be positive");
        if(waiter == null)
            throw new InvalidPhilosopherParamException("The value of param Waiter can not be null");

        this.id = id;
        this.delayTime = delayTime;
        this.waiter = waiter;
    }

    /**
     * This method is implementing the "eating" process
     * @throws InterruptedException If the thread was interrupted
     * @return void
     */
    @Override
    public void run() {
        try {
            System.out.println(this + " " + "is waiting for a seat");
            Seat seat = this.waiter.comeInPermission();

            System.out.println(this + " " + "has come in and now is thinking");
            actionPause();

            seat.rightStick().take();
            System.out.println(this + " " + "has grabbed right stick");

            seat.leftStick().take();
            System.out.println(this + " " + "has grabbed left stick");

            actionPause();
            System.out.println(this + " " + "has finished eating");

            seat.rightStick().drop();
            System.out.println(this + " " + "has dropped right stick");

            seat.leftStick().drop();
            System.out.println(this + " " + "has dropped left stick");


            System.out.println(this + " " + "has released the seat");
            notifyWaiter(seat);
        } catch (InterruptedException e) {
            System.out.println(this + " " + "exiting via interrupt");
        }
    }

    private void notifyWaiter(Seat seat) {
        waiter.seatReleased(seat);
    }

    @Override
    public String toString() {
        return "Philosopher " + id;
    }
}
