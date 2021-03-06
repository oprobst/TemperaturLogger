package de.oliverprobst.temperaturelog;

import java.nio.charset.Charset;

import com.fazecast.jSerialComm.SerialPort;

public class TemperaturReceiver {

	private final byte[] requestTemperature = { 'T' };
	private final byte[] requestMemory = { 0x02 };
	private final byte[] requestVerbose = { 'V' };

	private final SerialPort comPort;

	public TemperaturReceiver() {
		int port = 1;
		if (System.getProperty("port") != null) {
			port = Integer.valueOf(System.getProperty("port"));

		}

		comPort = SerialPort.getCommPorts()[port];

		comPort.setComPortParameters(4800, 8, 1, SerialPort.EVEN_PARITY);
		comPort.setFlowControl(SerialPort.FLOW_CONTROL_DISABLED);
		comPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 300, 0);
		comPort.openPort();
	}

	public void close() {
		comPort.closePort();

	}

	public float readCurrentTemperature() throws Exception {

		StringBuffer result = new StringBuffer();

		try {

			while (result.length() < 6) {
				comPort.writeBytes(requestTemperature, 1);

				byte[] readBuffer = new byte[8];
				int numRead = comPort.readBytes(readBuffer, readBuffer.length);

				System.out.println("Read " + numRead + " bytes.");

				String read = new String(readBuffer, Charset.forName("UTF-8"));

				if (read.startsWith("T")) {
					result.append(read);
				} else {
					System.out.println("discarding " + read.length() + " received bytes: '" + read + "'");
				}

			}

			float res = parseInt(result);
			return res;

		} catch (Exception e) {
			e.printStackTrace();

		}
		throw new Exception("No value received");

	}

	double lastValue = -10000;

	private float parseInt(StringBuffer result) throws Exception {

		String res = result.toString();
		System.out.println(res);
		res = res.substring(1, 5);

		float measure = Float.parseFloat(res.toString());

		if (lastValue > -9999.0 && (measure < lastValue - 600 || measure > lastValue + 600)) {
			throw new Exception("Strange value received: " + measure / 100);
		}
		lastValue = measure;
		return measure / 100;
	}
}