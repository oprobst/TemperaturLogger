package de.oliverprobst.temperaturelog;

public class ManualTest {

	public static void main(String[] args) {
		
		TemperaturReceiver rec = new TemperaturReceiver();
		try {
			float f = rec.readCurrentTemperature();
			System.err.println(f);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		rec.close();
	}

}
