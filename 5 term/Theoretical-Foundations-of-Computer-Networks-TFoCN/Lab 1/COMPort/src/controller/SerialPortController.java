package controller;

import jssc.SerialPort;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import jssc.SerialPortList;

import java.util.ArrayList;
import java.util.Arrays;

public class SerialPortController {
    public static final byte XON_CHARACTER = 17;
    public static final byte XOFF_CHARACTER = 19;

    private static final int BAUD_RATE = SerialPort.BAUDRATE_9600;
    private static int DATA_BITS = SerialPort.DATABITS_8;
    private static final int PARITY = SerialPort.PARITY_NONE;
    private static final int STOP_BITS = SerialPort.STOPBITS_1;

    private SerialPort serialPort = null;
    private SerialPortEventListener serialPortEventListener;
    private boolean XonXoffFlowControlEnable = false;
    private boolean XoffIsSet = true;
    private int receivedBytes;
    private int sentBytes;

    public static ArrayList<String> getPortNames() {
        return new ArrayList<>(Arrays.asList(
                SerialPortList.getPortNames(
                )));
    }

    public void setSerialPort(SerialPort serialPort, boolean XonXoffFlowControlEnable) throws SerialPortException {
        this.serialPort = serialPort;
        this.XonXoffFlowControlEnable = XonXoffFlowControlEnable;

        serialPort.openPort();

        serialPort.setParams(BAUD_RATE,
                DATA_BITS,
                STOP_BITS,
                PARITY);

        serialPort.addEventListener(this.serialPortEventListener);
    }

    public SerialPortController(SerialPortEventListener serialPortEventListener) {
        this.serialPortEventListener = serialPortEventListener;
    }

    public void setReceivedBytes(int receivedBytes) {
        this.receivedBytes = receivedBytes;
    }

    public int getReceivedBytes() {
        return receivedBytes;
    }

    public void setSentBytes(int sentBytes) {
        this.sentBytes = sentBytes;
    }

    public int getSentBytes() {
        return sentBytes;
    }

    public void setXoffState(boolean value) {
        this.XoffIsSet = value;
    }

    public boolean getXonXoffFlowControlMode() {
        return this.XonXoffFlowControlEnable;
    }

    public boolean portIsOpened() {
        return this.serialPort != null;
    }

    public boolean closeSerialPort() throws SerialPortException{
        sentBytes = 0;
        receivedBytes = 0;
        boolean result = this.serialPort.closePort();
        this.serialPort = null;

        return result;
    }

    public void clearBuffer() throws SerialPortException {
        this.serialPort.purgePort(SerialPort.PURGE_RXCLEAR);
    }

    public boolean sendXon() throws SerialPortException{
        return this.serialPort.writeByte(SerialPortController.XON_CHARACTER);
    }

    public boolean sendXoff() throws SerialPortException {
        return this.serialPort.writeByte(SerialPortController.XOFF_CHARACTER);
    }

    public byte[] read(int byteCount) throws SerialPortException {
        byte[] result = this.serialPort.readBytes(byteCount);

        if(this.XonXoffFlowControlEnable) {
            if (result[0] != XON_CHARACTER && result[0] != XOFF_CHARACTER) {
                receivedBytes += result.length;
            }
        } else {
            receivedBytes += result.length;
        }

        return result;
    }

    public boolean write(byte[] source) throws SerialPortException  {
        boolean result = false;

        if(!(this.XoffIsSet && this.XonXoffFlowControlEnable)) {

            result = this.serialPort.writeBytes(source);

            if (result) {
                this.sentBytes += source.length;
            }
        }

        return result;
    }
}
