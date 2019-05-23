package logic.man;

import exceptions.InvalidWaiterParamException;
import logic.entities.ChopStick;
import logic.entities.Seat;

/**<h1>Waiter</h1>
 *
 * The Waiter class serves as an arbiter for all the
 * philosophers, controlling their eating process, and
 * distributing seats among them. It also serves to assign
 * chopsticks to the seats (because of the abstraction, according to which
 * this is one of waiter's duties: to lay down chopsticks on the table with fixed seats' amount)
 * @see logic.man.Philosopher
 * @see logic.entities.ChopStick
 * @see logic.entities.Seat
 */
public class Waiter {
    private ChopStick[] stickList;
    private Seat[] seatList;

    public Waiter(ChopStick[] stickList, Seat[] seatList) throws InvalidWaiterParamException {
        if (stickList.length == 0)
            throw new InvalidWaiterParamException("No chopsticks found!");
        if (seatList.length == 0)
            throw new InvalidWaiterParamException("No seats found!");

        if (stickList.length != seatList.length)
            throw new InvalidWaiterParamException("The amount of seats must match the amount of chopsticks!");

        this.seatList = seatList;
        this.stickList = stickList;

        for(int i = 0; i < this.seatList.length - 1; i++) {
            this.seatList[i].setRightStick(this.stickList[i]);
            this.seatList[i].setLeftStick(this.stickList[i + 1]);
        }
        this.seatList[seatList.length - 1].setRightStick(this.stickList[0]);
        this.seatList[seatList.length - 1].setLeftStick(this.stickList[stickList.length - 1]);
    }

    /**
     * This method is used to synchronize those Philosophers,
     * who are currently waiting, and not eating. If there is an empty
     * seat, the Waiter gives Philosopher a permission to take it.
     * @throws InterruptedException If the thread was interrupted
     * @return Seat
     * @see logic.man.Philosopher
     * @see logic.entities.Seat
     */
    public synchronized
    Seat comeInPermission() throws InterruptedException {

        boolean continueFlag = true;
        Seat freeSeat = null;

        while(continueFlag) {
            for (Seat seat : seatList) {
                if (!seat.isOccupied()) {
                    freeSeat = seat;
                    freeSeat.occupy();
                    continueFlag = false;
                    break;
                }
            }
            if (continueFlag) wait();
        }

        return freeSeat;
    }

    /**
     * This method is used to release seats, when a Philosopher
     * has finished "eating"
     * @param seat The seat, which must be released
     * @return void
     * @see logic.entities.Seat
     */
    public synchronized void seatReleased(Seat seat) {
        seat.release();
        notifyAll();
    }
}